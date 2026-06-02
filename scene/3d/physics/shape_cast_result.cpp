/**************************************************************************/
/*  shape_cast_result.cpp                                                 */
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

#include "shape_cast_result.h"

void ShapeCastResult::clear() {
	hit_something = false;
	intersection_point = Vector3();
	intersection_normal = Vector3();
	rid = RID();
	hit_object_id = ObjectID();
	hit_object = nullptr;
	shape_index = -1;
	linear_velocity_at_contact = Vector3();
	collision_safe_fraction = 0;
	collision_unsafe_fraction = 0;
}

void ShapeCastResult::copy_to(const Ref<ShapeCastResult>& p_destination) const {
	p_destination->hit_something = hit_something;
	p_destination->intersection_point = intersection_point;
	p_destination->intersection_normal = intersection_normal;
	p_destination->rid = rid;
	p_destination->hit_object_id = hit_object_id;
	p_destination->hit_object = hit_object;
	p_destination->shape_index = shape_index;
	p_destination->linear_velocity_at_contact = linear_velocity_at_contact;
	p_destination->collision_safe_fraction = collision_safe_fraction;
	p_destination->collision_unsafe_fraction = collision_unsafe_fraction;
}

void ShapeCastResult::_bind_methods() {
	XT_AUTO_BIND_INITONLY_PROPERTY_SPECIAL_OBJECTID(ShapeCastResult, hit_object);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeCastResult, hit_something, Variant::BOOL);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeCastResult, intersection_point, Variant::VECTOR3);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeCastResult, intersection_normal, Variant::VECTOR3);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeCastResult, rid, Variant::RID);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeCastResult, shape_index, Variant::INT);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeCastResult, linear_velocity_at_contact, Variant::VECTOR3);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeCastResult, collision_safe_fraction, Variant::FLOAT);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeCastResult, collision_unsafe_fraction, Variant::FLOAT);
	
	ClassDB::bind_method(D_METHOD("clear"), &ShapeCastResult::clear);
	ClassDB::bind_method(D_METHOD("copy_to", "destination"), &ShapeCastResult::copy_to);
}

ShapeCastResult::ShapeCastResult() {
	clear();
}

#endif // PHYSICS_3D_DISABLED
