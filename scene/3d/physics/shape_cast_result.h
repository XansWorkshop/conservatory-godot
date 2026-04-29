/**************************************************************************/
/*  shape_cast_result.h                                                   */
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

#ifndef PHYSICS_3D_DISABLED

#pragma once

#include "core/typedefs.h"
#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/object/object_id.h"
#include "core/templates/rid.h"
#include "core/math/vector3.h"
#include "core/variant/binder_common.h"
#include "core/config/project_settings.h"

#include "thirdparty/xanstools/xanstools.h"

class ShapeCastResult : public RefCounted {
	GDCLASS(ShapeCastResult, RefCounted);

private:
	ObjectID hit_object_id;
	Object *hit_object = nullptr;

protected:
	static void _bind_methods();

public:
	XT_AUTO_PROPERTY_INLINE_C(bool, success);
	XT_AUTO_PROPERTY_INLINE_C(Vector3, intersection_point);
	XT_AUTO_PROPERTY_INLINE_C(Vector3, intersection_normal);
	XT_AUTO_PROPERTY_INLINE_C(RID, rid);
	XT_AUTO_PROPERTY_INLINE_C(Vector3, linear_velocity_at_contact);
	XT_AUTO_PROPERTY_INLINE_C(real_t, collision_safe_fraction);
	XT_AUTO_PROPERTY_INLINE_C(real_t, collision_unsafe_fraction);
	XT_AUTO_PROPERTY_INLINE_C(int, shape_index);

	ObjectID _get_hit_object_id() const;
	int64_t get_hit_object_id() const;
	void _set_hit_object_id(const ObjectID &p_id);
	void set_hit_object_id_and_instance(const int64_t p_id);

	Object *get_hit_object() const;
	void _set_hit_object(const Object *p_collider);

	void clear();
	void copy_to(const Ref<ShapeCastResult> &p_destination) const;

	ShapeCastResult();
	/*
private:
	bool success;
	Vector3 point;
	Vector3 normal;
	RID rid;
	ObjectID hit_object_id;
	Object *hit_object = nullptr;
	int shape_index = 0;
	Vector3 linear_velocity_at_contact;
	real_t collision_safe_fraction = 0;
	real_t collision_unsafe_fraction = 0;

protected:
	static void _bind_methods();

public:
	bool get_success() const;
	void set_success(bool p_success);

	Vector3 get_intersection_point() const;
	void set_intersection_point(const Vector3 &p_point);

	Vector3 get_intersection_normal() const;
	void set_intersection_normal(const Vector3 &p_normal);

	RID get_rid() const;
	void set_rid(const RID &p_rid);

	ObjectID _get_hit_object_id() const;
	int64_t get_hit_object_id() const;
	void _set_hit_object_id(const ObjectID &p_id);
	void set_hit_object_id_and_instance(const int64_t p_id);

	Object *get_hit_object() const;
	void _set_hit_object(const Object *p_collider);

	int get_shape_index() const;
	void set_shape_index(int p_shape_index);

	Vector3 get_linear_velocity_at_contact() const;
	void set_linear_velocity_at_contact(const Vector3 &p_velocity);

	void set_collision_safe_fraction(real_t p_value);
	real_t get_collision_safe_fraction() const;

	void set_collision_unsafe_fraction(real_t p_value);
	real_t get_collision_unsafe_fraction() const;

	void clear();
	void copy_to(const Ref<ShapeCastResult> &p_destination) const;

	ShapeCastResult();
	*/
};

#endif // PHYSICS_3D_DISABLED
