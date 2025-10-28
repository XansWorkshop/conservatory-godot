/**************************************************************************/
/*  shape_cast_3d_direct.cpp                                              */
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

#include "shape_cast_3d_direct.h"

#include "scene/3d/physics/collision_object_3d.h"
#include "scene/resources/3d/concave_polygon_shape_3d.h"

void ShapeCast3DDirect::set_collide_with_areas(bool p_clip) {
	collide_with_areas = p_clip;
}

bool ShapeCast3DDirect::is_collide_with_areas_enabled() const {
	return collide_with_areas;
}

void ShapeCast3DDirect::set_collide_with_bodies(bool p_clip) {
	collide_with_bodies = p_clip;
}

bool ShapeCast3DDirect::is_collide_with_bodies_enabled() const {
	return collide_with_bodies;
}

void ShapeCast3DDirect::set_shape_rid(const RID &p_rid) {
	shape_rid = p_rid;
}

RID ShapeCast3DDirect::get_shape_rid() const {
	return shape_rid;
}

void ShapeCast3DDirect::set_source_transform(const Transform3D &p_transform) {
	source_transform = p_transform;
}

Transform3D ShapeCast3DDirect::get_source_transform() const {
	return source_transform;
}

void ShapeCast3DDirect::set_motion(const Vector3 &p_motion) {
	motion = p_motion;
}

Vector3 ShapeCast3DDirect::get_motion() const {
	return motion;
}

void ShapeCast3DDirect::set_margin(real_t p_margin) {
	margin = p_margin;
}

real_t ShapeCast3DDirect::get_margin() const {
	return margin;
}

void ShapeCast3DDirect::set_max_results(int p_max_results) {
	max_results = p_max_results;
}

int ShapeCast3DDirect::get_max_results() const {
	return max_results;
}

void ShapeCast3DDirect::set_collision_mask(uint32_t p_mask) {
	collision_mask = p_mask;
}

uint32_t ShapeCast3DDirect::get_collision_mask() const {
	return collision_mask;
}

void ShapeCast3DDirect::set_collision_mask_value(int p_layer_number, bool p_value) {
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

bool ShapeCast3DDirect::get_collision_mask_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Collision layer number must be between 1 and 32 inclusive.");
	return get_collision_mask() & (1 << (p_layer_number - 1));
}

bool ShapeCast3DDirect::get_hit_anything() const {
	return hit_something;
}

int ShapeCast3DDirect::get_hit_count() const {
	return last_result_count;
}

TypedArray<ShapeCastResult> ShapeCast3DDirect::get_results() const {
	int amount = get_hit_count();
	TypedArray<ShapeCastResult> returned_results;
	returned_results.resize(amount);
	for (int i = 0; i < amount; i++) {
		PhysicsDirectSpaceState3D::ShapeRestInfo info = result.get(i);
		Ref<ShapeCastResult> result_instance;
		result_instance.instantiate();
		result_instance->set_success(true);
		result_instance->set_intersection_point(info.point);
		result_instance->set_intersection_normal(info.normal);
		result_instance->set_rid(info.rid);
		result_instance->set_hit_object_id_and_instance((int64_t)info.collider_id);
		result_instance->set_collider_type((ShapeCastResult::PhysicsObjectType)info.type);
		result_instance->set_shape_index(info.shape);
		result_instance->set_linear_velocity_at_contact(info.linear_velocity);
		returned_results.set(i, result_instance);
	}
	return returned_results;
}

real_t ShapeCast3DDirect::get_closest_collision_safe_fraction() const {
	return collision_safe_fraction;
}

real_t ShapeCast3DDirect::get_closest_collision_unsafe_fraction() const {
	return collision_unsafe_fraction;
}

