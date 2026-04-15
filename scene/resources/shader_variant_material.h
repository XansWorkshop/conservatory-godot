/**************************************************************************/
/*  shader_variant_material.h                                             */
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
#include "scene/resources/shader.h"
#include "scene/resources/material.h"
#include "scene/resources/shader_variant_metadata.h"
#include "core/templates/hash_map.h"
#include "core/variant/typed_array.h"

#include "thirdparty/xanstools/array_interop.h"

class ShaderVariantMaterial : public ShaderMaterial {
	GDCLASS(ShaderVariantMaterial, ShaderMaterial);

private:
	Ref<Shader> base_shader;
	String base_shader_source;
	Vector<Ref<ShaderVariantMetadata>> variant_information;

	//Ref<Shader> _create_modified_shader_from_current() const;
	void _set_modified_shader_from_current();

protected:
	static void _bind_methods();

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	bool _property_can_revert(const StringName &p_name) const;
	bool _property_get_revert(const StringName &p_name, Variant &r_property) const;

public:
	void set_shader(const Ref<Shader> &p_shader) override {
		ERR_FAIL_MSG("Cannot directly set the shader of a ShaderVariantMaterial. To change the base shader upon which variants are installed, use set_base_shader.");
	}
	void set_shader_no_change_logic(const Ref<Shader> &p_shader);
	Ref<Shader> get_shader() const override;
	void set_base_shader(const Ref<Shader> &p_shader);
	Ref<Shader> get_base_shader() const;

	void set_shader_feature(const StringName &p_feature, const bool p_enabled);
	bool get_shader_feature(const StringName &p_feature) const;

	void set_shader_variant(const StringName &p_variant, const int p_index);
	int get_shader_variant(const StringName &p_variant) const;

	bool is_valid_shader_feature(const StringName &p_feature);
	bool is_valid_shader_variant_key(const StringName &p_variant);
	bool is_valid_shader_variant_value(const StringName &p_variant, const StringName &p_value_name);
	bool is_valid_shader_variant_value_index(const StringName &p_variant, const int p_index);
	int get_shader_variant_index(const StringName &p_variant, const StringName &p_value_name);

	TypedArray<ShaderVariantMetadata> get_variant_metadata() const;
	void set_variant_metadata(TypedArray<ShaderVariantMetadata> &p_metadata);

	void _base_shader_changed();

	ShaderVariantMaterial();
	~ShaderVariantMaterial();
};
#endif
