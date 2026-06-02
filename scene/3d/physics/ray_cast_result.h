/**************************************************************************/
/*  ray_cast_result.h                                                     */
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

#pragma once

#include "core/typedefs.h"
#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/object/object_id.h"
#include "core/templates/rid.h"
#include "core/math/vector3.h"
#include "core/variant/binder_common.h"
#include "core/config/project_settings.h"

#include "thirdparty/xanstools/xanstools.h"

#define JOLT_ALLOWS_RAYCAST_FACE_INDEX (GLOBAL_GET("physics/jolt_physics_3d/queries/enable_ray_cast_face_index").booleanize())
#define IS_USING_JOLT ((String)GLOBAL_GET("physics/3d/physics_engine") == "Jolt Physics")

class RayCastResult : public RefCounted {
	GDCLASS(RayCastResult, RefCounted);

	XT_AUTO_PROPERTY_SPECIAL_OBJECTID_WITH_OBJECT(hit_object);
	XT_AUTO_PROPERTY_INLINE_DC(bool, hit_something) = false;
	XT_AUTO_PROPERTY_INLINE_C(Vector3, hit_position) = Vector3();
	XT_AUTO_PROPERTY_INLINE_C(Vector3, hit_normal) = Vector3();
	XT_AUTO_PROPERTY_INLINE_C(RID, rid) = RID();
	XT_AUTO_PROPERTY_INLINE_DC(int, shape_index) = -1;
	XT_AUTO_PROPERTY_DC(int, face_index) = -1;
	XT_AUTO_PROPERTY_INLINE_C(Vector3, origin) = Vector3();

private:
	static uint8_t _can_index_face;

	static bool can_index_face() {
		if (_can_index_face == 0) {
			if (IS_USING_JOLT) {
				if (JOLT_ALLOWS_RAYCAST_FACE_INDEX) {
					_can_index_face = 2;
				} else {
					_can_index_face = 1;
				}
			} else {
				_can_index_face = 1;
			}
		}
		return _can_index_face == 2;
	}

protected:
	static void _bind_methods();

public:

	void clear();
	void copy_to(const Ref<RayCastResult> &p_destination) const;

	RayCastResult();
};

#undef JOLT_ALLOWS_RAYCAST_FACE_INDEX
#undef IS_USING_JOLT

#endif // PHYSICS_3D_DISABLED
