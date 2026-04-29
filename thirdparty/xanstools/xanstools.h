/**************************************************************************/
/*  material.h                                                            */
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
#include "core/templates/hash_set.h"

// Declares a private field with the provided name, then a get and set method for that field.
#define XT_AUTO_PROPERTY(type, name) \
private: \
	type name; \
\
public: \
	void set_##name(const type &p_value); \
	type get_##name() const

// Declares a private field with the provided name, then a get and set method for that field.
#define XT_AUTO_PROPERTY_INLINE(type, name) \
private: \
	type name; \
\
public: \
	void set_##name(type &p_value) { \
		name = p_value; \
	}; \
	type get_##name() const { \
		return name; \
	}

// Declares a private field with the provided name, then a get and set method for that field.
// This has a const input value.
#define XT_AUTO_PROPERTY_INLINE_C(type, name) \
private: \
	type name; \
\
public: \
	void set_##name(const type &p_value) { \
		name = p_value; \
	}; \
	type get_##name() const { \
		return name; \
	}

// Declares a private field with the provided name, then a get and set method for that field.
// This has a direct (non-ref, non-pointer) type. The type can be entered as a pointer type if desired.
#define XT_AUTO_PROPERTY_INLINE_D(type, name) \
private: \
	type name; \
\
public: \
	void set_##name(type p_value) { \
		name = p_value; \
	} \
	type get_##name() const { \
		return name; \
	}

// Declares a private field with the provided name, then a get and set method for that field.
// This has a pointer type.
#define XT_AUTO_PROPERTY_INLINE_P(type, name) \
private: \
	type* name; \
\
public: \
	void set_##name(type* p_value) { \
		name = p_value; \
	} \
	type* get_##name() const { \
		return name; \
	}

// Declares a private field with the provided name, then a get and set method for that field.
// This has a const direct (non-ref, non-pointer) type. The type can be entered as a pointer type if desired.
#define XT_AUTO_PROPERTY_INLINE_CD(type, name) \
private: \
	type name; \
\
public: \
	void set_##name(const type p_value) { \
		name = p_value; \
	} \
	const type get_##name() const { \
		return name; \
	}

// Automatically binds the getter and setter for a property with the provided name to the classDB, then registers a property.
#define XT_AUTO_BIND_PROPERTY(this_class, name, variant_type) \
	ClassDB::bind_method(D_METHOD("get_" #name), &this_class::get_##name); \
	ClassDB::bind_method(D_METHOD("set_" #name, #name), &this_class::set_##name); \
	ADD_PROPERTY(PropertyInfo(variant_type, #name), "set_" #name, "get_" #name)

// Automatically binds the getter and setter for a property with the provided name to the classDB, then registers a property.
#define XT_AUTO_BIND_INITONLY_PROPERTY(this_class, name, variant_type) \
	ClassDB::bind_method(D_METHOD("get_" #name), &this_class::get_##name); \
	ClassDB::bind_method(D_METHOD("set_" #name, #name), &this_class::set_##name); \
	ADD_INITONLY_PROPERTY(PropertyInfo(variant_type, #name), "set_" #name, "get_" #name)


struct XansTools {

	// Adds an item to the provided HashSet, returning true if the value was newly added, or false if it already existed.
	template <typename T>
	static bool hashset_add(HashSet<T> &p_hashset, T p_item) {
		uint32_t old_size = p_hashset.size();
		p_hashset.insert(p_item);
		return p_hashset.size() != old_size;
	}

	// Copies the contents of the provided vector into the typed array.
	template <typename T>
	static void vector_to_typed_array(Vector<Ref<T>> &p_src, TypedArray<T> &p_dst) {
		p_dst.resize(p_src.size());
		for (int i = 0; i < p_src.size(); ++i) {
			p_dst[i] = p_src[i];
		}
	}

	// Copies the contents of the provided typed array into the vector.
	template <typename T>
	static void typed_array_to_vector(TypedArray<T> &p_src, Vector<Ref<T>> &p_dst) {
		p_dst.resize(p_src.size());
		for (int i = 0; i < p_src.size(); ++i) {
			p_dst[i] = p_src[i];
		}
	}
};
