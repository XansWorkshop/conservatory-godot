/**************************************************************************/
/*  shape_query_result.cpp                                                */
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

#include "shape_query_result.h"
#include "core/object/class_db.h"

void ShapeQueryResult::clear() {
	collider = nullptr;
	collider_id = ObjectID();
	rid = RID();
	shape = -1;
}
void ShapeQueryResult::copy_to(Ref<ShapeQueryResult> &p_other) const {
	p_other->collider = collider;
	p_other->collider_id = collider_id;
	p_other->rid = rid;
	p_other->shape = shape;
}

void ShapeQueryResult::_bind_methods() {
	XT_AUTO_BIND_INITONLY_PROPERTY_SPECIAL_OBJECTID(ShapeQueryResult, collider);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeQueryResult, rid, Variant::RID);
	XT_AUTO_BIND_INITONLY_PROPERTY(ShapeQueryResult, shape, Variant::INT);
}

ShapeQueryResult::ShapeQueryResult() {
	clear();
}
