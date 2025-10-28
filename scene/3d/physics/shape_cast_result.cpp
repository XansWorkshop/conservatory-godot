/**************************************************************************/
/*  shape_cast_result.cpp                                                 */
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

#include "shape_cast_result.h"

bool ShapeCastResult::get_success() const {
	return success;
}

void ShapeCastResult::set_success(bool p_success) {
	success = p_success;
}

Vector3 ShapeCastResult::get_intersection_point() const {
	return point;
}
void ShapeCastResult::set_intersection_point(const Vector3 &p_point) {
	point = p_point;
}

Vector3 ShapeCastResult::get_intersection_normal() const {
	return normal;
}
void ShapeCastResult::set_intersection_normal(const Vector3 &p_normal) {
	normal = p_normal;
}

RID ShapeCastResult::get_rid() const {
	return rid;
}
void ShapeCastResult::set_rid(const RID& p_rid) {
	rid = p_rid;
}

ObjectID ShapeCastResult::_get_hit_object_id() const {
	return hit_object_id;
}
int64_t ShapeCastResult::get_hit_object_id() const {
	return (int64_t)hit_object_id;
}
void ShapeCastResult::_set_hit_object_id(const ObjectID& p_id) {
	hit_object_id = p_id;	
}
void ShapeCastResult::set_hit_object_id_and_instance(const int64_t p_id) {
	// The public version
	hit_object_id = ObjectID(p_id);
	if (!hit_object_id.is_null()) {
		hit_object = ObjectDB::get_instance(hit_object_id);
	} else {
		hit_object = nullptr;
	}
}

Object* ShapeCastResult::get_hit_object() const {
	return hit_object;
}
void ShapeCastResult::_set_hit_object(const Object *p_collider) {
	hit_object = (Object *)p_collider;
}

ShapeCastResult::PhysicsObjectType ShapeCastResult::get_collider_type() const {
	return type;
}
void ShapeCastResult::set_collider_type(ShapeCastResult::PhysicsObjectType p_type) {
	type = p_type;
}

int ShapeCastResult::get_shape_index() const {
	return shape_index;
}
void ShapeCastResult::set_shape_index(int p_shape_index) {
	shape_index = p_shape_index;
}

Vector3 ShapeCastResult::get_linear_velocity_at_contact() const {
	return linear_velocity_at_contact;
}
void ShapeCastResult::set_linear_velocity_at_contact(const Vector3& p_velocity) {
	linear_velocity_at_contact = p_velocity;
}

void ShapeCastResult::clear() {
	success = false;
	point = Vector3();
	normal = Vector3();
	rid = RID();
	type = INVALID;
	hit_object_id = ObjectID();
	hit_object = nullptr;
	shape_index = 0;
	linear_velocity_at_contact = Vector3();
}

void ShapeCastResult::copy_to(const Ref<ShapeCastResult>& p_destination) const {
	p_destination->success = success;
	p_destination->point = point;
	p_destination->normal = normal;
	p_destination->rid = rid;
	p_destination->type = type;
	p_destination->hit_object_id = hit_object_id;
	p_destination->hit_object = hit_object;
	p_destination->shape_index = shape_index;
	p_destination->linear_velocity_at_contact = linear_velocity_at_contact;
}

void ShapeCastResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_success"), &ShapeCastResult::get_success);
	ClassDB::bind_method(D_METHOD("get_intersection_point"), &ShapeCastResult::get_intersection_point);
	ClassDB::bind_method(D_METHOD("get_intersection_normal"), &ShapeCastResult::get_intersection_normal);
	ClassDB::bind_method(D_METHOD("get_rid"), &ShapeCastResult::get_rid);
	ClassDB::bind_method(D_METHOD("get_hit_object_id"), &ShapeCastResult::get_hit_object_id);
	ClassDB::bind_method(D_METHOD("get_hit_godot_object"), &ShapeCastResult::get_hit_object);
	ClassDB::bind_method(D_METHOD("get_collider_type"), &ShapeCastResult::get_collider_type);
	ClassDB::bind_method(D_METHOD("get_shape_index"), &ShapeCastResult::get_shape_index);
	ClassDB::bind_method(D_METHOD("get_linear_velocity_at_contact"), &ShapeCastResult::get_linear_velocity_at_contact);

	ClassDB::bind_method(D_METHOD("set_success", "success"), &ShapeCastResult::set_success);
	ClassDB::bind_method(D_METHOD("set_intersection_point", "point"), &ShapeCastResult::set_intersection_point);
	ClassDB::bind_method(D_METHOD("set_intersection_normal", "normal"), &ShapeCastResult::set_intersection_normal);
	ClassDB::bind_method(D_METHOD("set_rid", "rid"), &ShapeCastResult::set_rid);
	ClassDB::bind_method(D_METHOD("set_hit_object_id", "id"), &ShapeCastResult::set_hit_object_id_and_instance);
	ClassDB::bind_method(D_METHOD("set_collider_type", "type"), &ShapeCastResult::set_collider_type);
	ClassDB::bind_method(D_METHOD("set_shape_index", "shape_index"), &ShapeCastResult::set_shape_index);
	ClassDB::bind_method(D_METHOD("set_linear_velocity_at_contact", "velocity"), &ShapeCastResult::set_linear_velocity_at_contact);

	ClassDB::bind_method(D_METHOD("clear"), &ShapeCastResult::clear);
	ClassDB::bind_method(D_METHOD("copy_to", "destination"), &ShapeCastResult::copy_to);

	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::BOOL, "success", PROPERTY_HINT_NONE), "set_success", "get_success");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::VECTOR3, "intersection_point", PROPERTY_HINT_NONE, "suffix:m"), "set_intersection_point", "get_intersection_point");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::VECTOR3, "intersection_normal", PROPERTY_HINT_NONE, "suffix:m"), "set_intersection_normal", "get_intersection_normal");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::RID, "hit_rid", PROPERTY_HINT_NONE), "set_rid", "get_rid");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_object_id", PROPERTY_HINT_NONE), "set_hit_object_id", "get_hit_object_id");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::OBJECT, "hit_godot_object", PROPERTY_HINT_NONE), "get_hit_godot_object");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_type", PROPERTY_HINT_ENUM, "invalid,area,body,soft_body"), "set_collider_type", "get_collider_type");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_shape_index", PROPERTY_HINT_NONE), "set_shape_index", "get_shape_index");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::VECTOR3, "linear_velocity_at_contact", PROPERTY_HINT_NONE), "set_linear_velocity_at_contact", "get_linear_velocity_at_contact");

	BIND_ENUM_CONSTANT(INVALID);
	BIND_ENUM_CONSTANT(AREA);
	BIND_ENUM_CONSTANT(BODY);
	BIND_ENUM_CONSTANT(SOFT_BODY);
}

ShapeCastResult::ShapeCastResult() {
	clear();
}

void ShapeCastResultExtras::set_collision_safe_fraction(real_t p_value) {
	collision_safe_fraction = p_value;
}

real_t ShapeCastResultExtras::get_collision_safe_fraction() const {
	return collision_safe_fraction;
}

void ShapeCastResultExtras::set_collision_unsafe_fraction(real_t p_value) {
	collision_unsafe_fraction = p_value;
}

real_t ShapeCastResultExtras::get_collision_unsafe_fraction() const {
	return collision_unsafe_fraction;
}

void ShapeCastResultExtras::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_collision_safe_fraction", "value"), &ShapeCastResultExtras::set_collision_safe_fraction);
	ClassDB::bind_method(D_METHOD("get_collision_safe_fraction"), &ShapeCastResultExtras::get_collision_safe_fraction);
	ClassDB::bind_method(D_METHOD("set_collision_unsafe_fraction", "value"), &ShapeCastResultExtras::set_collision_unsafe_fraction);
	ClassDB::bind_method(D_METHOD("get_collision_unsafe_fraction"), &ShapeCastResultExtras::get_collision_unsafe_fraction);

	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::FLOAT, "collision_safe_fraction"), "set_collision_safe_fraction", "get_collision_safe_fraction");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::FLOAT, "collision_unsafe_fraction"), "set_collision_unsafe_fraction", "get_collision_unsafe_fraction");
}

#endif // PHYSICS_3D_DISABLED
