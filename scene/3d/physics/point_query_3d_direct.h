/**************************************************************************/
/*  point_query_3d_direct.h                                               */
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

#pragma once

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/array.h"
#include "core/variant/typed_array.h"
#include "core/variant/dictionary.h"
#include "core/math/vector3.h"
#include "servers/physics_3d/physics_server_3d.h"
#include "scene/3d/physics/point_query_result.h"

#include "thirdparty/xanstools/xanstools.h"

class PointQuery3DDirect : public RefCounted {
	GDCLASS(PointQuery3DDirect, RefCounted);

protected:
	static void _bind_methods();

public:
	XT_AUTO_PROPERTY_INLINE_C(Vector3, position);
	XT_AUTO_PROPERTY_INLINE_C(uint32_t, collision_mask);
	XT_AUTO_PROPERTY_INLINE_C(bool, collide_with_bodies);
	XT_AUTO_PROPERTY_INLINE_C(bool, collide_with_areas);
	XT_AUTO_PROPERTY_INLINE_C(TypedArray<RID>, exclude);

	TypedArray<PointQueryResult> query(const RID &p_space, int p_max_results) const;
	static TypedArray<PointQueryResult> query_statically(const RID &p_space, const Ref<PhysicsPointQueryParameters3D> &p_parameters, int max_results);
	static int query_statically_preallocated(const RID &p_space, const Ref<PhysicsPointQueryParameters3D> &p_parameters, TypedArray<PointQueryResult> p_presized_result_array);
};
