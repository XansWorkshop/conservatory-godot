/**************************************************************************/
/*  object_id.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
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

#pragma once

#include "core/templates/hashfuncs.h"
#include "core/typedefs.h"

// Class to store an object ID (int64)
// needs to be compatile with int64 because this is what Variant uses
// Also, need to be explicitly only castable to 64 bits integer types
// to avoid bugs due to loss of precision


// This needs to add up to 63, 1 bit is for reference.
// Added by Xan 2026: I need a bit for custom IDs, so take one from the validator.
// Also moved from object.h because I need it here for is_ref_counted
#define OBJECTDB_VALIDATOR_BITS 38 // Was 39 in base Godot
#define OBJECTDB_VALIDATOR_MASK ((uint64_t(1) << OBJECTDB_VALIDATOR_BITS) - 1)
#define OBJECTDB_SLOT_MAX_COUNT_BITS 24
#define OBJECTDB_SLOT_MAX_COUNT_MASK ((uint64_t(1) << OBJECTDB_SLOT_MAX_COUNT_BITS) - 1)
#define OBJECTDB_REFERENCE_BIT (uint64_t(1) << (OBJECTDB_SLOT_MAX_COUNT_BITS + OBJECTDB_VALIDATOR_BITS + 1))
#define OBJECTDB_TC_CUSTOM_BIT (uint64_t(1) << (OBJECTDB_SLOT_MAX_COUNT_BITS + OBJECTDB_VALIDATOR_BITS + 0))

class ObjectID {
	uint64_t id = 0;

public:
	_ALWAYS_INLINE_ bool is_ref_counted() const { return (id & OBJECTDB_REFERENCE_BIT) != 0; }
	_ALWAYS_INLINE_ bool is_valid() const { return id != 0; }
	_ALWAYS_INLINE_ bool is_null() const { return id == 0; }
	_ALWAYS_INLINE_ operator uint64_t() const { return id; }
	_ALWAYS_INLINE_ operator int64_t() const { return (int64_t)id; }

	_ALWAYS_INLINE_ bool operator==(const ObjectID &p_id) const { return id == p_id.id; }
	_ALWAYS_INLINE_ bool operator!=(const ObjectID &p_id) const { return id != p_id.id; }
	_ALWAYS_INLINE_ bool operator<(const ObjectID &p_id) const { return id < p_id.id; }

	_ALWAYS_INLINE_ void operator=(int64_t p_int64) { id = p_int64; }
	_ALWAYS_INLINE_ void operator=(uint64_t p_uint64) { id = p_uint64; }

	uint32_t hash() const { return HashMapHasherDefault::hash(id); }

	_ALWAYS_INLINE_ ObjectID() {}
	_ALWAYS_INLINE_ explicit ObjectID(const uint64_t p_id) { id = p_id; }
	_ALWAYS_INLINE_ explicit ObjectID(const int64_t p_id) { id = p_id; }
};

template <>
struct is_zero_constructible<ObjectID> : std::true_type {};
