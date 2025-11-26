/**************************************************************************/
/*  simulation_domain.cpp                                                 */
/**************************************************************************/
/*                         This file is part of:                          */
/*                 GODOT ENGINE /// THE CONSERVATORY FORK                 */
/*          https://godotengine.org /// https://xansworkshop.com          */
/**************************************************************************/
/*                     DERIVED FROM GODOT SOURCE CODE                     */
/*                       SEE ORIGINAL LICENSE BELOW                       */
/**************************************************************************/
/* Copyright (c) 2025-present Xan's Workshop.                             */
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#if !defined(PHYSICS_3D_DISABLED) && !defined(PHYSICS_2D_DISABLED) && !defined(_3D_DISABLED)
#include "simulation_domain.h"

#define TC_SIMULATION_DOMAIN_WORLD_2D_MISSING "The 2D world was deleted, or was not created yet. Did you remember to call initialize after construction?"
#define TC_SIMULATION_DOMAIN_WORLD_3D_MISSING "The 3D world was deleted, or was not created yet. Did you remember to call initialize after construction?"

SimulationDomain *SimulationDomain::current;
bool *SimulationDomain::is_client;
Ref<World2D> SimulationDomain::parent_world_2d;
Ref<World3D> SimulationDomain::parent_world_3d;
ObjectID SimulationDomain::last_parent;

void SimulationDomain::set_is_client_ptr(int64_t p_ptr) {
	ERR_FAIL_COND_MSG(is_client != nullptr, "Illegal attempt to use set_is_client_ptr.");
	ERR_FAIL_COND_MSG(p_ptr == 0, "Invalid null pointer for set_is_client_ptr.");
	is_client = (bool*)p_ptr;
}

bool SimulationDomain::current_is_this(const Viewport* p_viewport) {
	ERR_FAIL_NULL_V(p_viewport, false);
	if (!current) return false;
	return last_parent == p_viewport->get_instance_id();
}

// The notification order is: Predelete, Exit Tree, Unparented

void SimulationDomain::_notification(int p_what) {
	ERR_MAIN_THREAD_GUARD;
	switch (p_what) {
		/*
		* Occurs too late.
		case Node::NOTIFICATION_POSTINITIALIZE:
			if (domain_world_2d.is_null()) {
				domain_world_2d.instantiate();
			}
			if (domain_world_3d.is_null()) {
				domain_world_3d.instantiate();
			}
			*/
		case Node::NOTIFICATION_PREDELETE_CLEANUP:
			if (SimulationDomain::current == this) {
				deactivate();
			}
			domain_world_2d = nullptr;
			domain_world_3d = nullptr;
		case Node::NOTIFICATION_EXIT_TREE:
			if (SimulationDomain::current == this) {
				WARN_PRINT("Deactivating the current domain because it exited the scene tree.");
				deactivate();
			}
			break;
	}
}

bool SimulationDomain::get_active() const {
	return SimulationDomain::current == this;
}

void SimulationDomain::make_active() {
	ERR_MAIN_THREAD_GUARD
	ERR_FAIL_COND_MSG(SimulationDomain::current == this, "This SimulationDomain is already active.");
	ERR_FAIL_COND_MSG(SimulationDomain::is_client == nullptr, "The boolean designating whether or not this is the game client was missing.");

	bool is_client_real = *SimulationDomain::is_client;
	if (!is_client_real) {
		ERR_FAIL_MSG("Domains cannot be activated or deactivated on the server, as there is no screen for it to override in the first place.");
	}

	if (!is_inside_tree()) {
		ERR_FAIL_MSG("The SimulationDomain must be in the scene tree to be made active.");
	}

	Viewport *tree_root_viewport = get_tree()->get_root()->get_viewport();
	ERR_FAIL_NULL(tree_root_viewport);

	if (domain_world_2d.is_null()) {
		WARN_PRINT("2D World was deleted from a SimulationDomain.");
		domain_world_2d.instantiate();
	}
	if (domain_world_3d.is_null()) {
		WARN_PRINT("3D World was deleted from a SimulationDomain.");
		domain_world_3d.instantiate();
	}

	if (SimulationDomain::current) {
		SimulationDomain::current->deactivate();
	}
	
	parent_world_2d = tree_root_viewport->get_world_2d();
	parent_world_3d = tree_root_viewport->get_world_3d();
	last_parent = tree_root_viewport->get_instance_id();

	tree_root_viewport->set_world_2d(domain_world_2d);
	tree_root_viewport->set_world_3d(domain_world_3d);

	// Must be set after, mods to viewport make it reject setting the world when the current domain is valid.
	SimulationDomain::current = this;
}


void SimulationDomain::deactivate() {
	ERR_MAIN_THREAD_GUARD
	ERR_FAIL_COND_MSG(SimulationDomain::current != this, "This SimulationDomain is not the current active domain, and cannot be deactivated.");
	ERR_FAIL_COND_MSG(SimulationDomain::is_client == nullptr, "The boolean designating whether or not this is the game client was missing.");

	bool is_client_real = *SimulationDomain::is_client;
	if (!is_client_real) {
		ERR_FAIL_MSG("Domains cannot be activated or deactivated on the server, as there is no screen for it to override in the first place.");
	}

	// Must be first!
	SimulationDomain::current = nullptr;

	Viewport *original_viewport = ObjectDB::get_instance<Viewport>(last_parent);
	if (original_viewport) {
		if (original_viewport->get_world_2d() == domain_world_2d) {
			// Only restore if it's still set to this domain's world!!
			// In another case, the user may have illegally replaced it.
			// n.b. it is acceptable that the parent world may be null. It'll complain and fix it, but the warning it raises is perfect so we let it happen.
			original_viewport->set_world_2d(parent_world_2d);
		}
		if (original_viewport->get_world_3d() == domain_world_3d) {
			original_viewport->set_world_3d(parent_world_3d);
		}
	}
	parent_world_2d = nullptr;
	parent_world_3d = nullptr;
	last_parent = ObjectID();
}

