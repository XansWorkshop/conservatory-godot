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

#include "core/object/object.h"
#include "scene/main/node.h"
#include "scene/main/viewport.h"
#include "servers/physics_server_3d.h"
#include "servers/rendering_server.h"
#include "core/object/callable_method_pointer.h"
#include "core/config/project_settings.h"

class Simulation3D;

class Simulation3D : public Node {
	GDCLASS(Simulation3D, Node);

	bool predeleted = false;
	bool world_instance_and_marshals_destroyed = false;
	bool created_properly = false;
	bool malformed = false;
	RID space = RID();
	RID scenario = RID();

	float gravity = -9.816f;

	// Parameters: message, message length, context, context length, error code
	static void (*tc_crash)(const unsigned char*, int, const unsigned char*, int, int);

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
	void set_gravity(float p_gravity);
	void destroy();

	_FORCE_INLINE_ static const int64_t static_construct() {
		Simulation3D* instance = memnew(Simulation3D);
		instance->created_properly = true;
		return (int64_t)instance;
	}

	Simulation3D();
	~Simulation3D();
};

#endif // SIMULATION_3D_H
#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