void ShapeCast3DDirect::set_from_parameters(const Ref<PhysicsShapeQueryParameters3D> &p_parameters) {
	shape_rid = p_parameters->get_shape_rid();
	motion = p_parameters->get_motion();
	exclude = HashSet<RID>();
	for (const RID &rid : p_parameters->get_exclude()) {
		exclude.insert(rid);
	}
	margin = p_parameters->get_margin();
	collision_mask = p_parameters->get_collision_mask();
	collide_with_areas = p_parameters->is_collide_with_areas_enabled();
	collide_with_bodies = p_parameters->is_collide_with_bodies_enabled();
}

int ShapeCast3DDirect::cast(const RID& p_space) {
	result.clear();

	ERR_FAIL_COND_V_MSG(p_space.is_null(), 0, "Space is invalid. ShapeCast3DDirect requires a valid space to be passed into its cast method.");
	ERR_FAIL_COND_V_MSG(shape_rid.is_null(), 0, "Null reference to shape. ShapeCast3DDirect requires a shape to sweep for collisions.");

	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V(dss, 0);

	PhysicsDirectSpaceState3D::ShapeParameters params;
	params.shape_rid = shape_rid;
	params.transform = source_transform;
	params.motion = motion;
	params.margin = margin;
	params.exclude = exclude;
	params.collision_mask = collision_mask;
	params.collide_with_bodies = collide_with_bodies;
	params.collide_with_areas = collide_with_areas;

	dss->cast_motion(params, collision_safe_fraction, collision_unsafe_fraction);
	if (collision_unsafe_fraction < 1.0) {
		// Move shape transform to the point of impact,
		// so we can collect contact info at that point.
		Transform3D modified_trs = Transform3D(source_transform);
		modified_trs.set_origin(modified_trs.get_origin() + params.motion * (collision_unsafe_fraction + CMP_EPSILON));
		params.transform = modified_trs;
	}
	// Regardless of whether the shape is stuck or it's moved along
	// the motion vector, we'll only consider static collisions from now on.
	params.motion = Vector3();

	bool intersected = true;
	int result_count = 0;
	while (intersected && result.size() < max_results) {
		PhysicsDirectSpaceState3D::ShapeRestInfo info;
		intersected = dss->rest_info(params, &info);
		if (intersected) {
			result.push_back(info);
			params.exclude.insert(info.rid);
			result_count++;
		}
	}
	hit_something = !result.is_empty();
	last_result_count = result_count;
	return result_count;
}

