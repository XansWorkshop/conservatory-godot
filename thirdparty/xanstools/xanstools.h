/**************************************************************************/
/*  xanstools.h                                                           */
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

// Declares a private field with the provided name, then a get and set method for that field (setter is by reference).
// This has a const input value. This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY(int, value) = 0;
#define XT_AUTO_PROPERTY(type, name) \
public: \
	void set_##name(type &p_##name); \
	type get_##name() const; \
private: \
	type name

// Declares a private field with the provided name, then a get and set method for that field (setter is by reference).
// This has a const input value. This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY_C(int, value) = 0;
// "C" for "Constant"
#define XT_AUTO_PROPERTY_C(type, name) \
public: \
	void set_##name(const type &p_##name); \
	type get_##name() const; \
private: \
	type name

// Declares a private field with the provided name, then a get and set method for that field.
// This has a const input value. This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY_D(int, value) = 0;
// "D" for "Direct" - the type is not by reference.
#define XT_AUTO_PROPERTY_D(type, name) \
public: \
	void set_##name(type p_##name); \
	type get_##name() const; \
private: \
	type name

// Declares a private field with the provided name, then a get and set method for that field.
// This has a const input value. This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY_D(int, value) = 0;
// "D" for "Direct" - the type is not by reference.
// "C" for "Constant"
#define XT_AUTO_PROPERTY_DC(type, name) \
public: \
	void set_##name(const type p_##name); \
	type get_##name() const; \
private: \
	type name

// Declares a private field with the provided name, then a get and set method for that field (setter is by reference).
// This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY_INLINE(int, value) = 0;
#define XT_AUTO_PROPERTY_INLINE(type, name) \
public: \
	void set_##name(type &p_##name) { \
		name = p_##name; \
	} \
	type get_##name() const { \
		return name; \
	} \
private: \
	type name

// Declares a private field with the provided name, then a get and set method for that field (setter is by reference).
// This has a const input value. This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY_INLINE_C(int, value) = 0;
// "C" for "Constant"
#define XT_AUTO_PROPERTY_INLINE_C(type, name) \
public: \
	void set_##name(const type &p_##name) { \
		name = p_##name; \
	} \
	type get_##name() const { \
		return name; \
	} \
private: \
	type name

// Declares a private field with the provided name, then a get and set method for that field.
// This has a direct (non-ref, non-pointer) type. The type can be entered as a pointer type if desired.
// This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY_INLINE_D(int, value) = 0;
// "D" for "Direct" - the type is not by reference.
#define XT_AUTO_PROPERTY_INLINE_D(type, name) \
public: \
	void set_##name(type p_##name) { \
		name = p_##name; \
	} \
	type get_##name() const { \
		return name; \
	} \
private: \
	type name

// Declares a private field with the provided name, then a get and set method for that field.
// This has a const direct (non-ref, non-pointer) type.
// This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY_INLINE_DC(int, value) = 0;
// "D" for "Direct" - the type is not by reference.
// "C" for "Constant"
#define XT_AUTO_PROPERTY_INLINE_DC(type, name) \
public: \
	void set_##name(const type p_##name) { \
		name = p_##name; \
	} \
	const type get_##name() const { \
		return name; \
	} \
private: \
	type name

// Declares a private field with the provided name, then a get and set method for that field.
// This has a pointer type.
// This macro is designed to be used as the left hand side; you can set it equal to the default value.
// For example: XT_AUTO_PROPERTY_INLINE_P(int, value) = nullptr;
// "P" for "Pointer"
#define XT_AUTO_PROPERTY_INLINE_P(type, name) \
public: \
	void set_##name(type *p_##name) { \
		name = p_##name; \
	} \
	type *get_##name() const { \
		return name; \
	} \
private: \
	type *name

// A special variation of XT_AUTO_PROPERTY* which implements methods for handling ObjectID.
// In particular, this creates two internal methods which get and set the object ID directly,
// and then two more methods which return int64 instead for export.
#define XT_AUTO_PROPERTY_SPECIAL_OBJECTID(name) \
public: \
	void set_##name (const ObjectID &p_##name) { \
		name = p_##name; \
	} \
	ObjectID get_##name() const { \
		return name; \
	} \
