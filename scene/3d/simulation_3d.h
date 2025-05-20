/**************************************************************************/
/*  simulation_3d.h                                                       */
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
#ifndef SIMULATION_3D_H
#define SIMULATION_3D_H

#define ALLOW_GETTING_LAST_SUBMITTED_PARAMETERS

#include "core/config/project_settings.h"
#include "core/object/callable_method_pointer.h"
#include "core/object/object.h"
#include "scene/3d/world_environment.h"
#include "scene/main/node.h"
#include "scene/main/viewport.h"
#include "scene/resources/camera_attributes.h"
#include "scene/resources/compositor.h"
#include "scene/resources/environment.h"
#include "servers/physics_server_3d.h"
#include "servers/rendering_server.h"

class Simulation3D;

class Simulation3D : public Node {
	GDCLASS(Simulation3D, Node);

	bool predeleted = false;
	bool is_locked = false;
	bool world_instance_and_marshals_destroyed = false;
	bool created_properly = false;
	bool malformed = false;
	RID space = RID();
	RID scenario = RID();

	Ref<Environment> environment;
	Ref<CameraAttributes> camera_attributes;
	Ref<Compositor> compositor;

#ifdef ALLOW_GETTING_LAST_SUBMITTED_PARAMETERS
	bool environment_was_declared = false;
	bool camera_attributes_was_declared = false;
	bool compositor_was_declared = false;
#endif

	float gravity = -9.806f;

	// Parameters: message, message length, context, context length, error code
	static void (*tc_crash)(const unsigned char *, int, const unsigned char *, int, int);

	// Parameters: Simulation3D address, returns true if deletion is OK, false if not.
	static bool (*tc_destroy_validator)(const int64_t);

	static bool declared;

protected:
	void _notification(int p_what);
	static void _tc_crash(const String &p_msg, const String &p_context, int p_tc_error_code);
	static bool _tc_destroy_validator(const Simulation3D *p_instance);
	static void _bind_methods();

public:
	static int64_t set_conservatory_callbacks(const int64_t &p_crash, const int64_t &p_destroy);

	bool get_is_live() const;
	RID get_physics_space() const;
	RID get_render_scenario() const;
	float get_gravity() const;
	void set_gravity(const float p_gravity);
	void destroy();

	void init_environment(const Ref<Environment> &p_environment);
	void init_camera_attributes(const Ref<CameraAttributes> &p_camera_attributes);
	void init_compositor(const Ref<Compositor> &p_compositor);

#ifdef ALLOW_GETTING_LAST_SUBMITTED_PARAMETERS
	Ref<Environment> get_environment() const;
	Ref<CameraAttributes> get_camera_attributes() const;
	Ref<Compositor> get_compositor() const;
#endif

	_FORCE_INLINE_ static const int64_t static_construct() {
		Simulation3D *instance = memnew(Simulation3D);
		instance->created_properly = declared;
		return (int64_t)instance;
	}

	Simulation3D();
	~Simulation3D();
};

#ifdef ALLOW_GETTING_LAST_SUBMITTED_PARAMETERS
#define TC_ASSIGN_PARAMETER(destination)                       \
	if (p_##destination.is_valid()) {                          \
		destination = p_##destination.ptr()->duplicate(false); \
		destination##_was_declared = true;                     \
	} else {                                                   \
		destination = nullptr;                                 \
		destination##_was_declared = false;                    \
	}
#else
#define TC_ASSIGN_PARAMETER(destination)                       \
	if (p_##destination.is_valid()) {                          \
		destination = p_##destination.ptr()->duplicate(false); \
	} else {                                                   \
		destination = nullptr;                                 \
	}
#endif

#define TC_GET_RID(param) param.is_valid() ? param.ptr()->get_rid() : RID()

#define VALIDATE_PARAMETER(param, is_live)                                                                                                        \
	if (is_live && param##_was_declared && param.is_null()) {                                                                                     \
		_tc_crash(_STR(param) " was freed while its Simulation3D was still active.", "Verifying integrity of Simulation3D (in-engine).", 0x8005); \
		return;																																	  \
	}

#define TC_CND_FREE_RID(server, rid)        \
	if (rid.is_valid()) {                   \
		server::get_singleton()->free(rid); \
	}										\
	rid = RID()

#endif // SIMULATION_3D_H
#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
