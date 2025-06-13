/**************************************************************************/
/*  ray_cast_3d_direct.cpp                                                */
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
#include "ray_cast_3d_direct.h"

void RayCast3DDirect::set_source_position(const Vector3 &p_point) {
	source_position = p_point;
}

Vector3 RayCast3DDirect::get_source_position() const {
	return source_position;
}

void RayCast3DDirect::set_target_position(const Vector3 &p_point) {
	target_position = p_point;
}

Vector3 RayCast3DDirect::get_target_position() const {
	return target_position;
}

void RayCast3DDirect::set_transform_and_distance(const Transform3D& p_transform, real_t p_length) {
	source_position = p_transform.origin;
	target_position = p_transform.xform(Vector3(0, 0, -p_length));
}

void RayCast3DDirect::set_collision_mask(uint32_t p_mask) {
	collision_mask = p_mask;
}

uint32_t RayCast3DDirect::get_collision_mask() const {
	return collision_mask;
}

void RayCast3DDirect::set_collision_mask_value(int p_layer_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 32, "Collision layer number must be between 1 and 32 inclusive.");
	uint32_t mask = get_collision_mask();
	if (p_value) {
		mask |= 1 << (p_layer_number - 1);
	} else {
		mask &= ~(1 << (p_layer_number - 1));
	}
	set_collision_mask(mask);
}

bool RayCast3DDirect::get_collision_mask_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Collision layer number must be between 1 and 32 inclusive.");
	return get_collision_mask() & (1 << (p_layer_number - 1));
}

bool RayCast3DDirect::get_hit_something() const {
	return collided;
}

Object *RayCast3DDirect::get_hit_object() const {
	if (against.is_null()) {
		return nullptr;
	}

	return ObjectDB::get_instance(against);
}

ObjectID RayCast3DDirect::get_hit_object_id() const {
	return against;
}

RID RayCast3DDirect::get_collider_rid() const {
	return against_rid;
}

RayCastResult::PhysicsObjectType RayCast3DDirect::get_collider_type() const {
	return type;
}

int RayCast3DDirect::get_collider_shape() const {
	return against_shape;
}

Vector3 RayCast3DDirect::get_collision_point() const {
	return collision_point;
}

Vector3 RayCast3DDirect::get_collision_normal() const {
	return collision_normal;
}

int RayCast3DDirect::get_collider_face_index() const {
	return collision_face_index;
}

bool RayCast3DDirect::cast(const RID &p_space) {
	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V_MSG(dss, false, "There is no direct space state associated with the provided space.");

	PhysicsDirectSpaceState3D::RayParameters ray_params;
	ray_params.from = source_position;
	ray_params.to = target_position;
	ray_params.exclude = exclude;
	ray_params.collision_mask = collision_mask;
	ray_params.collide_with_bodies = collide_with_bodies;
	ray_params.collide_with_areas = collide_with_areas;
	ray_params.hit_from_inside = hit_from_inside;
	ray_params.hit_back_faces = hit_back_faces;

	PhysicsDirectSpaceState3D::RayResult rr;
	if (dss->intersect_ray(ray_params, rr)) {
		collided = true;
		type = (RayCastResult::PhysicsObjectType)rr.type;

		collision_point = rr.position;
		collision_normal = rr.normal;
		collision_face_index = rr.face_index;

		against = rr.collider_id;
		against_rid = rr.rid;
		against_shape = rr.shape;
	} else {
		collided = false;
		type = RayCastResult::PhysicsObjectType::INVALID;

		collision_point = Vector3(0, 0, 0);
		collision_normal = Vector3(0, 0, 0);
		collision_face_index = -1;

		against = ObjectID();
		against_rid = RID();
		against_shape = -1;
	}

	return collided;
}

void RayCast3DDirect::store_in_result(const Ref<RayCastResult> &p_result) const {
	RayCastResult *castResult = p_result.ptr();
	castResult->set_hit_position(collision_point);
	castResult->set_hit_normal(collision_normal);
	castResult->set_rid(against_rid);
	castResult->set_hit_object_id_and_instance(against);
	castResult->set_collider_type(type);
	castResult->set_shape_index(against_shape);
	castResult->set_face_index(collision_face_index);
	castResult->set_success(collided);
}

