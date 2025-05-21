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

#include "core/config/project_settings.h"
#include "core/object/object.h"
#include "scene/3d/world_environment.h"
#include "scene/main/node.h"
#include "scene/main/viewport.h"
#include "scene/resources/3d/world_3d.h"
#include "scene/main/window.h"
/*
#include "scene/3d/camera_3d.h"
#include "scene/resources/camera_attributes.h"
#include "scene/resources/compositor.h"
#include "scene/resources/environment.h"
#include "servers/physics_server_3d.h"
#include "servers/rendering_server.h"
*/

// #define USE_WORLD_PTR_NOT_REF

#ifdef USE_WORLD_PTR_NOT_REF
#error "This causes the world pointer to be destroyed."
#define TC_PTR(type, name) type *name
#define TC_IS_VALID(a) (a)
#define TC_IS_NULL(a) (!a)
#define TC_INSTANTIATE(type, name) name = memnew(type)
#define TC_DELETE(a) memdelete(a)
#else
#define TC_PTR(type, name) Ref<type> name
#define TC_IS_VALID(a) (a.is_valid())
#define TC_IS_NULL(a) (a.is_null())
#define TC_INSTANTIATE(type, name) name.instantiate()
#define TC_DELETE(a) memdelete(a.ptr())
#endif

class Simulation3D;
class WorldEnvirionment;

class Simulation3D : public Viewport {
	friend class Viewport;
	GDCLASS(Simulation3D, Viewport);

	bool predeleted = false;
	bool is_locked = false;
	bool world_instance_and_marshals_destroyed = false;
	bool created_properly = false;
	bool malformed = false;
	TC_PTR(World3D, world);

	// Parameters: message, message length, context, context length, error code
	static void (*tc_crash)(const unsigned char *, int, const unsigned char *, int, int);

	// Parameters: Simulation3D address, returns true if deletion is OK, false if not.
	static bool (*tc_destroy_validator)(const int64_t);

	// Parameters: None
	static bool (*tc_should_take_main_viewport)(void);

	static bool declared;

protected:
	void _notification(int p_what);
	static void _tc_crash(const String &p_msg, const String &p_context, int p_tc_error_code);
	static bool _tc_destroy_validator(const Simulation3D *p_instance);
	static bool _tc_should_take_main_viewport();
	static void _bind_methods();
	virtual DisplayServer::WindowID get_window_id() const override;

public:
	static int64_t set_conservatory_callbacks(const int64_t p_crash, const int64_t p_destroy, const int64_t p_is_client);
	static Simulation3D *current;

	bool get_is_live() const;
	RID get_physics_space() const;
	RID get_render_scenario() const;
	void destroy();

	void set_world_3d(const Ref<World3D> &p_world_3d) override;
	Ref<World3D> get_world_3d() const override;
	Ref<World3D> find_world_3d() const override;
	void set_use_own_world_3d(bool p_use_own_world_3d) override;
	bool is_using_own_world_3d() const override;

	_FORCE_INLINE_ static const int64_t static_construct() {
		Simulation3D *instance = memnew(Simulation3D);
		return (int64_t)instance;
	}

	Simulation3D();
	~Simulation3D();
};

#endif // SIMULATION_3D_H
#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
