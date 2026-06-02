/**************************************************************************/
/*  ray_cast_3d_direct.cpp                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                 GODOT ENGINE /// THE CONSERVATORY FORK                 */
/*          https://godotengine.org /// https://xansworkshop.com          */
/**************************************************************************/
/* Copyright (c) 2025-present Xan's Workshop.                             */
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

void RayCast3DDirect::set_transform_and_distance(const Transform3D& p_transform, real_t p_length) {
	source_position = p_transform.origin;
	target_position = p_transform.xform(Vector3(0, 0, -p_length));
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

bool RayCast3DDirect::cast(const RID &p_space) {
	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V_MSG(dss, false, "There is no direct space state associated with the provided space.");

	PhysicsDirectSpaceState3D::RayParameters ray_params;
	ray_params.from = source_position;
	ray_params.to = target_position;
	ray_params.exclude = filter;
	ray_params.exclude_is_actually_include = filter_is_inclusive;
	ray_params.collision_mask = collision_mask;
	ray_params.collide_with_bodies = collide_with_bodies;
	ray_params.collide_with_areas = collide_with_areas;
	ray_params.hit_from_inside = collide_from_inside;
	ray_params.hit_back_faces = collide_with_back_faces;

	PhysicsDirectSpaceState3D::RayResult rr;
	if (dss->intersect_ray(ray_params, rr)) {
		hit_something = true;
		collision_point = rr.position;
		collision_normal = rr.normal;
		hit_face_index = rr.face_index;
		hit_shape_index = rr.shape;

		hit_collider_id = rr.collider_id;
		hit_rid = rr.rid;
	} else {
		hit_something = false;
		collision_point = Vector3(0, 0, 0);
		collision_normal = Vector3(0, 0, 0);
		hit_face_index = -1;
		hit_shape_index = -1;

		hit_collider_id = ObjectID();
		hit_rid = RID();
	}

	return hit_something;
}

void RayCast3DDirect::store_in_result(const Ref<RayCastResult> &p_result) const {
	p_result->set_hit_something(hit_something);
	p_result->set_origin(source_position);
	p_result->set_hit_position(collision_point);
	p_result->set_hit_normal(collision_normal);
	p_result->set_rid(hit_rid);
	p_result->set_hit_object_id(hit_collider_id);
	p_result->set_shape_index(hit_shape_index);
	p_result->set_face_index(hit_face_index);
}

bool RayCast3DDirect::cast_statically(const RID &p_space, const Ref<PhysicsRayQueryParameters3D> &p_parameters, const Ref<RayCastResult> &p_result) {
	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V_MSG(dss, false, "There is no direct space state associated with the provided space.");
	ERR_FAIL_COND_V_MSG(p_parameters.is_null(), false, "The PhysicsRayQueryParameters3D instance is not valid.");
	ERR_FAIL_COND_V_MSG(p_result.is_null(), false, "The RayCastResult instance is not valid.");

	
	PhysicsDirectSpaceState3D::RayResult rr;
	if (dss->intersect_ray(p_parameters->get_parameters(), rr)) {
		p_result->set_origin(p_parameters->get_from());
		p_result->set_hit_position(rr.position);
		p_result->set_hit_normal(rr.normal);
		p_result->set_rid(rr.rid);
		p_result->set_hit_object_id(rr.collider_id);
		p_result->set_hit_object(rr.collider);
		p_result->set_shape_index(rr.shape);
		p_result->set_face_index(rr.face_index);
		p_result->set_hit_something(true);
		return true;
	} else {
		p_result->clear();
		return false;
	}
}

void RayCast3DDirect::set_from_parameters(const Ref<PhysicsRayQueryParameters3D> &p_parameters) {
	ERR_FAIL_COND_MSG(p_parameters.is_null(), "The provided parameters are null.");

	PhysicsDirectSpaceState3D::RayParameters parameters = p_parameters.ptr()->get_parameters();
	source_position = parameters.from;
	target_position = parameters.to;
	filter = parameters.exclude;
	filter_is_inclusive = parameters.exclude_is_actually_include;
	collision_mask = parameters.collision_mask;
	collide_with_bodies = parameters.collide_with_bodies;
	collide_with_areas = parameters.collide_with_areas;
	collide_from_inside = parameters.hit_from_inside;
	collide_with_back_faces = parameters.hit_back_faces;
}

void RayCast3DDirect::add_filter_rid(const RID &p_rid) {
	filter.insert(p_rid);
}

void RayCast3DDirect::remove_filter_rid(const RID &p_rid) {
	filter.erase(p_rid);
}

void RayCast3DDirect::clear_filter() {
	filter.clear();
}

void RayCast3DDirect::_bind_methods() {
	XT_AUTO_BIND_PROPERTY(RayCast3DDirect, collision_mask, Variant::INT);
	XT_AUTO_BIND_PROPERTY(RayCast3DDirect, collide_with_bodies, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(RayCast3DDirect, collide_with_areas, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(RayCast3DDirect, collide_from_inside, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(RayCast3DDirect, collide_with_back_faces, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(RayCast3DDirect, filter_is_inclusive, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(RayCast3DDirect, source_position, Variant::VECTOR3);
	XT_AUTO_BIND_PROPERTY(RayCast3DDirect, target_position, Variant::VECTOR3);

	XT_AUTO_BIND_READONLY_PROPERTY_SPECIAL_OBJECTID(RayCast3DDirect, hit_collider);
	XT_AUTO_BIND_READONLY_PROPERTY(RayCast3DDirect, hit_something, Variant::BOOL);
	XT_AUTO_BIND_READONLY_PROPERTY(RayCast3DDirect, hit_rid, Variant::RID);
	XT_AUTO_BIND_READONLY_PROPERTY(RayCast3DDirect, hit_shape_index, Variant::INT);
	XT_AUTO_BIND_READONLY_PROPERTY(RayCast3DDirect, hit_face_index, Variant::INT);
	XT_AUTO_BIND_READONLY_PROPERTY(RayCast3DDirect, collision_point, Variant::VECTOR3);
	XT_AUTO_BIND_READONLY_PROPERTY(RayCast3DDirect, collision_normal, Variant::VECTOR3);

	ClassDB::bind_method(D_METHOD("set_transform_and_distance", "transform", "length"), &RayCast3DDirect::set_transform_and_distance);
	ClassDB::bind_method(D_METHOD("set_from_parameters"), &RayCast3DDirect::set_from_parameters);
	ClassDB::bind_method(D_METHOD("add_filter_rid", "rid"), &RayCast3DDirect::add_filter_rid);
	ClassDB::bind_method(D_METHOD("remove_filter_rid", "rid"), &RayCast3DDirect::remove_filter_rid);
	ClassDB::bind_method(D_METHOD("clear_filter"), &RayCast3DDirect::clear_filter);

	ClassDB::bind_method(D_METHOD("cast", "space"), &RayCast3DDirect::cast);
	ClassDB::bind_method(D_METHOD("store_in_result", "result"), &RayCast3DDirect::store_in_result);
	ClassDB::bind_static_method(RayCast3DDirect::get_class_static(), D_METHOD("cast_statically", "space", "parameters", "result"), &RayCast3DDirect::cast_statically);

	ClassDB::bind_method(D_METHOD("set_collision_mask_value", "layer_number", "value"), &RayCast3DDirect::set_collision_mask_value);
	ClassDB::bind_method(D_METHOD("get_collision_mask_value", "layer_number"), &RayCast3DDirect::get_collision_mask_value);
}

RayCast3DDirect::RayCast3DDirect() {
}

RayCast3DDirect::RayCast3DDirect(const Ref<PhysicsRayQueryParameters3D> &p_parameters) {
	set_from_parameters(p_parameters);
}

#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
