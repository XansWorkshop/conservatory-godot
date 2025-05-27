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

bool SimulationDomain::declared;
SimulationDomain* SimulationDomain::current;
void (*SimulationDomain::tc_crash)(const unsigned char *, int, const unsigned char *, int, int);
bool (*SimulationDomain::tc_destroy_validator)(const int64_t);
bool (*SimulationDomain::tc_should_take_main_viewport)(void);

void SimulationDomain::_tc_crash(const String &p_msg, const String &p_context, int p_tc_error_code) {
	if (tc_crash) {
		PackedByteArray msg = p_msg.to_utf8_buffer();
		PackedByteArray context = p_context.to_utf8_buffer();
		tc_crash(msg.ptr(), msg.size(), context.ptr(), context.size(), p_tc_error_code);
	} else {
		ERR_FAIL_MSG(p_msg.ptr());
	}
}

bool SimulationDomain::_tc_destroy_validator(const SimulationDomain *p_instance) {
	if (tc_destroy_validator) {
		return tc_destroy_validator((int64_t)p_instance);
	} else {
		_tc_crash("The destroy method was not previously supplied correctly.", "Attempting to verify the destruction of a SimulationDomain", 0x8005);
		return false;
	}
}

bool SimulationDomain::_tc_should_take_main_viewport() {
	if (tc_should_take_main_viewport) {
		return tc_should_take_main_viewport();
	} else {
		_tc_crash("The IsClient method was not previously supplied correctly.", "Attempting to determine the function of a SimulationDomain", 0x8005);
		return false;
	}
}

int64_t SimulationDomain::set_conservatory_callbacks(const int64_t p_crash, const int64_t p_destroy, const int64_t p_should_take_main_viewport) {
	if (declared) {
		_tc_crash("Invalid attempt to call SimulationDomain.SetConservatoryCallbacks more than once.", "Verifying the integrity of the simulation", 0x8005);
		return 0;
	}
	ERR_FAIL_COND_V(p_crash == 0, 0);
	ERR_FAIL_COND_V(p_destroy == 0, 0);
	ERR_FAIL_COND_V(p_should_take_main_viewport == 0, 0);

	// This is so fucked
	tc_crash = (void (*)(const unsigned char *, int, const unsigned char *, int, int))p_crash;
	tc_destroy_validator = (bool (*)(const int64_t))p_destroy;
	tc_should_take_main_viewport = (bool (*)(void))p_should_take_main_viewport;
	declared = true;
	size_t addr = (size_t)(&SimulationDomain::static_construct);
	return (int64_t)addr;
}

