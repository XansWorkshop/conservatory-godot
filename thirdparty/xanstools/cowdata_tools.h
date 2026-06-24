/**************************************************************************/
/*  cowdata_tools.h                                                       */
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
#include "core/object/class_db.h"
#include "core/templates/cowdata.h"

class CowDataTools : public Object {
	GDCLASS(CowDataTools, Object);

protected:
	static void _bind_methods() {
		ClassDB::bind_static_method(CowDataTools::get_class_static(), D_METHOD("extract_offsets_ptr", "refcount_offset", "capacity_offset", "size_offset", "data_offset"), CowDataTools::extract_offsets_ptr);
	}

public:
	static void extract_offsets_ptr(int64_t p_refcount_offset_ptr, int64_t p_capacity_offset_ptr, int64_t p_size_offset_ptr, int64_t p_data_offset_ptr) {
		*(uint64_t *)(p_refcount_offset_ptr) = CowData<void>::REF_COUNT_OFFSET;
		*(uint64_t *)(p_capacity_offset_ptr) = CowData<void>::CAPACITY_OFFSET;
		*(uint64_t *)(p_size_offset_ptr) = CowData<void>::SIZE_OFFSET;
		*(uint64_t *)(p_data_offset_ptr) = CowData<void>::DATA_OFFSET;
	}
};
