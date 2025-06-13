#if false
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

#ifndef SHADER_VARIANT_MATERIAL_H
#define SHADER_VARIANT_MATERIAL_H

#include "shader_variant_collection.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/templates/hash_map.h"
#include "core/templates/hash_set.h"
#include "core/variant/typed_array.h"
#include "core/variant/typed_dictionary.h"
#include "core/string/string_builder.h"
#include "scene/resources/shader.h"
#include "scene/resources/material.h"
#include "servers/rendering/shader_language.h"
#include "servers/rendering/shader_preprocessor.h"
#include "servers/rendering_server.h"

#define TC_TYPED_DICT_STR_STR TypedDictionary<String, String>
#define TC_LIST_TO_TYPED_ARRAY(T, name, list)                                  \
	TypedArray<T> name;                                                        \
	{                                                                          \
		int name##__size = list.size();                                        \
		name.resize(name##__size);                                             \
		for (int name##__inc = 0; name##__inc < name##__size; ++name##__inc) { \
			name.set(name##__inc, list.get(name##__inc));                      \
		}                                                                      \
	}                                                                          \
	(void)0

#define TC_SET_TO_TYPED_ARRAY(T, name, hashset) \
	TypedArray<T> name;                         \
	{                                           \
		int name##__size = hashset.size();      \
		name.resize(name##__size);              \
		int name##__inc = 0;                    \
		for (T value : hashset) {               \
			name.set(name##__inc++, value);     \
		}                                       \
	}                                           \
	(void)0

#define TC_HASHMAP_TO_TYPED_DICTIONARY(TKey, TValue, name, map) \
	TypedDictionary<TKey, TValue> name;                         \
	{                                                           \
		for (KeyValue<TKey, TValue> name##__kvp : map) {        \
			name.set(name##__kvp.key, name##__kvp.value);       \
		}                                                       \
	}                                                           \
	(void)0	

class ShaderVariantMaterial : public ShaderMaterial {
	GDCLASS(ShaderVariantMaterial, ShaderMaterial);
	_THREAD_SAFE_CLASS_

	mutable ShaderVariantCollection *vars;
	Ref<Shader> shader;
	mutable HashMap<StringName, StringName> parameter_remap_cache;
	mutable HashMap<StringName, Variant> uniform_cache;
	mutable Mutex material_rid_mutex;
	Shader::Mode mode;

	_FORCE_INLINE_ void ensure_shader_parity() const {
		_THREAD_SAFE_METHOD_
		if (vars->last_set_shader != shader) {
			vars->set_shader(shader);
		}
	}

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	bool _property_can_revert(const StringName &p_name) const;
	bool _property_get_revert(const StringName &p_name, Variant &r_property) const;

	static void _bind_methods();

public:
	Ref<Shader> get_shader() const override;
	void set_shader(const Ref<Shader>& p_shader) override;

	void set_shader_parameter(const String &p_param, const Variant &p_value);
	Variant get_shader_parameter(const String &p_param) const;

	void _check_material_rid() const;
		
#ifdef TOOLS_ENABLED
	void get_argument_options(const StringName &p_function, int p_idx, List<String> *r_options) const override;
#endif

	virtual RID get_rid() const override;
	virtual RID get_shader_rid() const override;
	virtual Shader::Mode get_shader_mode() const override;

	const List<String> get_variant_names() const;
	const List<String> get_available_variants(const String &p_variant_name) const;
	const List<String> get_available_features() const;

	String get_default_variant(const String &p_variant_name) const;
	String get_variant(const String &p_variant_name) const;
	void set_variant(const String &p_variant_name, const String &p_variant);

	void enable_feature(const String &p_feature);
	void disable_feature(const String &p_feature);
	void enable_features(const TypedArray<String> &p_features);
	void disable_features(const TypedArray<String> &p_features);
	bool is_using_feature(const String &p_feature);

	void set_all(const List<String> &p_features, const HashMap<String, String> &p_variants);
	void get_all(List<String> *p_features, HashMap<String, String> *p_variants) const;

	void emit_compilation_changed();

	// GDVIRTUAL4(features_or_variants_changed, TypedArray<StringName>, TypedArray<StringName>, TC_TYPED_DICT_SNAME_SNAME, TC_TYPED_DICT_SNAME_SNAME);
	GDVIRTUAL2(compilation_changed, TypedArray<String>, TC_TYPED_DICT_STR_STR);

	ShaderVariantMaterial();
	~ShaderVariantMaterial();
};

#undef TC_TYPED_DICT_SNAME_SNAME

#endif // SHADER_VARIANT_MATERIAL_H
#endif
