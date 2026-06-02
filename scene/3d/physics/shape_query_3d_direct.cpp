/**************************************************************************/
/*  shape_query_3d_direct.cpp                                             */
/**************************************************************************/
/*                         This file is part of:                          */
/*                 GODOT ENGINE /// THE CONSERVATORY FORK                 */
/*          https://godotengine.org /// https://xansworkshop.com          */
/**************************************************************************/
/* Copyright (c) 2026-present Xan's Workshop.                             */
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
#include "shape_query_3d_direct.h"
#include "core/object/class_db.h"

int ShapeQuery3DDirect::query(const RID &p_space) {
	PhysicsShapeQueryParameters3D params_on_stack = PhysicsShapeQueryParameters3D();
	Ref<PhysicsShapeQueryParameters3D> parameters = &params_on_stack;
	parameters->set_shape_rid(shape);
	parameters->set_transform(transform);
	parameters->set_margin(margin);
	parameters->set_collision_mask(collision_mask);
	parameters->set_collide_with_bodies(collide_with_bodies);
	parameters->set_collide_with_areas(collide_with_areas);
	parameters->set_exclude(filter);
	parameters->set_change_exclusions_to_inclusions(filter_is_inclusive);
	results = ShapeQuery3DDirect::query_statically(p_space, parameters, max_results);
	result_count = results.size();
	hit_something = !results.is_empty();
	return result_count;
}

TypedArray<ShapeQueryResult> ShapeQuery3DDirect::query_statically(const RID &p_space, const Ref<PhysicsShapeQueryParameters3D> &p_parameters, int p_max_results) {
	ERR_FAIL_COND_V_MSG(p_max_results < 0, TypedArray<ShapeQueryResult>(), "The max results count can not be negative.");
	if (p_max_results == 0) {
		return TypedArray<ShapeQueryResult>();
	}

	PhysicsDirectSpaceState3D *state = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V_MSG(state, TypedArray<ShapeQueryResult>(), "There was no available physics state. Is the provided space RID valid?");

	Vector<PhysicsDirectSpaceState3D::ShapeResult> results;
	results.resize_uninitialized(p_max_results);
	int real_result_count = state->intersect_shape(p_parameters->get_parameters(), results.ptrw(), p_max_results);
	TypedArray<ShapeQueryResult> retval;
	retval.resize(real_result_count);
	for (int i = 0; i < real_result_count; ++i) {
		Ref<ShapeQueryResult> value;
		value.instantiate();
		PhysicsDirectSpaceState3D::ShapeResult shape_result = results[i];
		value->set_collider(shape_result.collider);
		value->set_collider_id(shape_result.collider_id);
		value->set_rid(shape_result.rid);
		value->set_shape(shape_result.shape);
		retval[i] = value;
	}
	return retval;
}

int ShapeQuery3DDirect::query_statically_preallocated(const RID &p_space, const Ref<PhysicsShapeQueryParameters3D> &p_parameters, TypedArray<ShapeQueryResult> p_presized_result_array) {
	if (p_presized_result_array.size() == 0) {
		return 0;
	}

	PhysicsDirectSpaceState3D *state = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V_MSG(state, 0, "There was no available physics state. Is the provided space RID valid?");

	Vector<PhysicsDirectSpaceState3D::ShapeResult> results;
	results.resize_uninitialized(p_presized_result_array.size());
	int real_result_count = state->intersect_shape(p_parameters->get_parameters(), results.ptrw(), p_presized_result_array.size());
	for (int i = 0; i < real_result_count; ++i) {
		Ref<ShapeQueryResult> value = p_presized_result_array[i];
		if (value.is_null()) {
			value.instantiate();
		}
		PhysicsDirectSpaceState3D::ShapeResult shape_result = results[i];
		value->set_collider(shape_result.collider);
		value->set_collider_id(shape_result.collider_id);
		value->set_rid(shape_result.rid);
		value->set_shape(shape_result.shape);
		p_presized_result_array[i] = value;
	}
	return real_result_count;
}

