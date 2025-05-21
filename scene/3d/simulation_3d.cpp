/**************************************************************************/
/*  simulation_3d.cpp                                                     */
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

#if !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
#include "simulation_3d.h"

bool Simulation3D::declared;
Simulation3D* Simulation3D::current;
void (*Simulation3D::tc_crash)(const unsigned char *, int, const unsigned char *, int, int);
bool (*Simulation3D::tc_destroy_validator)(const int64_t);
bool (*Simulation3D::tc_should_take_main_viewport)(void);

void Simulation3D::_tc_crash(const String &p_msg, const String &p_context, int p_tc_error_code) {
	if (tc_crash) {
		PackedByteArray msg = p_msg.to_utf8_buffer();
		PackedByteArray context = p_context.to_utf8_buffer();
		tc_crash(msg.ptr(), msg.size(), context.ptr(), context.size(), p_tc_error_code);
	} else {
		ERR_FAIL_MSG(p_msg.ptr());
	}
}

bool Simulation3D::_tc_destroy_validator(const Simulation3D *p_instance) {
	if (tc_destroy_validator) {
		return tc_destroy_validator((int64_t)p_instance);
	} else {
		_tc_crash("The destroy method was not previously supplied correctly.", "Attempting to verify the destruction of a Simulation3D", 0x8005);
		return false;
	}
}

bool Simulation3D::_tc_should_take_main_viewport() {
	if (tc_should_take_main_viewport) {
		return tc_should_take_main_viewport();
	} else {
		_tc_crash("The IsClient method was not previously supplied correctly.", "Attempting to determine the function of a Simulation3D", 0x8005);
		return false;
	}
}