void SimulationDomain::_notification(int p_what) {
	// The notification order is: Predelete, Exit Tree, Unparented
	ERR_MAIN_THREAD_GUARD;
	switch (p_what) {
		case Node::NOTIFICATION_ENTER_TREE:
			if (!created_properly) {
				malformed = true;
				if (declared) {
					_tc_crash("This SimulationDomain was not created using the correct technique.", "Verifying correct construction procedure (in-engine).", 0x8005);
				} else {
					_tc_crash("Xan forgot to statically initialize the simulation class.", "Verifying correct construction procedure (in-engine).", 0x8005);
				}
			} else {
				Node *my_parent = get_parent();
				Viewport *parent_viewport = Node::cast_to<Viewport>(my_parent);
				if (!parent_viewport || (parent_viewport != get_tree()->get_root()->get_viewport())) {
					_tc_crash("SimulationDomain must be parented to the root viewport.", "Verifying correct construction procedure (in-engine).", 0x8005);
					return;
				}
				if (TC_IS_NULL(world2d)) {
					_tc_crash("The World2D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
				}
				if (TC_IS_NULL(world3d)) {
					_tc_crash("The World3D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
				}
				is_locked = true;

				if (_tc_should_take_main_viewport()) {
					// We know for a fact that it's in the root viewport already.
					// Also we don't need to call setworld methods on this (via base), as its built in version of this signal
					// (NOTIFICATION_ENTER_TREE) does all of the code exeuction that doing ^ would achieve.
					parent_viewport->set_world_2d(world2d);
					parent_viewport->set_world_3d(world3d);
					current = this;
				}
			}
			break;
		case Node::NOTIFICATION_PREDELETE:
			predeleted = true;
			if (!world_instance_and_marshals_destroyed) {
				if (current == this) {
					current = nullptr;
				}
				_tc_crash("Illegal attempt to destroy SimulationDomain using Free(), or before its WorldInstance was destroyed.", "Verifying correct disposal procedure (in-engine).", 0x8005);

				// This might still run due to how exiting works so we should just clean it up anyway.
				if (TC_IS_VALID(world2d)) {
					TC_DELETE(world2d);
				}
				if (TC_IS_VALID(world3d)) {
					TC_DELETE(world3d);
				}
				memdelete(this);
			}
			break;
		case Node::NOTIFICATION_EXIT_TREE:
			if (!predeleted) {
				_tc_crash("Attempt to unparent a SimulationDomain. It cannot be removed from the scene tree; it MUST be deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
			}
			break;
		case Node::NOTIFICATION_INTERNAL_PROCESS:
			bool fail = TC_IS_NULL(world3d) || TC_IS_NULL(world2d);
			if (!fail) {
				fail = TC_IS_NULL(world3d->get_scenario()) || TC_IS_NULL(world3d->get_space())
					|| TC_IS_NULL(world2d->get_canvas()) || TC_IS_NULL(world2d->get_space());
			}
			if (fail && get_is_valid()) {
				_tc_crash("The World2D or World3D of a SimulationDomain was unexpectedly deleted, or the space/scenario/canvas of one or more of these objects was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
			}
			break;
	}
}

bool SimulationDomain::get_is_valid() const {
	if (_is_queued_for_deletion) {
		return false;
	}
	if (world_instance_and_marshals_destroyed) {
		return false;
	}
	return is_locked;
}

bool SimulationDomain::get_is_active() const {
	return SimulationDomain::current == this;
}

RID SimulationDomain::get_physics_space_2d() const {
	if (TC_IS_NULL(world2d)) {
		_tc_crash("The World2D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return RID();
	}
	return world2d->get_space();
}

RID SimulationDomain::get_physics_space_3d() const {
	if (TC_IS_NULL(world3d)) {
		_tc_crash("The World3D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return RID();
	}
	return world3d->get_space();
}

RID SimulationDomain::get_render_canvas() const {
	if (TC_IS_NULL(world2d)) {
		_tc_crash("The World2D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return RID();
	}
	return world2d->get_canvas();
}

RID SimulationDomain::get_render_scenario() const {
	if (TC_IS_NULL(world3d)) {
		_tc_crash("The World3D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return RID();
	}
	return world3d->get_scenario();
}

void SimulationDomain::destroy() {
	world_instance_and_marshals_destroyed = _tc_destroy_validator(this);
	if (world_instance_and_marshals_destroyed) {
		if (current == this) {
			current = nullptr;
		}
		if (TC_IS_VALID(world2d)) {
			TC_DELETE(world2d);
		}
		if (TC_IS_VALID(world3d)) {
			TC_DELETE(world3d);
		}
		memdelete(this);
	}
}
void SimulationDomain::set_world_2d(const Ref<World2D> &p_world_2d) {
	ERR_FAIL_MSG("set_world_2d is not supported on SimulationDomain.");
}
Ref<World2D> SimulationDomain::get_world_2d() const {
	if (TC_IS_NULL(world2d)) {
		_tc_crash("The World2D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return Ref<World2D>();
	}
	return world2d;
}
Ref<World2D> SimulationDomain::find_world_2d() const {
	if (TC_IS_NULL(world2d)) {
		_tc_crash("The World2D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return Ref<World2D>();
	}
	return world2d;
}

/*
DisplayServer::WindowID SimulationDomain::get_window_id() const {
	ERR_READ_THREAD_GUARD_V(DisplayServer::INVALID_WINDOW_ID);
	return DisplayServer::INVALID_WINDOW_ID;
}
*/

void SimulationDomain::set_world_3d(const Ref<World3D> &p_world_3d) {
	ERR_FAIL_MSG("set_world_3d is not supported on SimulationDomain.");
}
Ref<World3D> SimulationDomain::get_world_3d() const {
	if (TC_IS_NULL(world3d)) {
		_tc_crash("The World3D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return Ref<World3D>();
	}
	return world3d;
}
Ref<World3D> SimulationDomain::find_world_3d() const {
	if (TC_IS_NULL(world3d)) {
		_tc_crash("The World3D of a SimulationDomain was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return Ref<World3D>();
	}
	return world3d;
}
void SimulationDomain::set_use_own_world_3d(bool p_use_own_world_3d) {
	ERR_FAIL_MSG("set_use_own_world_3d is not supported on SimulationDomain.");
}
bool SimulationDomain::is_using_own_world_3d() const {
	return true;
}

void SimulationDomain::_bind_methods() {
	ClassDB::bind_static_method("SimulationDomain", D_METHOD("set_conservatory_callbacks", "crash", "destroy_callback", "should_take_over_main_viewport"), &SimulationDomain::set_conservatory_callbacks);
	ClassDB::bind_method(D_METHOD("get_is_valid"), &SimulationDomain::get_is_valid);
	ClassDB::bind_method(D_METHOD("get_is_active"), &SimulationDomain::get_is_active);
	ClassDB::bind_method(D_METHOD("get_physics_space_2d"), &SimulationDomain::get_physics_space_2d);
	ClassDB::bind_method(D_METHOD("get_physics_space_3d"), &SimulationDomain::get_physics_space_3d);
	ClassDB::bind_method(D_METHOD("get_render_canvas"), &SimulationDomain::get_render_canvas);
	ClassDB::bind_method(D_METHOD("get_render_scenario"), &SimulationDomain::get_render_scenario);
	ClassDB::bind_method(D_METHOD("destroy"), &SimulationDomain::destroy);

	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::BOOL, "is_valid"), "get_is_valid");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::BOOL, "is_active"), "get_is_active");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "physics_space_2d"), "get_physics_space_2d");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "physics_space_3d"), "get_physics_space_3d");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "render_canvas"), "get_render_canvas");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "render_scenario"), "get_render_scenario");
}

SimulationDomain::SimulationDomain() {
	created_properly = declared;
	TC_INSTANTIATE(World2D, world2d);
	TC_INSTANTIATE(World3D, world3d);
	set_process_internal(true);
}
SimulationDomain::~SimulationDomain() {
	if (current == this) {
		current = nullptr;
	}
}

#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
