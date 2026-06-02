/**************************************************************************/
/*  shape_cast_3d_direct.cpp                                              */
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

#ifndef PHYSICS_3D_DISABLED

#include "shape_cast_3d_direct.h"

#include "scene/3d/physics/collision_object_3d.h"
#include "scene/resources/3d/concave_polygon_shape_3d.h"

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

void ShapeCast3DDirect::set_from_parameters(const Ref<PhysicsShapeQueryParameters3D> &p_parameters) {
	shape = p_parameters->get_shape_rid();
	motion = p_parameters->get_motion();
	filter = HashSet<RID>();

	// Yes, this works for include too, since they share the underlying exclude list.
	for (const RID rid : p_parameters->get_exclude()) {
		filter.insert(rid);
	}
	filter_is_inclusive = p_parameters->get_change_exclusions_to_inclusions();
	margin = p_parameters->get_margin();
	collision_mask = p_parameters->get_collision_mask();
	collide_with_areas = p_parameters->is_collide_with_areas_enabled();
	collide_with_bodies = p_parameters->is_collide_with_bodies_enabled();
}

int ShapeCast3DDirect::cast(const RID& p_space) {
	result.clear();

	ERR_FAIL_COND_V_MSG(p_space.is_null(), 0, "Space is invalid. ShapeCast3DDirect requires a valid space to be passed into its cast method.");
	ERR_FAIL_COND_V_MSG(shape.is_null(), 0, "Null reference to shape. ShapeCast3DDirect requires a shape to sweep for collisions.");

	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V(dss, 0);

	PhysicsDirectSpaceState3D::ShapeParameters params;
	params.shape_rid = shape;
	params.transform = source_transform;
	params.motion = motion;
	params.margin = margin;
	params.exclude = filter;
	params.exclude_is_actually_include = filter_is_inclusive;
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
	int count = 0;
	while (intersected && result.size() < max_results) {
		PhysicsDirectSpaceState3D::ShapeRestInfo info;
		intersected = dss->rest_info(params, &info);
		if (intersected) {
			result.push_back(info);
			params.exclude.insert(info.rid);
			count++;
		}
	}
	hit_something = !result.is_empty();
	result_count = count;
	results.resize(count);
	for (int i = 0; i < count; i++) {
		PhysicsDirectSpaceState3D::ShapeRestInfo info = result.get(i);
		Ref<ShapeCastResult> result_instance;
		result_instance.instantiate();
		result_instance->set_hit_something(true);
		result_instance->set_intersection_point(info.point);
		result_instance->set_intersection_normal(info.normal);
		result_instance->set_rid(info.rid);
		result_instance->set_hit_object_id(info.collider_id);
		result_instance->set_hit_object(ObjectDB::get_instance(info.collider_id));
		result_instance->set_shape_index(info.shape);
		result_instance->set_linear_velocity_at_contact(info.linear_velocity);
		results[i] = result_instance;
	}

	return count;
}

TypedArray<ShapeCastResult> ShapeCast3DDirect::cast_statically(const RID &p_space, const Ref<PhysicsShapeQueryParameters3D> &p_parameters, const int p_max_results) {
	TypedArray<ShapeCastResult> out_result = TypedArray<ShapeCastResult>();
	ERR_FAIL_COND_V_MSG(p_max_results <= 0, out_result, "The provided results array must be resized before calling cast_statically, as its capacity determines the amount of results.");
	ERR_FAIL_COND_V_MSG(p_space.is_null(), out_result, "Space is invalid. ShapeCast3DDirect requires a valid space to be passed into its cast method.");
	ERR_FAIL_COND_V_MSG(p_parameters->get_shape_rid().is_null(), out_result, "Shape is invalid. ShapeCast3DDirect requires a shape to sweep for collisions.");

	PhysicsDirectSpaceState3D *dss = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V(dss, out_result);

	out_result.resize(p_max_results);

	PhysicsDirectSpaceState3D::ShapeParameters params = p_parameters->get_parameters();
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
			result_instance->set_hit_something(true);
			result_instance->set_intersection_point(info.point);
			result_instance->set_intersection_normal(info.normal);
			result_instance->set_rid(info.rid);
			result_instance->set_hit_object_id(info.collider_id);
			result_instance->set_hit_object(ObjectDB::get_instance(info.collider_id));
			result_instance->set_shape_index(info.shape);
			result_instance->set_linear_velocity_at_contact(info.linear_velocity);
			result_instance->set_collision_safe_fraction(collision_safe_fraction);
			result_instance->set_collision_unsafe_fraction(collision_unsafe_fraction);
			out_result.set(result_count++, result_instance);
		} else {
			break;
		}
	}

	return out_result;
}

void ShapeCast3DDirect::add_filter_rid(const RID &p_rid) {
	filter.insert(p_rid);
}
void ShapeCast3DDirect::remove_filter_rid(const RID &p_rid) {
	filter.erase(p_rid);
}
void ShapeCast3DDirect::clear_filter() {
	filter.clear();
}

void ShapeCast3DDirect::_bind_methods() {
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, shape, Variant::RID);
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, source_transform, Variant::TRANSFORM3D);
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, margin, Variant::FLOAT);
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, motion, Variant::VECTOR3);
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, collision_mask, Variant::INT);
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, collide_with_bodies, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, collide_with_areas, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, filter_is_inclusive, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(ShapeCast3DDirect, max_results, Variant::INT);

	XT_AUTO_BIND_READONLY_PROPERTY(ShapeCast3DDirect, hit_something, Variant::BOOL);
	XT_AUTO_BIND_READONLY_PROPERTY(ShapeCast3DDirect, result_count, Variant::BOOL);
	XT_AUTO_BIND_READONLY_PROPERTY(ShapeCast3DDirect, results, Variant::ARRAY);
	XT_AUTO_BIND_READONLY_PROPERTY(ShapeCast3DDirect, collision_safe_fraction, Variant::FLOAT);
	XT_AUTO_BIND_READONLY_PROPERTY(ShapeCast3DDirect, collision_unsafe_fraction, Variant::FLOAT);

	ClassDB::bind_method(D_METHOD("set_collision_mask_value", "layer_number", "value"), &ShapeCast3DDirect::set_collision_mask_value);
	ClassDB::bind_method(D_METHOD("get_collision_mask_value", "layer_number"), &ShapeCast3DDirect::get_collision_mask_value);

	ClassDB::bind_method(D_METHOD("set_from_parameters", "parameters"), &ShapeCast3DDirect::set_from_parameters);
	ClassDB::bind_method(D_METHOD("cast", "space"), &ShapeCast3DDirect::cast);
	ClassDB::bind_static_method(ShapeCast3DDirect::get_class_static(), D_METHOD("cast_statically", "space", "parameters", "max_results"), &ShapeCast3DDirect::cast_statically);
}

#endif // PHYSICS_3D_DISABLED
