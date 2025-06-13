/**************************************************************************/
/*  ray_cast_result.h                                                     */
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

#ifndef PHYSICS_3D_DISABLED

#ifndef RAY_CAST_RESULT_H
#define RAY_CAST_RESULT_H

#include "core/typedefs.h"
#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/object/object_id.h"
#include "core/templates/rid.h"
#include "core/math/vector3.h"
#include "core/variant/binder_common.h"
#include "core/config/project_settings.h"

#define JOLT_ALLOWS_RAYCAST_FACE_INDEX (GLOBAL_GET("physics/jolt_physics_3d/queries/enable_ray_cast_face_index").booleanize())
#define IS_USING_JOLT ((String)GLOBAL_GET("physics/3d/physics_engine") == "Jolt Physics")

class RayCastResult : public RefCounted {
	GDCLASS(RayCastResult, RefCounted);

public:
	enum PhysicsObjectType {
		INVALID,
		AREA,
		BODY,
		SOFT_BODY
	};

	// This struct is shared by The Conservatory.
	struct RayCastResultStruct {
		bool success;
		Vector3 position;
		Vector3 normal;
		RID rid;
		uint64_t hit_object_id;
		int32_t shape;
		int32_t face_index;
		PhysicsObjectType type;
	};

private:
	bool success = false;
	Vector3 position;
	Vector3 normal;
	RID rid;
	ObjectID collider_id;
	Object *collider = nullptr;
	int shape = -1;
	int face_index = -1;
	PhysicsObjectType type = INVALID;

	static bool can_index_face() {
		if (IS_USING_JOLT) {
			return JOLT_ALLOWS_RAYCAST_FACE_INDEX;
		}
		return true;
	}

protected:
	static void _bind_methods();

public:

	Vector3 get_hit_position() const;
	void set_hit_position(const Vector3 &p_position);

	Vector3 get_hit_normal() const;
	void set_hit_normal(const Vector3 &p_normal);

	RID get_rid() const;
	void set_rid(const RID &p_rid);

	ObjectID _get_hit_object_id() const;
	int64_t get_hit_object_id() const;
	void _set_hit_object_id(const ObjectID &p_id);
	void set_hit_object_id_and_instance(const int64_t p_id);

	Object *get_hit_object() const;
	void set_hit_object(const Object *p_collider);

	PhysicsObjectType get_collider_type() const;
	void set_collider_type(PhysicsObjectType p_type);

	int get_shape_index() const;
	void set_shape_index(int p_shape);

	int get_face_index() const;
	void set_face_index(int p_face_index);

	bool get_success() const;
	void set_success(bool p_success);

	void clear();

	void copy_to(const Ref<RayCastResult> &p_destination) const;
	// void copy_to_unsafe(const int64_t p_ptr) const;

	RayCastResult();
};

VARIANT_ENUM_CAST(RayCastResult::PhysicsObjectType);

#undef JOLT_ALLOWS_RAYCAST_FACE_INDEX
#undef IS_USING_JOLT

#endif // RAY_CAST_RESULT_H
#endif // PHYSICS_3D_DISABLED
