/**************************************************************************/
/*  shader_variant_metadata.cpp                                           */
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

#include "shader_variant_metadata.h"
#include "core/object/class_db.h"

void ShaderVariantMetadata::validate_definition(StringName *p_definition, bool make_uppercase) {
	StringName new_variant = SNAME("NAME");
	if (p_definition->is_empty()) {
		*p_definition = new_variant;
	} else {
		String def_clone = (String)*p_definition;
		bool is_first_char = true;
		bool changes_occurred = false;
		for (int i = 0; i < def_clone.length(); i++) {
			char32_t chr_val = def_clone.get(i);
			bool is_az_capital = chr_val >= 'A' && chr_val <= 'Z';
			bool is_az_lower = chr_val >= 'a' && chr_val <= 'z';
			bool is_zero_nine = chr_val >= '0' && chr_val <= '9';
			if (is_az_lower) {
				if (make_uppercase) {
					def_clone.set(i, chr_val - 32); // Capitalize
				}
			} else if (is_az_capital) {
				if (!make_uppercase) {
					def_clone.set(i, chr_val + 32); // Uncapitalize. Lowerize?
				}
			} else if (is_zero_nine) {
				if (unlikely(is_first_char)) {
					// ^ Only the first char is the first char. It will never happen again.
					def_clone.set(i, '_');
					changes_occurred = true;
				}
			} else if (chr_val != '_') {
				def_clone.set(i, '_');
				changes_occurred = true;
			}
			is_first_char = false;
		}

		if (changes_occurred) {
			*p_definition = StringName(def_clone);
		}
	}
}

StringName ShaderVariantMetadata::get_definition() const {
	return definition;
}

void ShaderVariantMetadata::set_definition(const StringName &p_definition) {
	StringName def = p_definition;
	validate_definition(&def, true);
	definition = def;
	// No property list change!
	emit_changed();
}

bool ShaderVariantMetadata::get_is_variant() const {
	return is_variant;
}

void ShaderVariantMetadata::set_is_variant(bool p_is_variant) {
	if (p_is_variant == is_variant) return;
	is_variant = p_is_variant;
	notify_property_list_changed();
	emit_changed();
}

TypedArray<StringName> ShaderVariantMetadata::get_valid_variants() const {
	return valid_variants.duplicate();
}
TypedArray<StringName> ShaderVariantMetadata::_get_valid_variants_direct() const {
	return valid_variants;
}
void ShaderVariantMetadata::set_valid_variants(const TypedArray<StringName>& p_valid_variants) {
	TypedArray<StringName> result = TypedArray<StringName>();
	result.reserve(p_valid_variants.size());

	// Try to retain the same default.
	StringName current_default = StringName();
	if (variant_default_index >= 0 && variant_default_index < valid_variants.size()) {
		current_default = valid_variants[variant_default_index];
	}

	// no & - this is a copy from a Variant and so removing this makes it explicit.
	for (const StringName current_name : p_valid_variants) {
		StringName real_name = current_name;
		validate_definition(&real_name, true);
		result.push_back(real_name);
	}

	variant_default_index = result.find(current_default);
	if (variant_default_index == -1) {
		variant_default_index = 0;
	}

	valid_variants = result;
	// notify_property_list_changed(); // Causes annoying issues, even though this is technically required...
	emit_changed();
}

bool ShaderVariantMetadata::get_default_feature_state() const {
	return feature_default_state;
}
void ShaderVariantMetadata::set_default_feature_state(bool p_default) {
	if (feature_default_state == p_default) {
		return;
	}
	feature_default_state = p_default;
	// No property list change!
	emit_changed();
}

int ShaderVariantMetadata::get_default_variant() const {
	return variant_default_index;
}
void ShaderVariantMetadata::set_default_variant(int p_default) {
	if (p_default >= valid_variants.size()) {
		p_default = valid_variants.size() - 1;
	}
	// Do the <0 thing after because of the -1 above.
	if (p_default < 0) {
		p_default = 0;
	}
	if (variant_default_index == p_default) {
		return;
	}
	variant_default_index = p_default;
	// No property list change!
	emit_changed();
}

