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

#include "ray_cast_result.h"

uint8_t RayCastResult::_can_index_face = 0;

int RayCastResult::get_face_index() const {
	if (face_index < 0 && hit_something) {
		if (!can_index_face()) {
			ERR_FAIL_V_MSG(-1, "The physics engine is set to Jolt Physics, and the current configuration skips looking for which face got hit; get_face_index() is not usable at this time.");
		}
	}
	return face_index;
}

void RayCastResult::set_face_index(int p_face_index) {
	face_index = p_face_index;
}

void RayCastResult::clear() {
	origin = Vector3();
	hit_position = Vector3();
	hit_normal = Vector3();
	rid = RID();
	hit_object_id = ObjectID();
	hit_object = nullptr;
	shape_index = -1;
	face_index = -1;
	hit_something = false;
}

void RayCastResult::copy_to(const Ref<RayCastResult> &p_destination) const {
	ERR_FAIL_COND_MSG(p_destination.is_null(), "The provided destination object is null.");
	p_destination->origin = origin;
	p_destination->hit_position = hit_position;
	p_destination->hit_normal = hit_normal;
	p_destination->rid = rid;
	p_destination->hit_object_id = hit_object_id;
	p_destination->hit_object = hit_object;
	p_destination->shape_index = shape_index;
	p_destination->face_index = face_index;
	p_destination->hit_something = hit_something;
}

void RayCastResult::_bind_methods() {
	XT_AUTO_BIND_INITONLY_PROPERTY_SPECIAL_OBJECTID(RayCastResult, hit_object);
	XT_AUTO_BIND_INITONLY_PROPERTY(RayCastResult, hit_something, Variant::BOOL);
	XT_AUTO_BIND_INITONLY_PROPERTY(RayCastResult, hit_position, Variant::VECTOR3);
	XT_AUTO_BIND_INITONLY_PROPERTY(RayCastResult, hit_normal, Variant::VECTOR3);
	XT_AUTO_BIND_INITONLY_PROPERTY(RayCastResult, rid, Variant::RID);
	XT_AUTO_BIND_INITONLY_PROPERTY(RayCastResult, shape_index, Variant::INT);
	XT_AUTO_BIND_INITONLY_PROPERTY(RayCastResult, face_index, Variant::INT);
	XT_AUTO_BIND_INITONLY_PROPERTY(RayCastResult, origin, Variant::VECTOR3);

	ClassDB::bind_method(D_METHOD("clear"), &RayCastResult::clear);
	ClassDB::bind_method(D_METHOD("copy_to", "destination"), &RayCastResult::copy_to);
}

RayCastResult::RayCastResult() {
	clear();
}

#endif // PHYSICS_3D_DISABLED
