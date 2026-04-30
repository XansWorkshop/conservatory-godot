/**************************************************************************/
/*  shape_cast_3d_direct.h                                                */
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

#pragma once

#include "scene/3d/node_3d.h"
#include "scene/resources/3d/shape_3d.h"
#include "scene/3d/physics/shape_cast_result.h"

#include "thirdparty/xanstools/xanstools.h"

class CollisionObject3D;

class ShapeCast3DDirect : public RefCounted {
	GDCLASS(ShapeCast3DDirect, RefCounted);

	HashSet<RID> filter;

	// Result
	int last_result_count = 0;
	Vector<PhysicsDirectSpaceState3D::ShapeRestInfo> result;
	bool hit_something = false;
	real_t collision_safe_fraction = 1.0;
	real_t collision_unsafe_fraction = 1.0;

protected:
	static void _bind_methods();

public:
	XT_AUTO_PROPERTY_INLINE_C(bool, collide_with_bodies);
	XT_AUTO_PROPERTY_INLINE_C(bool, collide_with_areas);
	XT_AUTO_PROPERTY_INLINE_C(uint32_t, collision_mask);
	XT_AUTO_PROPERTY_INLINE_C(RID, shape);
	XT_AUTO_PROPERTY_INLINE_C(Transform3D, source_transform);
	XT_AUTO_PROPERTY_INLINE_C(Vector3, motion);
	XT_AUTO_PROPERTY_INLINE_C(real_t, margin);
	XT_AUTO_PROPERTY_INLINE_C(int, max_results);
	XT_AUTO_PROPERTY_INLINE_C(bool, filter_is_inclusive);

	void set_collision_mask_value(int p_layer_number, bool p_value);
	bool get_collision_mask_value(int p_layer_number) const;

	bool get_hit_anything() const;
	int get_hit_count() const;
	TypedArray<ShapeCastResult> get_results() const;
	real_t get_closest_collision_safe_fraction() const;
	real_t get_closest_collision_unsafe_fraction() const;

	void set_from_parameters(const Ref<PhysicsShapeQueryParameters3D> &p_parameters);
	int cast(const RID &p_space);
	static TypedArray<ShapeCastResult> cast_statically(const RID &p_space, const Ref<PhysicsShapeQueryParameters3D> &p_parameters, const int p_max_results);

	void add_filter_rid(const RID &p_rid);
	void remove_filter_rid(const RID &p_rid);
	void clear_filter();
};
