/**************************************************************************/
/*  shader_variant_metadata.h                                             */
/**************************************************************************/
/*                         This file is part of:                          */
/*                 GODOT ENGINE /// THE CONSERVATORY FORK                 */
/*          https://godotengine.org /// https://xansworkshop.com          */
/**************************************************************************/
/*                     DERIVED FROM GODOT SOURCE CODE                     */
/*                       SEE ORIGINAL LICENSE BELOW                       */
/**************************************************************************/
/* Copyright (c) 2026-present Xan's Workshop.                             */
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

#if false
#pragma once

#include "core/io/resource.h"
#include "core/templates/hash_map.h"
#include "core/variant/typed_array.h"

class ShaderVariantMetadata : public Resource {
	GDCLASS(ShaderVariantMetadata, Resource);

	friend class ShaderVariantMaterial;

private:
	StringName definition;

	// False: This is a feature (toggle)
	// True: This is a variant (set of values)
	bool is_variant;

	// The default state, if this is a feature. Ignored if this is a variant.
	bool feature_default_state;

	// The default index, if this is a variant. Ignored if this is a feature.
	int variant_default_index;

	// The allowed values, if this is a variant. Ignored if this is a feature.
	TypedArray<StringName> valid_variants;

	struct State {
		bool feature_state;
		int variant_index;
	} _state;

protected:

	static void _bind_methods();
	static void validate_definition(StringName *p_definition);

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	bool _property_can_revert(const StringName &p_name) const;
	bool _property_get_revert(const StringName &p_name, Variant &r_property) const;

public:
	StringName get_definition() const;
	void set_definition(const StringName &p_definition);

	bool get_is_variant() const;
	void set_is_variant(bool p_is_variant);

	TypedArray<StringName> get_valid_variants() const;
	void set_valid_variants(TypedArray<StringName> &p_valid_variants);

	bool get_default_feature_state() const;
	void set_default_feature_state(bool p_default);

	int get_default_variant_index() const;
	void set_default_variant_index(int p_default);

	_FORCE_INLINE_ void reset_state() {
		_state.feature_state = false;
		_state.variant_index = 0;
	}

	Ref<ShaderVariantMetadata> copy() const;

	ShaderVariantMetadata();
	~ShaderVariantMetadata();
};
#endif