int64_t Simulation3D::set_conservatory_callbacks(const int64_t p_crash, const int64_t p_destroy, const int64_t p_should_take_main_viewport) {
	if (declared) {
		_tc_crash("Invalid attempt to call Simulation3D.SetConservatoryCallbacks more than once.", "Verifying the integrity of the simulation", 0x8005);
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
	size_t addr = (size_t)(&Simulation3D::static_construct);
	return (int64_t)addr;
}

void Simulation3D::_notification(int p_what) {
	// The notification order is: Predelete, Exit Tree, Unparented
	ERR_MAIN_THREAD_GUARD;
	switch (p_what) {
		case Node::NOTIFICATION_ENTER_TREE:
			if (!created_properly) {
				malformed = true;
				if (declared) {
					_tc_crash("This Simulation3D was not created using the correct technique.", "Verifying correct construction procedure (in-engine).", 0x8005);
				} else {
					_tc_crash("Xan forgot to statically initialize the simulation class.", "Verifying correct construction procedure (in-engine).", 0x8005);
				}
			} else {
				Node *my_parent = get_parent();
				Viewport *parent_viewport = Node::cast_to<Viewport>(my_parent);
				if (!parent_viewport || (parent_viewport != get_tree()->get_root()->get_viewport())) {
					_tc_crash("Simulation3D must be parented to the root viewport.", "Verifying correct construction procedure (in-engine).", 0x8005);
					return;
				}
				if (TC_IS_NULL(world)) {
					_tc_crash("The World3D of a Simulation3D was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
				}
				is_locked = true;

				// Future Xan:
				// What happens here is that I set the world by calling the base method, then
				// the game errors (safely) because its existing world's scenario isn't valid in
				// the rendering server.
				// I *assume* this is because the world is not yet used in my test scenario, thus
				// it gets disposed of? I'm not sure. Regardless, this *should* fix it:
				RS::get_singleton()->viewport_set_scenario(parent_viewport->get_viewport_rid(), RID());

				// Then we just carry on like normal.
				parent_viewport->set_world_3d(world);
				Viewport::set_world_3d(world);
				if (_tc_should_take_main_viewport()) {
					// We know for a fact that it's the root viewport already.
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
				_tc_crash("Illegal attempt to destroy Simulation3D using Free(), or before its WorldInstance was destroyed.", "Verifying correct disposal procedure (in-engine).", 0x8005);

				// This might still run due to how exiting works so we should just clean it up anyway.
				if (TC_IS_VALID(world)) {
					TC_DELETE(world);
				}
				memdelete(this);
			}
			break;
		case Node::NOTIFICATION_EXIT_TREE:
			if (!predeleted) {
				_tc_crash("Attempt to unparent a Simulation3D. It cannot be removed from the scene tree via RemoveChild(); it MUST be deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
			}
			break;
		case Node::NOTIFICATION_INTERNAL_PROCESS:
			if (TC_IS_NULL(world) && get_is_live()) {
				_tc_crash("The World3D of a Simulation3D was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
			}
			break;
	}
}

bool Simulation3D::get_is_live() const {
	if (_is_queued_for_deletion) {
		return false;
	}
	if (world_instance_and_marshals_destroyed) {
		return false;
	}
	return is_locked;
}

RID Simulation3D::get_physics_space() const {
	if (TC_IS_NULL(world)) {
		_tc_crash("The World3D of a Simulation3D was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return RID();
	}
	return world->get_space();
}

RID Simulation3D::get_render_scenario() const {
	if (TC_IS_NULL(world)) {
		_tc_crash("The World3D of a Simulation3D was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return RID();
	}
	return world->get_scenario();
}

void Simulation3D::destroy() {
	world_instance_and_marshals_destroyed = _tc_destroy_validator(this);
	if (world_instance_and_marshals_destroyed) {
		if (current == this) {
			current = nullptr;
		}
		if (TC_IS_VALID(world)) {
			TC_DELETE(world);
		}
		memdelete(this);
	}
}

DisplayServer::WindowID Simulation3D::get_window_id() const {
	ERR_READ_THREAD_GUARD_V(DisplayServer::INVALID_WINDOW_ID);
	return DisplayServer::INVALID_WINDOW_ID;
}

void Simulation3D::set_world_3d(const Ref<World3D>& p_world_3d) {
	if (!world_instance_and_marshals_destroyed) {
		ERR_FAIL_MSG("set_world_3d is not supported on Simulation3D.");
	}
	Viewport::set_world_3d(p_world_3d);
}
Ref<World3D> Simulation3D::get_world_3d() const {
	if (TC_IS_NULL(world)) {
		_tc_crash("The World3D of a Simulation3D was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return Ref<World3D>();
	}
	return world;
}
Ref<World3D> Simulation3D::find_world_3d() const {
	if (TC_IS_NULL(world)) {
		_tc_crash("The World3D of a Simulation3D was unexpectedly deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
		return Ref<World3D>();
	}
	return world;
}
void Simulation3D::set_use_own_world_3d(bool p_use_own_world_3d) {
	ERR_FAIL_MSG("set_use_own_world_3d is not supported on Simulation3D.");
}
bool Simulation3D::is_using_own_world_3d() const {
	return true;
}

void Simulation3D::_bind_methods() {
	ClassDB::bind_static_method("Simulation3D", D_METHOD("set_conservatory_callbacks", "crash", "can_destroy", "should_take_over_main_viewport"), &Simulation3D::set_conservatory_callbacks);
	ClassDB::bind_method(D_METHOD("get_is_live"), &Simulation3D::get_is_live);
	ClassDB::bind_method(D_METHOD("get_physics_space"), &Simulation3D::get_physics_space);
	ClassDB::bind_method(D_METHOD("get_render_scenario"), &Simulation3D::get_render_scenario);
	ClassDB::bind_method(D_METHOD("destroy"), &Simulation3D::destroy);

	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::BOOL, "is_live"), "get_is_live");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "physics_space"), "get_physics_space");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "render_scenario"), "get_render_scenario");
}

Simulation3D::Simulation3D() {
	created_properly = declared;
	TC_INSTANTIATE(World3D, world);
}
Simulation3D::~Simulation3D() {
	if (current == this) {
		current = nullptr;
	}
}

#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