void RayCast3DDirect::store_in_result_unsafe(const int64_t p_result) const {
	RayCastResult::RayCastResultStruct *castResult = (RayCastResult::RayCastResultStruct *)p_result;
	castResult->position = collision_point;
	castResult->normal = collision_normal;
	castResult->rid = against_rid;
	castResult->hit_object_id = against;
	castResult->type = type;
	castResult->shape = against_shape;
	castResult->face_index = collision_face_index;
	castResult->success = collided;
}

bool RayCast3DDirect::cast_statically(const RID &p_space, const Ref<PhysicsRayQueryParameters3D> &p_parameters, const Ref<RayCastResult> &p_result) {
	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V_MSG(dss, false, "There is no direct space state associated with the provided space.");
	ERR_FAIL_COND_V_MSG(p_parameters.is_null(), false, "The PhysicsRayQueryParameters3D instance is not valid.");
	ERR_FAIL_COND_V_MSG(p_result.is_null(), false, "The RayCastResult instance is not valid.");

	
	PhysicsDirectSpaceState3D::RayResult rr;

	if (dss->intersect_ray(p_parameters->get_parameters(), rr)) {
		RayCastResult *castResult = p_result.ptr();
		castResult->set_hit_position(rr.position);
		castResult->set_hit_normal(rr.normal);
		castResult->set_rid(rr.rid);
		castResult->_set_hit_object_id(rr.collider_id);
		castResult->set_hit_object(rr.collider);
		castResult->set_collider_type((RayCastResult::PhysicsObjectType)rr.type);
		castResult->set_shape_index(rr.shape);
		castResult->set_face_index(rr.face_index);
		castResult->set_success(true);
		return true;
	} else {
		RayCastResult *castResult = p_result.ptr();
		castResult->clear();
		return false;
	}
}

/*
bool RayCast3DDirect::cast_statically_unsafe(const RID &p_space, const Ref<PhysicsRayQueryParameters3D> &p_parameters, const int64_t p_result) {
	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V_MSG(dss, false, "There is no direct space state associated with the provided space.");
	ERR_FAIL_COND_V_MSG(p_parameters.is_null(), false, "The PhysicsRayQueryParameters3D instance is not valid.");
	ERR_FAIL_COND_V_MSG(p_result, false, "The pointer to the result struct is not valid.");

	PhysicsDirectSpaceState3D::RayResult rr;

	if (dss->intersect_ray(p_parameters->get_parameters(), rr)) {
		RayCastResult::RayCastResultStruct *castResult = (RayCastResult::RayCastResultStruct *)p_result;
		castResult->position = rr.position;
		castResult->normal = rr.normal;
		castResult->rid = rr.rid;
		castResult->hit_object_id = (uint64_t)rr.collider_id;
		castResult->type = (RayCastResult::PhysicsObjectType)rr.type;
		castResult->shape = rr.shape;
		castResult->face_index = rr.face_index;
		castResult->success = true;
		return true;
	} else {
		RayCastResult::RayCastResultStruct *castResult = (RayCastResult::RayCastResultStruct *)p_result;
		castResult->position = Vector3();
		castResult->normal = Vector3();
		castResult->rid = RID();
		castResult->hit_object_id = 0;
		castResult->type = RayCastResult::PhysicsObjectType::INVALID;
		castResult->shape = -1;
		castResult->face_index = -1;
		castResult->success = false;
		return false;
	}
}
*/

void RayCast3DDirect::set_from_parameters(const Ref<PhysicsRayQueryParameters3D> &p_parameters) {
	ERR_FAIL_COND_MSG(p_parameters.is_null(), "The provided parameters are null.");

	PhysicsDirectSpaceState3D::RayParameters parameters = p_parameters.ptr()->get_parameters();
	source_position = parameters.from;
	target_position = parameters.to;
	exclude = parameters.exclude;
	collision_mask = parameters.collision_mask;
	collide_with_bodies = parameters.collide_with_bodies;
	collide_with_areas = parameters.collide_with_areas;
	hit_from_inside = parameters.hit_from_inside;
	hit_back_faces = parameters.hit_back_faces;
}

