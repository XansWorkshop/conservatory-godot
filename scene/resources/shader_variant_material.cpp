/**************************************************************************/
/*  shader_variant_material.cpp                                           */
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

#include "shader_variant_material.h"
#include "core/config/engine.h"
#include "core/error/error_macros.h"
#include "core/object/callable_mp.h"
#include "core/object/class_db.h"
#include "servers/rendering/rendering_server.h"


void ShaderVariantMaterial::_set_modified_shader_from_current() {
	if (base_shader.is_null()) {
		return;
	}
	Ref<Shader> new_shader;

	String new_source = String("// AUTO-GENERATED CODE BLOCK FROM SHADER VARIANT //\n");
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->is_variant) {
			int variant_index = data->_state.variant_index;
			if (variant_index >= 0 && variant_index < data->valid_variants.size()) {
				StringName term = data->valid_variants[variant_index];
				new_source += "#define " + data->definition + "_" + term + " 1\n";
			}
		} else {
			if (data->_state.feature_state) {
				new_source += "#define " + data->definition + " 1\n";
			}
		}
	}
	new_source += base_shader_source;

	new_shader.instantiate();
	new_shader->set_include_path(base_shader->get_include_path());
	new_shader->set_code(new_source);

	// Use base behavior to change the shader to this modified variation.
	set_shader_no_change_logic(new_shader);
}


void ShaderVariantMaterial::set_shader_no_change_logic(const Ref<Shader> &p_shader) {
	shader = p_shader;

	RID rid;
	if (shader.is_valid()) {
		rid = shader->get_rid();
	}

	RID material_rid = _get_material();
	if (material_rid.is_valid()) {
		RS::get_singleton()->material_set_shader(material_rid, rid);
	}

	notify_property_list_changed(); //properties for shader exposed
	emit_changed();
}


Ref<Shader> ShaderVariantMaterial::get_shader() const {
	return ShaderMaterial::get_shader();
}
void ShaderVariantMaterial::set_base_shader(const Ref<Shader> &p_shader) {
	ERR_FAIL_COND_MSG(p_shader->get_code().is_empty(), "The shader provided to ShaderVariantMaterial must have source code that can be modified.");

	// Only connect/disconnect the signal when running in the editor.
	// This can be a slow operation, and `notify_property_list_changed()` (which is called by `_shader_changed()`)
	// does nothing in non-editor builds anyway. See GH-34741 for details.
	if (base_shader.is_valid() && Engine::get_singleton()->is_editor_hint()) {
		base_shader->disconnect_changed(callable_mp(this, &ShaderVariantMaterial::_base_shader_changed));
	}

	base_shader = p_shader;
	base_shader_source = p_shader->get_code();

	if (base_shader.is_valid() && Engine::get_singleton()->is_editor_hint()) {
		base_shader->connect_changed(callable_mp(this, &ShaderVariantMaterial::_base_shader_changed));
	}

	// This sets shader and updates properties and everything. Everything is handled here.
	_set_modified_shader_from_current();
}
Ref<Shader> ShaderVariantMaterial::get_base_shader() const {
	return base_shader;
}

void ShaderVariantMaterial::set_shader_feature(const StringName& p_feature, const bool p_enabled) {
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_feature) {
			if (!data->is_variant) {
				if (data->_state.feature_state != p_enabled) {
					data->_state.feature_state = p_enabled;
					_set_modified_shader_from_current();
					return;
				} else {
					// It's alright, but no state change.
					return;
				}
			}
		}
	}
	ERR_FAIL_MSG(vformat("No such shader feature '%s'", p_feature));
}
bool ShaderVariantMaterial::get_shader_feature(const StringName& p_feature) const {
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_feature) {
			if (!data->is_variant) {
				return data->_state.feature_state;
			}
		}
	}
	ERR_FAIL_V_MSG(false, vformat("No such shader feature '%s'", p_feature));
}

void ShaderVariantMaterial::set_shader_variant(const StringName& p_variant, const int p_index) {
	ERR_FAIL_COND_MSG(p_index < 0, "Index cannot be negative.");
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_variant) {
			if (data->is_variant) {
				ERR_FAIL_INDEX(p_index, data->valid_variants.size());
				if (data->_state.variant_index != p_index) {
					data->_state.variant_index = p_index;
					_set_modified_shader_from_current();
				} else {
					return;
				}
			}
		}
	}
	ERR_FAIL_MSG(vformat("No such shader variant '%s'", p_variant));
}
int ShaderVariantMaterial::get_shader_variant(const StringName& p_variant) const {
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_variant) {
			if (data->is_variant) {
				return data->_state.variant_index;
			}
		}
	}
	ERR_FAIL_V_MSG(0, vformat("No such shader variant '%s'", p_variant));
}

bool ShaderVariantMaterial::is_valid_shader_feature(const StringName& p_feature) {
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_feature) {
			if (!data->is_variant) {
				// This statement adds support for variants and features with the same name, which is kind of stupid but I'll allow it.
				return true;
			}
		}
	}
	return false;
}
bool ShaderVariantMaterial::is_valid_shader_variant_key(const StringName& p_variant) {
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_variant) {
			if (data->is_variant) {
				// This statement adds support for variants and features with the same name, which is kind of stupid but I'll allow it.
				return true;
			}
		}
	}
	return false;
}
bool ShaderVariantMaterial::is_valid_shader_variant_value(const StringName& p_variant, const StringName& p_value_name) {
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_variant) {
			if (data->is_variant) {
				return data->valid_variants.find(p_value_name) != -1;
			}
		}
	}
	return false;
}
bool ShaderVariantMaterial::is_valid_shader_variant_value_index(const StringName& p_variant, const int p_index) {
	if (p_index < 0) {
		return false;
	}
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_variant) {
			if (data->is_variant) {
				return p_index < data->valid_variants.size();
			}
		}
	}
	return false;
}
int ShaderVariantMaterial::get_shader_variant_index(const StringName& p_variant, const StringName& p_value_name) {
	for (const Ref<ShaderVariantMetadata> &data : variant_information) {
		if (data->definition == p_variant) {
			if (data->is_variant) {
				return data->valid_variants.find(p_value_name);
			}
		}
	}
	return -1;
}

