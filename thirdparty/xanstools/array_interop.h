/**************************************************************************/
/*  array_interop.h                                                       */
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
#include "core/templates/vector.h"
#include "core/variant/array.h"
#include "core/variant/typed_array.h"

class XanArrayInterop {
public:

	// Converts a Vector<Ref<T>> into a TypedArray<T>. If r_modify_this is not null, it will be modified rather than allocating a new instance.
	template <typename T>
	static TypedArray<T> vector_to_typed_array(const Vector<Ref<T>> &p_vector, const TypedArray<T> *r_modify_this = nullptr) {
		if (r_modify_this) {
			r_modify_this->clear();
			r_modify_this->resize(p_vector.size());
			for (const Ref<T> &reference : p_vector) {
				r_modify_this->push_back(reference);
			}
			return *r_modify_this;
		} else {
		}
	}

	// Converts a TypedArray<T> into a Vector<Ref<T>>. If r_modify_this is not null, it will be modified rather than allocating a new instance.
	template <typename T>
	static Vector<Ref<T>> typed_array_to_vector(const TypedArray<T> &p_array, const Vector<Ref<T>> *r_modify_this = nullptr) {
		if (r_modify_this) {
			r_modify_this->clear();
			r_modify_this->resize_uninitialized(p_array.size());
			for (const Variant &reference : p_array) {
				Ref<T> stored;
				stored.reference_ptr((Object *)reference);
				r_modify_this->push_back(stored);
			}
			return *r_modify_this;
		} else {
			Vector<Ref<T>> result;
			result.resize_uninitialized(p_array.size());
			for (const Variant &reference : p_array) {
				Ref<T> stored;
				stored.reference_ptr((Object *)reference);
				result.push_back(stored);
			}
			return result;
		}
	}
};