void RayCast3DDirect::add_exception_rid(const RID &p_rid) {
	exclude.insert(p_rid);
}

void RayCast3DDirect::add_exception(const CollisionObject3D *p_node) {
	ERR_FAIL_NULL_MSG(p_node, "The passed Node must be an instance of CollisionObject3D.");
	add_exception_rid(p_node->get_rid());
}

void RayCast3DDirect::remove_exception_rid(const RID &p_rid) {
	exclude.erase(p_rid);
}

void RayCast3DDirect::remove_exception(const CollisionObject3D *p_node) {
	ERR_FAIL_NULL_MSG(p_node, "The passed Node must be an instance of CollisionObject3D.");
	remove_exception_rid(p_node->get_rid());
}

void RayCast3DDirect::clear_exceptions() {
	exclude.clear();
}

void RayCast3DDirect::set_collide_with_areas(bool p_enabled) {
	collide_with_areas = p_enabled;
}

bool RayCast3DDirect::is_collide_with_areas_enabled() const {
	return collide_with_areas;
}

void RayCast3DDirect::set_collide_with_bodies(bool p_enabled) {
	collide_with_bodies = p_enabled;
}

bool RayCast3DDirect::is_collide_with_bodies_enabled() const {
	return collide_with_bodies;
}

void RayCast3DDirect::set_hit_from_inside(bool p_enabled) {
	hit_from_inside = p_enabled;
}

bool RayCast3DDirect::is_hit_from_inside_enabled() const {
	return hit_from_inside;
}

void RayCast3DDirect::set_hit_back_faces(bool p_enabled) {
	hit_back_faces = p_enabled;
}

bool RayCast3DDirect::is_hit_back_faces_enabled() const {
	return hit_back_faces;
}