TypedArray<ShaderVariantMetadata> ShaderVariantMaterial::get_variant_metadata() const {
	return XanArrayInterop::vector_to_typed_array(variant_information);
}
void ShaderVariantMaterial::set_variant_metadata(TypedArray<ShaderVariantMetadata>& p_metadata) {
	TypedArray<ShaderVariantMetadata> sanitized;
	sanitized.resize(p_metadata.size());

	HashSet<StringName> unique_names;
	for (int i = 0; i < p_metadata.size(); ++i) {
		const Ref<ShaderVariantMetadata> metadata = p_metadata[i];
		if (!unique_names.has(metadata->definition)) {
			sanitized.push_back(metadata);
			unique_names.insert(metadata->definition);
		}
	}

	Vector<Ref<ShaderVariantMetadata>> replacement_metadata;
	replacement_metadata.resize(sanitized.size());

	for (const Ref<ShaderVariantMetadata> metadata : sanitized) {
		Ref<ShaderVariantMetadata> my_copy = metadata->copy();

		int index_to_remove = -1;
		int i = 0;
		for (const Ref<ShaderVariantMetadata> &existing : variant_information) {
			if (existing->definition == metadata->definition && existing->is_variant == metadata->is_variant) {
				// Might be compatible...
				if (existing->is_variant) {
					// Make sure the variants have an option with the same name
					const StringName name = existing->valid_variants[existing->_state.variant_index];
					if (my_copy->valid_variants.find(name) >= 0) {
						my_copy->_state = existing->_state;
						index_to_remove = i;
						break;
					}
				}
			}
			i++;
		}
		if (index_to_remove != -1) {
			variant_information.remove_at(index_to_remove);
		}

		replacement_metadata.push_back(my_copy);
	}
	variant_information.clear();
	variant_information = replacement_metadata;
	_set_modified_shader_from_current();
}

void ShaderVariantMaterial::_base_shader_changed() {
	// notify_property_list_changed();
	_set_modified_shader_from_current();
}

bool ShaderVariantMaterial::_set(const StringName& p_name, const Variant& p_value) {

}
bool ShaderVariantMaterial::_get(const StringName& p_name, Variant& r_ret) const {

}
void ShaderVariantMaterial::_get_property_list(List<PropertyInfo>* p_list) const {

}
bool ShaderVariantMaterial::_property_can_revert(const StringName& p_name) const {

}
bool ShaderVariantMaterial::_property_get_revert(const StringName& p_name, Variant& r_property) const {

}

void ShaderVariantMaterial::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_base_shader", "shader"), &ShaderVariantMaterial::set_base_shader);
	ClassDB::bind_method(D_METHOD("get_base_shader"), &ShaderVariantMaterial::get_base_shader);

	ClassDB::bind_method(D_METHOD("set_shader_feature", "feature", "state"), &ShaderVariantMaterial::set_shader_feature);
	ClassDB::bind_method(D_METHOD("get_shader_feature", "feature"), &ShaderVariantMaterial::get_shader_feature);

	ClassDB::bind_method(D_METHOD("set_shader_variant", "variant", "index"), &ShaderVariantMaterial::set_shader_variant);
	ClassDB::bind_method(D_METHOD("get_shader_variant", "variant"), &ShaderVariantMaterial::get_shader_variant);

	ClassDB::bind_method(D_METHOD("is_valid_shader_feature", "feature"), &ShaderVariantMaterial::is_valid_shader_feature);
	ClassDB::bind_method(D_METHOD("is_valid_shader_variant_key", "variant"), &ShaderVariantMaterial::is_valid_shader_variant_key);
	ClassDB::bind_method(D_METHOD("is_valid_shader_variant_value", "variant", "index"), &ShaderVariantMaterial::is_valid_shader_variant_value);
	ClassDB::bind_method(D_METHOD("get_shader_variant_index", "variant", "value_name"), &ShaderVariantMaterial::get_shader_variant_index);

	ClassDB::bind_method(D_METHOD("set_variant_metadata", "metadata"), &ShaderVariantMaterial::set_variant_metadata);
	ClassDB::bind_method(D_METHOD("get_variant_metadata"), &ShaderVariantMaterial::get_variant_metadata);

	/*
	bool is_valid_shader_feature(const StringName &p_feature);
	bool is_valid_shader_variant_key(const StringName &p_variant);
	bool is_valid_shader_variant_value(const StringName &p_variant, const StringName &p_value_name);
	bool is_valid_shader_variant_value_index(const StringName &p_variant, const int p_index);
	int get_shader_variant_index(const StringName &p_variant, const StringName &p_value_name);

	TypedArray<ShaderVariantMetadata> get_variant_metadata() const;
	void set_variant_metadata(TypedArray<ShaderVariantMetadata> &p_metadata);
	*/

	ADD_READONLY_PROPERTY(PropertyInfo(Variant::OBJECT, "shader", PROPERTY_HINT_RESOURCE_TYPE, Shader::get_class_static()), "get_shader");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "base_shader", PROPERTY_HINT_RESOURCE_TYPE, Shader::get_class_static()), "set_base_shader", "get_base_shader");
}

ShaderVariantMaterial::ShaderVariantMaterial() {

}
ShaderVariantMaterial::~ShaderVariantMaterial() {

}

#endif