void ShapeQuery3DDirect::set_collision_mask_value(int p_layer_number, bool p_value) {
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

bool ShapeQuery3DDirect::get_collision_mask_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Collision layer number must be between 1 and 32 inclusive.");
	return get_collision_mask() & (1 << (p_layer_number - 1));
}

void ShapeQuery3DDirect::set_from_parameters(const Ref<PhysicsShapeQueryParameters3D>& p_parameters) {
	shape = p_parameters->get_shape_rid();
	transform = p_parameters->get_transform();
	margin = p_parameters->get_margin();
	collision_mask = p_parameters->get_collision_mask();
	collide_with_bodies = p_parameters->is_collide_with_bodies_enabled();
	collide_with_areas = p_parameters->is_collide_with_areas_enabled();
	filter_is_inclusive = p_parameters->get_change_exclusions_to_inclusions();

	// Copy the filter array, sort it, and then remove duplicates.
	// Without this step, add/remove exclusion methods would fail as they rely on binary search.
	TypedArray<RID> filter_pre = p_parameters->get_exclude();
	int length = filter_pre.size();

	TypedArray<RID> filter_post;
	filter_post.resize(length);
	filter_pre.sort();
	RID last = RID();
	for (int i = 0; i < length; ++i) {
		RID current = filter_pre[i];
		if (current != last) {
			// When sorted, duplicates are always consecutive.
			filter_post.push_back(current);
		}
	}
	filter = filter_post;
}

void ShapeQuery3DDirect::add_filter_rid(const RID &p_rid) {
	int index = filter.bsearch(p_rid);
	if (index < 0) {
		filter.insert(~index, p_rid);
	}
}

void ShapeQuery3DDirect::remove_filter_rid(const RID &p_rid) {
	int index = filter.bsearch(p_rid);
	if (index >= 0) {
		filter.remove_at(index);
	}
}

void ShapeQuery3DDirect::clear_filter() {
	filter.clear();
}

void ShapeQuery3DDirect::_bind_methods() {
	XT_AUTO_BIND_PROPERTY(ShapeQuery3DDirect, shape, Variant::RID);
	XT_AUTO_BIND_PROPERTY(ShapeQuery3DDirect, transform, Variant::TRANSFORM3D);
	XT_AUTO_BIND_PROPERTY(ShapeQuery3DDirect, margin, Variant::FLOAT);
	XT_AUTO_BIND_PROPERTY(ShapeQuery3DDirect, collision_mask, Variant::INT);
	XT_AUTO_BIND_PROPERTY(ShapeQuery3DDirect, collide_with_bodies, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(ShapeQuery3DDirect, collide_with_areas, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(ShapeQuery3DDirect, filter_is_inclusive, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(ShapeQuery3DDirect, max_results, Variant::INT);
	XT_AUTO_BIND_READONLY_PROPERTY(ShapeQuery3DDirect, results, Variant::ARRAY);
	XT_AUTO_BIND_READONLY_PROPERTY(ShapeQuery3DDirect, result_count, Variant::INT);
	XT_AUTO_BIND_READONLY_PROPERTY(ShapeQuery3DDirect, hit_something, Variant::BOOL);

	ClassDB::bind_method(D_METHOD("query", "space"), &ShapeQuery3DDirect::query);
	ClassDB::bind_static_method(ShapeQuery3DDirect::get_class_static(), D_METHOD("query_statically_preallocated", "space", "parameters", "presized_result_array"), &ShapeQuery3DDirect::query_statically_preallocated);
	ClassDB::bind_static_method(ShapeQuery3DDirect::get_class_static(), D_METHOD("query_statically", "space", "parameters", "max_results"), &ShapeQuery3DDirect::query_statically);
}

ShapeQuery3DDirect::ShapeQuery3DDirect() { }
#endif