void RayCast3DDirect::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_source_position", "global_point"), &RayCast3DDirect::set_source_position);
	ClassDB::bind_method(D_METHOD("get_source_position"), &RayCast3DDirect::get_source_position);

	ClassDB::bind_method(D_METHOD("set_target_position", "global_point"), &RayCast3DDirect::set_target_position);
	ClassDB::bind_method(D_METHOD("get_target_position"), &RayCast3DDirect::get_target_position);

	ClassDB::bind_method(D_METHOD("set_transform_and_distance", "transform", "length"), &RayCast3DDirect::set_transform_and_distance);

	ClassDB::bind_method(D_METHOD("cast", "space"), &RayCast3DDirect::cast);
	ClassDB::bind_method(D_METHOD("store_in_result", "result"), &RayCast3DDirect::store_in_result);
	ClassDB::bind_static_method("RayCast3DDirect", D_METHOD("cast_statically", "space", "parameters", "result"), &RayCast3DDirect::cast_statically);

	ClassDB::bind_method(D_METHOD("get_hit_something"), &RayCast3DDirect::get_hit_something);
	ClassDB::bind_method(D_METHOD("get_godot_object"), &RayCast3DDirect::get_hit_object);
	ClassDB::bind_method(D_METHOD("get_object_id"), &RayCast3DDirect::get_hit_object_id);
	ClassDB::bind_method(D_METHOD("get_collider_rid"), &RayCast3DDirect::get_collider_rid);
	ClassDB::bind_method(D_METHOD("get_collider_type"), &RayCast3DDirect::get_collider_type);
	ClassDB::bind_method(D_METHOD("get_collider_shape"), &RayCast3DDirect::get_collider_shape);
	ClassDB::bind_method(D_METHOD("get_collider_face_index"), &RayCast3DDirect::get_collider_face_index);
	ClassDB::bind_method(D_METHOD("get_collision_point"), &RayCast3DDirect::get_collision_point);
	ClassDB::bind_method(D_METHOD("get_collision_normal"), &RayCast3DDirect::get_collision_normal);

	ClassDB::bind_method(D_METHOD("set_from_parameters"), &RayCast3DDirect::set_from_parameters);

	ClassDB::bind_method(D_METHOD("add_exception_rid", "rid"), &RayCast3DDirect::add_exception_rid);
	ClassDB::bind_method(D_METHOD("add_exception", "node"), &RayCast3DDirect::add_exception);

	ClassDB::bind_method(D_METHOD("remove_exception_rid", "rid"), &RayCast3DDirect::remove_exception_rid);
	ClassDB::bind_method(D_METHOD("remove_exception", "node"), &RayCast3DDirect::remove_exception);

	ClassDB::bind_method(D_METHOD("clear_exceptions"), &RayCast3DDirect::clear_exceptions);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &RayCast3DDirect::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &RayCast3DDirect::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_collision_mask_value", "layer_number", "value"), &RayCast3DDirect::set_collision_mask_value);
	ClassDB::bind_method(D_METHOD("get_collision_mask_value", "layer_number"), &RayCast3DDirect::get_collision_mask_value);

	ClassDB::bind_method(D_METHOD("set_collide_with_areas", "enable"), &RayCast3DDirect::set_collide_with_areas);
	ClassDB::bind_method(D_METHOD("is_collide_with_areas_enabled"), &RayCast3DDirect::is_collide_with_areas_enabled);

	ClassDB::bind_method(D_METHOD("set_collide_with_bodies", "enable"), &RayCast3DDirect::set_collide_with_bodies);
	ClassDB::bind_method(D_METHOD("is_collide_with_bodies_enabled"), &RayCast3DDirect::is_collide_with_bodies_enabled);

	ClassDB::bind_method(D_METHOD("set_hit_from_inside", "enable"), &RayCast3DDirect::set_hit_from_inside);
	ClassDB::bind_method(D_METHOD("is_hit_from_inside_enabled"), &RayCast3DDirect::is_hit_from_inside_enabled);

	ClassDB::bind_method(D_METHOD("set_hit_back_faces", "enable"), &RayCast3DDirect::set_hit_back_faces);
	ClassDB::bind_method(D_METHOD("is_hit_back_faces_enabled"), &RayCast3DDirect::is_hit_back_faces_enabled);

	ADD_READONLY_PROPERTY(PropertyInfo(Variant::BOOL, "last_hit_something", PROPERTY_HINT_NONE), "get_hit_something");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::VECTOR3, "last_hit_position", PROPERTY_HINT_NONE), "get_collision_point");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::VECTOR3, "last_hit_normal", PROPERTY_HINT_NONE), "get_collision_normal");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::RID, "last_hit_rid", PROPERTY_HINT_NONE), "get_collider_rid");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::INT, "last_hit_object_id", PROPERTY_HINT_NONE), "get_object_id");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::OBJECT, "last_hit_godot_object", PROPERTY_HINT_NONE), "get_godot_object");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::INT, "last_hit_object_type", PROPERTY_HINT_ENUM, "invalid,area,body,soft_body"), "get_collider_type");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::INT, "last_hit_shape_index", PROPERTY_HINT_NONE), "get_collider_shape");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::INT, "last_hit_face_index", PROPERTY_HINT_NONE), "get_collider_face_index");
	
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "source_position", PROPERTY_HINT_NONE), "set_source_position", "get_source_position");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "target_position", PROPERTY_HINT_NONE), "set_target_position", "get_target_position");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hit_from_inside"), "set_hit_from_inside", "is_hit_from_inside_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hit_back_faces"), "set_hit_back_faces", "is_hit_back_faces_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collide_with_areas", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collide_with_areas", "is_collide_with_areas_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collide_with_bodies", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collide_with_bodies", "is_collide_with_bodies_enabled");
}

RayCast3DDirect::RayCast3DDirect() {
}

RayCast3DDirect::RayCast3DDirect(const Ref<PhysicsRayQueryParameters3D> &p_parameters) {
	set_from_parameters(p_parameters);
}

#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