TypedArray<ShapeCastResult> ShapeCast3DDirect::cast_statically(const RID &p_space, const Ref<PhysicsShapeQueryParameters3D> &p_parameters, const int p_max_results, const Ref<ShapeCastResultExtras> &p_extra_context) {
	TypedArray<ShapeCastResult> out_result = TypedArray<ShapeCastResult>();
	ERR_FAIL_COND_V_MSG(p_max_results <= 0, out_result, "The provided results array must be resized before calling cast_statically, as its capacity determines the amount of results.");
	ERR_FAIL_COND_V_MSG(p_space.is_null(), out_result, "Space is invalid. ShapeCast3DDirect requires a valid space to be passed into its cast method.");
	ERR_FAIL_COND_V_MSG(p_parameters->get_shape_rid().is_null(), out_result, "Shape is invalid. ShapeCast3DDirect requires a shape to sweep for collisions.");

	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V(dss, out_result);

	out_result.resize(p_max_results);

	PhysicsDirectSpaceState3D::ShapeParameters params;
	params.shape_rid = p_parameters->get_shape_rid();
	params.transform = p_parameters->get_transform();
	params.motion = p_parameters->get_motion();
	params.margin = p_parameters->get_margin();
	params.collision_mask = p_parameters->get_collision_mask();
	params.collide_with_bodies = p_parameters->is_collide_with_bodies_enabled();
	params.collide_with_areas = p_parameters->is_collide_with_areas_enabled();
	for (const RID &rid : p_parameters->get_exclude()) {
		params.exclude.insert(rid);
	}

	real_t collision_safe_fraction = 0;
	real_t collision_unsafe_fraction = 0;
	dss->cast_motion(params, collision_safe_fraction, collision_unsafe_fraction);
	if (collision_unsafe_fraction < 1.0) {
		// Move shape transform to the point of impact,
		// so we can collect contact info at that point.
		Transform3D modified_trs = Transform3D(params.transform);
		modified_trs.set_origin(modified_trs.get_origin() + params.motion * (collision_unsafe_fraction + CMP_EPSILON));
		params.transform = modified_trs;
	}
	// Regardless of whether the shape is stuck or it's moved along
	// the motion vector, we'll only consider static collisions from now on.
	params.motion = Vector3();

	int result_count = 0;
	int max_results = out_result.size();
	PhysicsDirectSpaceState3D::ShapeRestInfo info;
	while (result_count < max_results) {
		if (dss->rest_info(params, &info)) {
			params.exclude.insert(info.rid);

			Ref<ShapeCastResult> result_instance;
			result_instance.instantiate();
			result_instance->set_success(true);
			result_instance->set_intersection_point(info.point);
			result_instance->set_intersection_normal(info.normal);
			result_instance->set_rid(info.rid);
			result_instance->set_hit_object_id_and_instance((int64_t)info.collider_id);
			result_instance->set_collider_type((ShapeCastResult::PhysicsObjectType)info.type);
			result_instance->set_shape_index(info.shape);
			result_instance->set_linear_velocity_at_contact(info.linear_velocity);
			out_result.set(result_count++, result_instance);
		} else {
			break;
		}
	}

	if (p_extra_context.is_valid()) {
		p_extra_context->set_collision_safe_fraction(collision_safe_fraction);
		p_extra_context->set_collision_unsafe_fraction(collision_unsafe_fraction);
	}

	return out_result; //Vector2(collision_safe_fraction, collision_unsafe_fraction);
}

void ShapeCast3DDirect::add_exception_rid(const RID &p_rid) {
	exclude.insert(p_rid);
}

void ShapeCast3DDirect::add_exception(const CollisionObject3D *p_node) {
	ERR_FAIL_NULL_MSG(p_node, "The passed Node must be an instance of CollisionObject3D.");
	add_exception_rid(p_node->get_rid());
}

void ShapeCast3DDirect::remove_exception_rid(const RID &p_rid) {
	exclude.erase(p_rid);
}

void ShapeCast3DDirect::remove_exception(const CollisionObject3D *p_node) {
	ERR_FAIL_NULL_MSG(p_node, "The passed Node must be an instance of CollisionObject3D.");
	remove_exception_rid(p_node->get_rid());
}

void ShapeCast3DDirect::clear_exceptions() {
	exclude.clear();
}

