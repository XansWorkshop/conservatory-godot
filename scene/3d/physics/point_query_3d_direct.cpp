/**************************************************************************/
/*  point_query_3d_direct.cpp                                             */
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

#include "point_query_3d_direct.h"
#include "core/object/class_db.h"

void PointQuery3DDirect::_bind_methods() {
	XT_AUTO_BIND_PROPERTY(PointQuery3DDirect, position, Variant::VECTOR3);
	XT_AUTO_BIND_PROPERTY(PointQuery3DDirect, collision_mask, Variant::INT);
	XT_AUTO_BIND_PROPERTY(PointQuery3DDirect, collide_with_bodies, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(PointQuery3DDirect, collide_with_areas, Variant::BOOL);
	XT_AUTO_BIND_PROPERTY(PointQuery3DDirect, exclude, Variant::ARRAY);

	ClassDB::bind_method(D_METHOD("query", "space", "max_results"), &PointQuery3DDirect::query);

	ClassDB::bind_static_method(PointQuery3DDirect::get_class_static(), D_METHOD("query_statically_preallocated", "space", "parameters", "presized_result_array"), &PointQuery3DDirect::query_statically_preallocated);
	ClassDB::bind_static_method(PointQuery3DDirect::get_class_static(), D_METHOD("query_statically", "space", "parameters", "max_results"), &PointQuery3DDirect::query_statically);
}

TypedArray<PointQueryResult> PointQuery3DDirect::query(const RID& p_space, int p_max_results) const {
	PhysicsPointQueryParameters3D params_on_stack = PhysicsPointQueryParameters3D();
	Ref<PhysicsPointQueryParameters3D> parameters = &params_on_stack;
	parameters->set_position(position);
	parameters->set_collision_mask(collision_mask);
	parameters->set_collide_with_bodies(collide_with_bodies);
	parameters->set_collide_with_areas(collide_with_areas);
	parameters->set_exclude(exclude);
	return PointQuery3DDirect::query_statically(p_space, parameters, p_max_results);
}

TypedArray<PointQueryResult> PointQuery3DDirect::query_statically(const RID &p_space, const Ref<PhysicsPointQueryParameters3D> &p_parameters, int p_max_results) {
	ERR_FAIL_COND_V_MSG(p_max_results < 0, TypedArray<PointQueryResult>(), "The max results count can not be negative.");
	if (p_max_results == 0) {
		return TypedArray<PointQueryResult>();
	}

	PhysicsDirectSpaceState3D *state = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_V_MSG(state, TypedArray<PointQueryResult>(), "There was no available physics state. Is the provided space RID valid?");

	Vector<PhysicsDirectSpaceState3D::ShapeResult> results;
	results.resize_uninitialized(p_max_results);
	int real_result_count = state->intersect_point(p_parameters->get_parameters(), results.ptrw(), p_max_results);
	TypedArray<PointQueryResult> retval;
	retval.resize(real_result_count);
	for (int i = 0; i < real_result_count; ++i) {
		Ref<PointQueryResult> value;
		value.instantiate();
		PhysicsDirectSpaceState3D::ShapeResult point_result = results[i];
		value->set_collider(point_result.collider);
		value->set_collider_id(point_result.collider_id);
		value->set_rid(point_result.rid);
		value->set_shape(point_result.shape);
		retval[i] = value;
	}
	return retval;
}

void PointQuery3DDirect::query_statically_preallocated(const RID &p_space, const Ref<PhysicsPointQueryParameters3D> &p_parameters, TypedArray<PointQueryResult> p_presized_result_array) {
	if (p_presized_result_array.size() == 0) {
		return;
	}

	PhysicsDirectSpaceState3D *state = PhysicsServer3D::get_singleton()->space_get_direct_state(p_space);
	ERR_FAIL_NULL_MSG(state, "There was no available physics state. Is the provided space RID valid?");

	Vector<PhysicsDirectSpaceState3D::ShapeResult> results;
	results.resize_uninitialized(p_presized_result_array.size());
	int real_result_count = state->intersect_point(p_parameters->get_parameters(), results.ptrw(), p_presized_result_array.size());
	for (int i = 0; i < real_result_count; ++i) {
		Ref<PointQueryResult> value = p_presized_result_array[i];
		if (value.is_null()) {
			value.instantiate();
		}
		PhysicsDirectSpaceState3D::ShapeResult point_result = results[i];
		value->set_collider(point_result.collider);
		value->set_collider_id(point_result.collider_id);
		value->set_rid(point_result.rid);
		value->set_shape(point_result.shape);
		p_presized_result_array[i] = value;
	}
}
