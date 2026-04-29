/**************************************************************************/
/*  point_query_result.cpp                                                */
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

#include "point_query_result.h"
#include "core/object/class_db.h"

void PointQueryResult::clear() {
	collider = nullptr;
	collider_id = ObjectID();
	rid = RID();
	shape = 0;
}

void PointQueryResult::copy_to(Ref<PointQueryResult> &p_other) const {
	p_other->collider = collider;
	p_other->collider_id = collider_id;
	p_other->rid = rid;
	p_other->shape = shape;
}

void PointQueryResult::_bind_methods() {
	/*
	ClassDB::bind_method(D_METHOD("get_collider"), &PointQueryResult::get_collider);
	ClassDB::bind_method(D_METHOD("set_collider", "collider"), &PointQueryResult::set_collider);
	ClassDB::bind_method(D_METHOD("get_collider_id"), &PointQueryResult::get_collider_id);
	ClassDB::bind_method(D_METHOD("set_collider_id", "id"), &PointQueryResult::set_collider_id);
	ClassDB::bind_method(D_METHOD("get_rid"), &PointQueryResult::get_rid);
	ClassDB::bind_method(D_METHOD("set_rid", "rid"), &PointQueryResult::set_rid);
	ClassDB::bind_method(D_METHOD("get_shape"), &PointQueryResult::get_shape);
	ClassDB::bind_method(D_METHOD("set_shape", "shape"), &PointQueryResult::set_shape);

	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::OBJECT, "hit_godot_object"), "set_collider", "get_collider");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_object_id"), "set_collider_id", "get_collider_id");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::RID, "hit_rid"), "set_rid", "get_rid");
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, "hit_shape_index"), "set_shape", "get_shape");
	*/
	XT_AUTO_BIND_INITONLY_PROPERTY(PointQueryResult, collider, Variant::OBJECT);
	XT_AUTO_BIND_INITONLY_PROPERTY(PointQueryResult, collider_id, Variant::INT);
	XT_AUTO_BIND_INITONLY_PROPERTY(PointQueryResult, rid, Variant::RID);
	XT_AUTO_BIND_INITONLY_PROPERTY(PointQueryResult, shape, Variant::INT);
}

PointQueryResult::PointQueryResult() {
	clear();
}