void ShapeCast3DDirect::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_shape_rid", "shape"), &ShapeCast3DDirect::set_shape_rid);
	ClassDB::bind_method(D_METHOD("get_shape_rid"), &ShapeCast3DDirect::get_shape_rid);

	ClassDB::bind_method(D_METHOD("set_source_transform", "transform"), &ShapeCast3DDirect::set_source_transform);
	ClassDB::bind_method(D_METHOD("get_source_transform"), &ShapeCast3DDirect::get_source_transform);

	ClassDB::bind_method(D_METHOD("set_motion", "motion"), &ShapeCast3DDirect::set_motion);
	ClassDB::bind_method(D_METHOD("get_motion"), &ShapeCast3DDirect::get_motion);

	ClassDB::bind_method(D_METHOD("set_margin", "margin"), &ShapeCast3DDirect::set_margin);
	ClassDB::bind_method(D_METHOD("get_margin"), &ShapeCast3DDirect::get_margin);

	ClassDB::bind_method(D_METHOD("set_max_results", "max_results"), &ShapeCast3DDirect::set_max_results);
	ClassDB::bind_method(D_METHOD("get_max_results"), &ShapeCast3DDirect::get_max_results);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &ShapeCast3DDirect::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &ShapeCast3DDirect::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_collision_mask_value", "layer_number", "value"), &ShapeCast3DDirect::set_collision_mask_value);
	ClassDB::bind_method(D_METHOD("get_collision_mask_value", "layer_number"), &ShapeCast3DDirect::get_collision_mask_value);

	ClassDB::bind_method(D_METHOD("get_hit_anything"), &ShapeCast3DDirect::get_hit_anything);
	ClassDB::bind_method(D_METHOD("get_hit_count"), &ShapeCast3DDirect::get_hit_count);
	ClassDB::bind_method(D_METHOD("get_results"), &ShapeCast3DDirect::get_results);

	ClassDB::bind_method(D_METHOD("get_closest_collision_safe_fraction"), &ShapeCast3DDirect::get_closest_collision_safe_fraction);
	ClassDB::bind_method(D_METHOD("get_closest_collision_unsafe_fraction"), &ShapeCast3DDirect::get_closest_collision_unsafe_fraction);

	ClassDB::bind_method(D_METHOD("set_from_parameters", "parameters"), &ShapeCast3DDirect::set_from_parameters);
	ClassDB::bind_method(D_METHOD("cast", "space"), &ShapeCast3DDirect::cast);
	ClassDB::bind_static_method("ShapeCast3DDirect", D_METHOD("cast_statically", "space", "parameters", "max_results", "additional_info"), &ShapeCast3DDirect::cast_statically);

	ClassDB::bind_method(D_METHOD("set_collide_with_areas", "enable"), &ShapeCast3DDirect::set_collide_with_areas);
	ClassDB::bind_method(D_METHOD("is_collide_with_areas_enabled"), &ShapeCast3DDirect::is_collide_with_areas_enabled);

	ClassDB::bind_method(D_METHOD("set_collide_with_bodies", "enable"), &ShapeCast3DDirect::set_collide_with_bodies);
	ClassDB::bind_method(D_METHOD("is_collide_with_bodies_enabled"), &ShapeCast3DDirect::is_collide_with_bodies_enabled);

	ClassDB::bind_method(D_METHOD("add_exception_rid", "rid"), &ShapeCast3DDirect::add_exception_rid);
	ClassDB::bind_method(D_METHOD("add_exception", "node"), &ShapeCast3DDirect::add_exception);

	ClassDB::bind_method(D_METHOD("remove_exception_rid", "rid"), &ShapeCast3DDirect::remove_exception_rid);
	ClassDB::bind_method(D_METHOD("remove_exception", "node"), &ShapeCast3DDirect::remove_exception);

	ClassDB::bind_method(D_METHOD("clear_exceptions"), &ShapeCast3DDirect::clear_exceptions);

	ADD_PROPERTY(PropertyInfo(Variant::RID, "shape_rid", PROPERTY_HINT_NONE), "set_shape_rid", "get_shape_rid");
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM3D, "source_transform", PROPERTY_HINT_NONE), "set_source_transform", "get_source_transform");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "motion", PROPERTY_HINT_NONE), "set_motion", "get_motion");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "margin", PROPERTY_HINT_RANGE, "0,100,0.01,suffix:m"), "set_margin", "get_margin");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_results"), "set_max_results", "get_max_results");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::BOOL, "hit_anything", PROPERTY_HINT_NONE), "get_hit_anything");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_count", PROPERTY_HINT_NONE), "get_hit_count");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::ARRAY, "results", PROPERTY_HINT_ARRAY_TYPE, "ShapeCastResult"), "get_results");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::INT, "closest_collision_safe_fraction", PROPERTY_HINT_NONE), "get_closest_collision_safe_fraction");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::INT, "closest_collision_unsafe_fraction", PROPERTY_HINT_NONE), "get_closest_collision_unsafe_fraction");

	ADD_GROUP("Collide With", "collide_with");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collide_with_areas"), "set_collide_with_areas", "is_collide_with_areas_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collide_with_bodies"), "set_collide_with_bodies", "is_collide_with_bodies_enabled");
}

#endif // PHYSICS_3D_DISABLED