bool ShaderVariantMetadata::_set(const StringName& p_name, const Variant& p_value) {
	if (p_name == "default_feature_state") {
		set_default_feature_state(p_value.booleanize());
		return true;
	} else if (p_name == "default_variant") {
		if (p_value.is_num()) {
			set_default_variant(p_value);
			return true;
		}
	} else if (p_name == "valid_variants") {
		if (p_value.is_array()) {
			Array value_as_array = p_value;
			TypedArray<StringName> values = TypedArray<StringName>();
			values.reserve(value_as_array.size());
			for (const Variant &var : value_as_array) {
				if (var.is_string()) {
					values.push_back(var);
				}
			}
			set_valid_variants(values);
			return true;
		}
	}
	return false;
}
bool ShaderVariantMetadata::_get(const StringName& p_name, Variant& r_ret) const {
	if (p_name == "default_feature_state") {
		r_ret = get_default_feature_state();
		return true;
	} else if (p_name == "default_variant") {
		r_ret = get_default_variant();
		return true;
	} else if (p_name == "valid_variants") {
		r_ret = get_valid_variants();
		return true;
	}
	return false;
}
void ShaderVariantMetadata::_get_property_list(List<PropertyInfo> *p_list) const {

	// Features:

	p_list->push_back(PropertyInfo(
		Variant::BOOL,
		"default_feature_state",
		PROPERTY_HINT_NONE,
		"",
		is_variant ? PROPERTY_USAGE_NO_EDITOR : PROPERTY_USAGE_DEFAULT // Still saves either way, but only shows if false
	));

	// Variants:

	p_list->push_back(PropertyInfo(
		Variant::PACKED_STRING_ARRAY,
		"valid_variants",
		PROPERTY_HINT_NONE,
		"",
		is_variant ? PROPERTY_USAGE_DEFAULT : PROPERTY_USAGE_NO_EDITOR
	));

	// Build the enum suggestions string for the property.
	String variant_index_suggestions = String();

	// no & - this is a copy from a Variant and so removing this makes it explicit.
	for (const StringName valid : valid_variants) {
		if (likely(variant_index_suggestions.length() != 0)) {
			variant_index_suggestions += ',';
		}
		variant_index_suggestions += valid;
	}
	p_list->push_back(PropertyInfo(
		Variant::INT,
		"default_variant",
		PROPERTY_HINT_ENUM,
		variant_index_suggestions,
		is_variant ? PROPERTY_USAGE_DEFAULT : PROPERTY_USAGE_NO_EDITOR
	));
}
bool ShaderVariantMetadata::_property_can_revert(const StringName& p_name) const {
	return false;
}
bool ShaderVariantMetadata::_property_get_revert(const StringName& p_name, Variant& r_property) const {
	return false;
}

void ShaderVariantMetadata::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_definition"), &ShaderVariantMetadata::get_definition);
	ClassDB::bind_method(D_METHOD("set_definition", "definition"), &ShaderVariantMetadata::set_definition);
	ClassDB::bind_method(D_METHOD("get_is_variant"), &ShaderVariantMetadata::get_is_variant);
	ClassDB::bind_method(D_METHOD("set_is_variant", "is_variant"), &ShaderVariantMetadata::set_is_variant);
	ClassDB::bind_method(D_METHOD("get_valid_variants"), &ShaderVariantMetadata::get_valid_variants);
	ClassDB::bind_method(D_METHOD("set_valid_variants", "valid_variants"), &ShaderVariantMetadata::set_valid_variants);
	ClassDB::bind_method(D_METHOD("get_default_feature_state"), &ShaderVariantMetadata::get_default_feature_state);
	ClassDB::bind_method(D_METHOD("set_default_feature_state", "state"), &ShaderVariantMetadata::set_default_feature_state);
	ClassDB::bind_method(D_METHOD("get_default_variant"), &ShaderVariantMetadata::get_default_variant);
	ClassDB::bind_method(D_METHOD("set_default_variant", "index"), &ShaderVariantMetadata::set_default_variant);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "definition"), "set_definition", "get_definition");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_variant"), "set_is_variant", "get_is_variant");
}

ShaderVariantMetadata::ShaderVariantMetadata() {
	definition = SNAME("NEW_VARIANT");
	feature_default_state = false;
	variant_default_index = 0;
	is_variant = false;
	valid_variants = TypedArray<StringName>();
}
ShaderVariantMetadata::~ShaderVariantMetadata() {
	valid_variants.clear();
}