void SimulationDomain::initialize() {
	if (domain_world_2d.is_null()) {
		domain_world_2d.instantiate();
	}
	if (domain_world_3d.is_null()) {
		domain_world_3d.instantiate();
	}
}

RID SimulationDomain::get_physics_space_2d() const {
	ERR_FAIL_NULL_V_MSG(domain_world_2d, RID(), TC_SIMULATION_DOMAIN_WORLD_2D_MISSING);
	return domain_world_2d->get_space();
}

RID SimulationDomain::get_physics_space_3d() const {
	ERR_FAIL_NULL_V_MSG(domain_world_3d, RID(), TC_SIMULATION_DOMAIN_WORLD_3D_MISSING);
	return domain_world_3d->get_space();
}

RID SimulationDomain::get_render_canvas() const {
	ERR_FAIL_NULL_V_MSG(domain_world_2d, RID(), TC_SIMULATION_DOMAIN_WORLD_2D_MISSING);
	return domain_world_2d->get_canvas();
}

RID SimulationDomain::get_render_scenario() const {
	ERR_FAIL_NULL_V_MSG(domain_world_3d, RID(), TC_SIMULATION_DOMAIN_WORLD_3D_MISSING);
	return domain_world_3d->get_scenario();
}

void SimulationDomain::set_world_2d(const Ref<World2D> &p_world_2d) {
	ERR_FAIL_MSG("set_world_2d is not supported on SimulationDomain. It always has its own world created ahead of time.");
}
Ref<World2D> SimulationDomain::get_world_2d() const {
	ERR_FAIL_NULL_V_MSG(domain_world_2d, Ref<World2D>(), TC_SIMULATION_DOMAIN_WORLD_2D_MISSING);
	return domain_world_2d;
}
Ref<World2D> SimulationDomain::find_world_2d() const {
	ERR_FAIL_NULL_V_MSG(domain_world_2d, Ref<World2D>(), TC_SIMULATION_DOMAIN_WORLD_2D_MISSING);
	return domain_world_2d;
}

void SimulationDomain::set_world_3d(const Ref<World3D> &p_world_3d) {
	ERR_FAIL_MSG("set_world_3d is not supported on SimulationDomain.");
}
Ref<World3D> SimulationDomain::get_world_3d() const {
	ERR_FAIL_NULL_V_MSG(domain_world_3d, Ref<World3D>(), TC_SIMULATION_DOMAIN_WORLD_3D_MISSING);
	return domain_world_3d;
}
Ref<World3D> SimulationDomain::find_world_3d() const {
	ERR_FAIL_NULL_V_MSG(domain_world_3d, Ref<World3D>(), TC_SIMULATION_DOMAIN_WORLD_3D_MISSING);
	return domain_world_3d;
}
void SimulationDomain::set_use_own_world_3d(bool p_use_own_world_3d) {
	ERR_FAIL_MSG("set_use_own_world_3d is not supported on SimulationDomain.");
}
bool SimulationDomain::is_using_own_world_3d() const {
	return false;
}

void SimulationDomain::_bind_methods() {
	ClassDB::bind_static_method("SimulationDomain", D_METHOD("set_is_client_ptr", "ptr"), &SimulationDomain::set_is_client_ptr);

	ClassDB::bind_method(D_METHOD("get_is_active"), &SimulationDomain::get_active);
	ClassDB::bind_method(D_METHOD("make_active"), &SimulationDomain::make_active);
	ClassDB::bind_method(D_METHOD("deactivate"), &SimulationDomain::deactivate);
	ClassDB::bind_method(D_METHOD("initialize"), &SimulationDomain::initialize);
	ClassDB::bind_method(D_METHOD("get_physics_space_2d"), &SimulationDomain::get_physics_space_2d);
	ClassDB::bind_method(D_METHOD("get_physics_space_3d"), &SimulationDomain::get_physics_space_3d);
	ClassDB::bind_method(D_METHOD("get_render_canvas"), &SimulationDomain::get_render_canvas);
	ClassDB::bind_method(D_METHOD("get_render_scenario"), &SimulationDomain::get_render_scenario);

	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::BOOL, "is_active"), "get_is_active");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "physics_space_2d"), "get_physics_space_2d");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "physics_space_3d"), "get_physics_space_3d");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "render_canvas"), "get_render_canvas");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "render_scenario"), "get_render_scenario");
}

SimulationDomain::SimulationDomain() { }

SimulationDomain::~SimulationDomain() {
	if (SimulationDomain::current == this) {
		deactivate();
	}
	parent_world_2d = nullptr;
	parent_world_3d = nullptr;
	domain_world_2d = nullptr;
	domain_world_3d = nullptr;
}

#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