\
private: \
	void _set_##name(int64_t p_##name) { \
		name = ObjectID(p_##name); \
	} \
	int64_t _get_##name() const { \
		return (int64_t)name; \
	} \
	ObjectID name = ObjectID()

// A special variation of XT_AUTO_PROPERTY* which implements methods for handling ObjectID.
// In particular, this creates two internal methods which get and set the object ID directly,
// and then two more methods which return int64 instead for export.
// This additional method also causes the user-facing setter to set the hit object (denoted by object_field_name)
// when setting the int ID. The internal method does not do this.
#define XT_AUTO_PROPERTY_SPECIAL_OBJECTID_WITH_OBJECT(name) \
public: \
	void set_##name##_id(const ObjectID &p_##name##_id) { \
		name##_id = p_##name##_id; \
	} \
	ObjectID get_##name##_id() const { \
		return name##_id; \
	} \
	void set_##name(Object *p_##name) { \
		name = p_##name; \
	} \
	Object *get_##name() const { \
		return name; \
	} \
\
private: \
	void _set_##name##_id(int64_t p_##name##_id) { \
		name##_id = ObjectID(p_##name##_id); \
		name = ObjectDB::get_instance(name##_id); \
	} \
	int64_t _get_##name##_id() const { \
		return (int64_t)name##_id; \
	} \
	ObjectID name##_id = ObjectID(); \
	Object *name = nullptr

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

// Automatically binds the getter for a read-only property with the provided name to the classDB, then registers a property.
#define XT_AUTO_BIND_READONLY_PROPERTY(this_class, name, variant_type) \
	ClassDB::bind_method(D_METHOD("get_" #name), &this_class::get_##name); \
	ADD_READONLY_PROPERTY(PropertyInfo(variant_type, #name), "get_" #name)

// Same as XT_AUTO_BIND_PROPERTY, but this binds the special prefix methods for ObjectID.
#define XT_AUTO_BIND_PROPERTY_SPECIAL_OBJECTID(this_class, name) \
	ClassDB::bind_method(D_METHOD("get_" #name "_id"), &this_class::_get_##name##_id); \
	ClassDB::bind_method(D_METHOD("set_" #name "_id", #name "_id"), &this_class::_set_##name##_id); \
	ADD_PROPERTY(PropertyInfo(Variant::INT, #name "_id"), "set_" #name "_id", "get_" #name "_id"); \
	XT_AUTO_BIND_READONLY_PROPERTY(this_class, name, Variant::OBJECT)
// ^ Always readonly

// Same as XT_AUTO_BIND_INITONLY_PROPERTY, but this binds the special prefix methods for ObjectID.
#define XT_AUTO_BIND_INITONLY_PROPERTY_SPECIAL_OBJECTID(this_class, name) \
	ClassDB::bind_method(D_METHOD("get_" #name "_id"), &this_class::_get_##name##_id); \
	ClassDB::bind_method(D_METHOD("set_" #name "_id", #name "_id"), &this_class::_set_##name##_id); \
	ADD_INITONLY_PROPERTY(PropertyInfo(Variant::INT, #name "_id"), "set_" #name "_id", "get_" #name "_id"); \
	XT_AUTO_BIND_READONLY_PROPERTY(this_class, name, Variant::OBJECT)
// ^ Always readonly

// Same as XT_AUTO_BIND_PROPERTY, but this binds the special prefix methods for ObjectID.
#define XT_AUTO_BIND_READONLY_PROPERTY_SPECIAL_OBJECTID(this_class, name) \
	ClassDB::bind_method(D_METHOD("get_" #name "_id"), &this_class::_get_##name##_id); \
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::INT, #name "_id"), "get_" #name "_id"); \
	XT_AUTO_BIND_READONLY_PROPERTY(this_class, name, Variant::OBJECT)
// ^ Always readonly


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
