/**************************************************************************/
/*  shape_cast_result.h                                                   */
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

class ShapeCastResult : public RefCounted {
	GDCLASS(ShapeCastResult, RefCounted);

private:
	XT_AUTO_PROPERTY_SPECIAL_OBJECTID_WITH_OBJECT(hit_object);
	XT_AUTO_PROPERTY_INLINE_DC(bool, hit_something) = false;
	XT_AUTO_PROPERTY_INLINE_C(Vector3, intersection_point) = Vector3();
	XT_AUTO_PROPERTY_INLINE_C(Vector3, intersection_normal) = Vector3();
	XT_AUTO_PROPERTY_INLINE_C(RID, rid) = RID();
	XT_AUTO_PROPERTY_INLINE_C(Vector3, linear_velocity_at_contact) = Vector3();
	XT_AUTO_PROPERTY_INLINE_DC(real_t, collision_safe_fraction) = 0.0f;
	XT_AUTO_PROPERTY_INLINE_DC(real_t, collision_unsafe_fraction) = 0.0f;
	XT_AUTO_PROPERTY_INLINE_DC(int, shape_index) = -1;

protected:
	static void _bind_methods();

public:
	void clear();
	void copy_to(const Ref<ShapeCastResult> &p_destination) const;

	ShapeCastResult();
};

#endif // PHYSICS_3D_DISABLED
