/**************************************************************************/
/*  color_name_injector.h                                                 */
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

// Used to add custom named colors from C#.


#include "core/object/object.h"
#include "core/object/class_db.h"

class ColorNameInjector : public Object {
	GDCLASS(ColorNameInjector, Object);

protected:
	static void _bind_methods() {
		ClassDB::bind_static_method(ColorNameInjector::get_class_static(), D_METHOD("set_color_by_name_ptr", "ptr"), ColorNameInjector::set_color_by_name_ptr);
	}

public:
	static bool (*cs_color_by_name)(const uint8_t* utf8, int32_t length, float* rgba);

	static void set_color_by_name_ptr(int64_t p_ptr) {
		cs_color_by_name = (bool(*)(const uint8_t*, int32_t, float*))p_ptr;
	}

	static bool get_color_by_name(const String& p_name, float* p_color) {
		if (cs_color_by_name) {
			PackedByteArray chars = p_name.to_utf8_buffer();
			return cs_color_by_name(chars.ptr(), chars.size(), p_color);
		}
		return false;
	}
};
