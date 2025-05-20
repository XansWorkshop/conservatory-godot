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

#ifndef PHYSICS_3D_DISABLED

#include "ray_cast_result.h"

Vector3 RayCastResult::get_hit_position() const {
	return position;
}
Vector3 RayCastResult::get_hit_normal() const {
	return normal;
}
RID RayCastResult::get_rid() const {
	return rid;
}
ObjectID RayCastResult::_get_hit_object_id() const {
	return collider_id;
}
int64_t RayCastResult::get_hit_object_id() const {
	return (int64_t)collider_id;
}
Object *RayCastResult::get_hit_object() const {
	return collider;
}
RayCastResult::PhysicsObjectType RayCastResult::get_collider_type() const {
	return type;
}
int RayCastResult::get_shape_index() const {
	return shape;
}
int RayCastResult::get_face_index() const {
	if (face_index < 0 && success) {
		if (!can_index_face()) {
			ERR_FAIL_V_MSG(-1, "The physics engine is set to Jolt Physics, and the current configuration skips looking for which face got hit; get_face_index() is not useful at this time.");
		}
	}
	return face_index;
}
bool RayCastResult::get_success() const {
	return success;
}

void RayCastResult::set_hit_position(const Vector3 &p_position) {
	position = p_position;
}
void RayCastResult::set_hit_normal(const Vector3 &p_normal) {
	normal = p_normal;
}
void RayCastResult::set_rid(const RID &p_rid) {
	rid = p_rid;
}
void RayCastResult::_set_hit_object_id(const ObjectID &p_id) {
	collider_id = p_id;
}
void RayCastResult::set_hit_object_id_and_instance(const int64_t p_id) {
	// The public version
	collider_id = ObjectID(p_id);
	if (!collider_id.is_null()) {
		collider = ObjectDB::get_instance(collider_id);
	} else {
		collider = nullptr;
	}
}
void RayCastResult::set_hit_object(const Object *p_collider) {
	collider = (Object*)p_collider;
}
void RayCastResult::set_collider_type(RayCastResult::PhysicsObjectType p_type) {
	type = p_type;
}
void RayCastResult::set_shape_index(int p_shape) {
	shape = p_shape;
}
void RayCastResult::set_face_index(int p_face_index) {
	face_index = p_face_index;
}
void RayCastResult::set_success(bool p_success) {
	success = p_success;
}

void RayCastResult::clear() {
	position = Vector3();
	normal = Vector3();
	rid = RID();
	collider_id = ObjectID();
	collider = nullptr;
	shape = -1;
	face_index = -1;
	type = INVALID;
	success = false;
}

void RayCastResult::copy_to(const Ref<RayCastResult> &p_destination) const {
	ERR_FAIL_COND_MSG(p_destination.is_null(), "The provided destination object is null.");
	RayCastResult *other = p_destination.ptr();
	other->position = position;
	other->normal = normal;
	other->rid = rid;
	other->collider_id = collider_id;
	other->collider = collider;
	other->shape = shape;
	other->face_index = face_index;
	other->type = type;
	other->success = success;
}

void RayCastResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_success"), &RayCastResult::get_success);
	ClassDB::bind_method(D_METHOD("get_hit_position"), &RayCastResult::get_hit_position);
	ClassDB::bind_method(D_METHOD("get_hit_normal"), &RayCastResult::get_hit_normal);
	ClassDB::bind_method(D_METHOD("get_rid"), &RayCastResult::get_rid);
	ClassDB::bind_method(D_METHOD("get_hit_object_id"), &RayCastResult::get_hit_object_id);
	ClassDB::bind_method(D_METHOD("get_hit_godot_object"), &RayCastResult::get_hit_object);
	ClassDB::bind_method(D_METHOD("get_collider_type"), &RayCastResult::get_collider_type);
	ClassDB::bind_method(D_METHOD("get_shape_index"), &RayCastResult::get_shape_index);
	ClassDB::bind_method(D_METHOD("get_face_index"), &RayCastResult::get_face_index);

	ClassDB::bind_method(D_METHOD("set_success"), &RayCastResult::set_success);
	ClassDB::bind_method(D_METHOD("set_hit_position"), &RayCastResult::set_hit_position);
	ClassDB::bind_method(D_METHOD("set_hit_normal"), &RayCastResult::set_hit_normal);
	ClassDB::bind_method(D_METHOD("set_rid"), &RayCastResult::set_rid);
	ClassDB::bind_method(D_METHOD("set_hit_object_id"), &RayCastResult::set_hit_object_id_and_instance);
	ClassDB::bind_method(D_METHOD("set_collider_type"), &RayCastResult::set_collider_type);
	ClassDB::bind_method(D_METHOD("set_shape_index"), &RayCastResult::set_shape_index);
	ClassDB::bind_method(D_METHOD("set_face_index"), &RayCastResult::set_face_index);

	ClassDB::bind_method(D_METHOD("clear"), &RayCastResult::clear);
	ClassDB::bind_method(D_METHOD("copy_to", "destination"), &RayCastResult::copy_to);

	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::BOOL, "success", PROPERTY_HINT_NONE), "set_success", "get_success");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::VECTOR3, "hit_position", PROPERTY_HINT_NONE, "suffix:m"), "set_hit_position", "get_hit_position");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::VECTOR3, "hit_normal", PROPERTY_HINT_NONE, "suffix:m"), "set_hit_normal", "get_hit_normal");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::RID, "hit_rid", PROPERTY_HINT_NONE), "set_rid", "get_rid");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_object_id", PROPERTY_HINT_NONE), "set_hit_object_id", "get_hit_object_id");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::OBJECT, "hit_godot_object", PROPERTY_HINT_NONE), "get_hit_godot_object");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_type", PROPERTY_HINT_ENUM, "invalid,area,body,soft_body"), "set_collider_type", "get_collider_type");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_shape_index", PROPERTY_HINT_NONE), "set_shape_index", "get_shape_index");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_face_index", PROPERTY_HINT_NONE), "set_face_index", "get_face_index");

	BIND_ENUM_CONSTANT(RayCastResult::PhysicsObjectType::INVALID);
	BIND_ENUM_CONSTANT(RayCastResult::PhysicsObjectType::AREA);
	BIND_ENUM_CONSTANT(RayCastResult::PhysicsObjectType::BODY);
	BIND_ENUM_CONSTANT(RayCastResult::PhysicsObjectType::SOFT_BODY);
}

RayCastResult::RayCastResult() {
}
#endif // PHYSICS_3D_DISABLED
