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

int64_t Simulation3D::set_conservatory_callbacks(const int64_t &p_crash, const int64_t &p_destroy) {
	if (declared) {
		_tc_crash("Invalid attempt to call Simulation3D.SetConservatoryCallbacks more than once.", "Verifying the integrity of the simulation", 0x8005);
		return 0;
	}
	ERR_FAIL_COND_V(p_crash == 0, 0);
	ERR_FAIL_COND_V(p_destroy == 0, 0);
	//ERR_FAIL_COND_V(p_crash.get_argument_count() != 3, 0);
	//ERR_FAIL_COND_V(p_destroy.get_argument_count() != 1, 0);

	// This is so fucked
	tc_crash = (void(*)(const unsigned char*, int, const unsigned char*, int, int))p_crash;
	tc_destroy_validator = (bool(*)(const int64_t))p_destroy;
	declared = true;
	size_t addr = (size_t)(&Simulation3D::static_construct);
	return (int64_t)addr;
}

void Simulation3D::_notification(int p_what) {
	// The notification order is: Predelete, Exit Tree, Unparented
	switch (p_what) {
		case Node::NOTIFICATION_ENTER_TREE:
			if (!created_properly) {
				malformed = true;
				_tc_crash("This Simulation3D was not created using the correct technique.", "Verifying correct construction procedure (in-engine).", 0x8005);
			} else {
				space = PhysicsServer3D::get_singleton()->space_create();
				PhysicsServer3D::get_singleton()->space_set_active(space, true);
				PhysicsServer3D::get_singleton()->area_set_param(space, PhysicsServer3D::AREA_PARAM_GRAVITY, gravity);
				PhysicsServer3D::get_singleton()->area_set_param(space, PhysicsServer3D::AREA_PARAM_GRAVITY_VECTOR, new Vector3(0.0f, -1.0f, 0.0f));
				PhysicsServer3D::get_singleton()->area_set_param(space, PhysicsServer3D::AREA_PARAM_LINEAR_DAMP, GLOBAL_GET("physics/3d/default_linear_damp"));
				PhysicsServer3D::get_singleton()->area_set_param(space, PhysicsServer3D::AREA_PARAM_ANGULAR_DAMP, GLOBAL_GET("physics/3d/default_angular_damp"));

				scenario = RenderingServer::get_singleton()->scenario_create();
			}
			break;
		case Node::NOTIFICATION_PREDELETE:
			predeleted = true;
			if (!world_instance_and_marshals_destroyed) {
				_tc_crash("Illegal attempt to destroy Simulation3D using Free(), or before its WorldInstance was destroyed..", "Verifying correct disposal procedure (in-engine).", 0x8005);
			}
			break;
		case Node::NOTIFICATION_EXIT_TREE:
			if (!predeleted) {
				_tc_crash("Attempt to unparent a Simulation3D. It cannot be removed from the scene tree via RemoveChild(); it MUST be deleted.", "Verifying correct disposal procedure (in-engine).", 0x8005);
			}
			if (space.is_valid()) {
				PhysicsServer3D::get_singleton()->free(space);
				space = RID();
			}
			if (scenario.is_valid()) {
				RenderingServer::get_singleton()->free(scenario);
				scenario = RID();
			}
			break;
		//case Node::NOTIFICATION_UNPARENTED:
			//break;
	}
}

bool Simulation3D::get_is_live() const {
	if (_is_queued_for_deletion) return false;
	if (world_instance_and_marshals_destroyed) return false;
	return space.is_valid() && scenario.is_valid();
}

RID Simulation3D::get_physics_space() const {
	if (!space.is_valid()) {
		_tc_crash("The physics space was not created, or was freed.", "Verifying integrity of Simulation3D (in-engine).", 0x8005);
	}
	return space;
}
RID Simulation3D::get_render_scenario() const {
	if (!scenario.is_valid()) {
		_tc_crash("The render scenario was not created, or was freed.", "Verifying integrity of Simulation3D (in-engine).", 0x8005);
	}
	return scenario;
}

float Simulation3D::get_gravity() const {
	return gravity;
}

void Simulation3D::set_gravity(float p_gravity) {
	if (space.is_valid()) {
		_tc_crash("The physics space has already been created. This must be set on initialization.", "Verifying integrity of Simulation3D (in-engine).", 0x8005);
	}
	gravity = p_gravity;
}

void Simulation3D::destroy() {
	world_instance_and_marshals_destroyed = _tc_destroy_validator(this);
	if (world_instance_and_marshals_destroyed) {
		//this->queue_free();
		memdelete(this);
	}
}

void Simulation3D::_bind_methods() {
	ClassDB::bind_static_method("Simulation3D", D_METHOD("set_conservatory_callbacks", "crash", "destroy"), &Simulation3D::set_conservatory_callbacks);
	ClassDB::bind_method(D_METHOD("get_is_live"), &Simulation3D::get_is_live);
	ClassDB::bind_method(D_METHOD("get_physics_space"), &Simulation3D::get_physics_space);
	ClassDB::bind_method(D_METHOD("get_render_scenario"), &Simulation3D::get_render_scenario);
	ClassDB::bind_method(D_METHOD("destroy"), &Simulation3D::destroy);
	ClassDB::bind_method(D_METHOD("set_gravity"), &Simulation3D::set_gravity);
	ClassDB::bind_method(D_METHOD("get_gravity"), &Simulation3D::get_gravity);

	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::BOOL, "is_live"), "get_is_live");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "physics_space"), "get_physics_space");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::Type::RID, "render_scenario"), "get_render_scenario");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::Type::FLOAT, "gravity"), "set_gravity", "get_gravity");
}

Simulation3D::Simulation3D() { }
Simulation3D::~Simulation3D() { }

#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
