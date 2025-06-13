#if false
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

#include "shader_variant_material.h"

#define SET_TO_CURRENT_VARIATION RSI->material_set_shader(get_rid(), vars->current_shader_variation)

ShaderVariantMaterial::ShaderVariantMaterial() {
	vars = memnew(ShaderVariantCollection);
	vars->owner = this;
}

ShaderVariantMaterial::~ShaderVariantMaterial() {
	memdelete(vars);
}

Ref<Shader> ShaderVariantMaterial::get_shader() const {
	ensure_shader_parity();
	return shader;
}

void ShaderVariantMaterial::set_shader(const Ref<Shader> &p_shader) {
	shader = p_shader;
	bool changed = vars->set_shader(p_shader);
	if (changed) {
		SET_TO_CURRENT_VARIATION;
		emit_compilation_changed();
	}
}

const List<String> ShaderVariantMaterial::get_variant_names() const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	return vars->get_variant_names();
}

const List<String> ShaderVariantMaterial::get_available_variants(const String &p_variant_name) const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	return vars->get_available_variants(p_variant_name);
}

const List<String> ShaderVariantMaterial::get_available_features() const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	return vars->get_available_features();
}

String ShaderVariantMaterial::get_default_variant(const String &p_variant_name) const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	return vars->get_default_variant(p_variant_name);
}
String ShaderVariantMaterial::get_variant(const String &p_variant_name) const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	return vars->get_variant(p_variant_name);
}

void ShaderVariantMaterial::set_variant(const String &p_variant_name, const String &p_variant) {
	ensure_shader_parity();
	bool changed = vars->set_variant(p_variant_name, p_variant);
	if (changed) {
		SET_TO_CURRENT_VARIATION;
		emit_compilation_changed();
	}
}

void ShaderVariantMaterial::enable_feature(const String &p_feature) {
	ensure_shader_parity();
	bool changed = vars->enable_feature(p_feature);
	if (changed) {
		SET_TO_CURRENT_VARIATION;
		emit_compilation_changed();
	}
}
void ShaderVariantMaterial::disable_feature(const String &p_feature) {
	ensure_shader_parity();
	bool changed = vars->disable_feature(p_feature);
	if (changed) {
		SET_TO_CURRENT_VARIATION;
		emit_compilation_changed();
	}
}
void ShaderVariantMaterial::enable_features(const TypedArray<String> &p_features) {
	ensure_shader_parity();
	List<String> result;
	for (int i = 0; i < p_features.size(); ++i) {
		result.push_back(p_features.get(i));
	}
	bool changed = vars->enable_features(result);
	if (changed) {
		SET_TO_CURRENT_VARIATION;
		emit_compilation_changed();
	}
}
void ShaderVariantMaterial::disable_features(const TypedArray<String> &p_features) {
	ensure_shader_parity();
	List<String> result;
	for (int i = 0; i < p_features.size(); ++i) {
		result.push_back(p_features.get(i));
	}
	bool changed = vars->disable_features(result);
	if (changed) {
		SET_TO_CURRENT_VARIATION;
		emit_compilation_changed();
	}
}

bool ShaderVariantMaterial::is_using_feature(const String &p_feature) {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	return vars->is_using_feature(p_feature);
}

void ShaderVariantMaterial::set_all(const List<String> &p_features, const HashMap<String, String> &p_variants) {
	ensure_shader_parity();
	List<String> last_features;
	HashMap<String, String> last_variants;
	bool changed = vars->set_all(p_features, p_variants, &last_features, &last_variants);

	if (changed) {
		SET_TO_CURRENT_VARIATION;
		emit_compilation_changed();
	}
}

void ShaderVariantMaterial::get_all(List<String> *p_features, HashMap<String, String> *p_variants) const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	vars->get_all(p_features, p_variants);
}

void ShaderVariantMaterial::emit_compilation_changed() {
	if (GDVIRTUAL_IS_OVERRIDDEN(compilation_changed)) {
		TC_SET_TO_TYPED_ARRAY(StringName, current_features_gd, vars->features);
		TC_HASHMAP_TO_TYPED_DICTIONARY(String, String, current_variants_gd, vars->variants);
		GDVIRTUAL_CALL(compilation_changed, current_features_gd, current_variants_gd);
	}
}

void ShaderVariantMaterial::set_shader_parameter(const String &p_param, const Variant &p_value) {
	ensure_shader_parity();
	RID material_rid = _get_material();
	if (p_value.get_type() == Variant::NIL) {
		uniform_cache.erase(p_param);
		if (material_rid.is_valid()) {
			RSI->material_set_param(material_rid, p_param, Variant());
		}
	} else {
		Variant *v = uniform_cache.getptr(p_param);
		if (!v) {
			// Never assigned, also update the remap cache.
			parameter_remap_cache["shader_parameter/" + p_param] = p_param;
			uniform_cache.insert(p_param, p_value);
		} else {
			*v = p_value;
		}

		if (p_value.get_type() == Variant::OBJECT) {
			RID tex_rid = p_value;
			if (tex_rid == RID()) {
				uniform_cache.erase(p_param);

				if (material_rid.is_valid()) {
					RSI->material_set_param(material_rid, p_param, Variant());
				}
			} else if (material_rid.is_valid()) {
				RSI->material_set_param(material_rid, p_param, tex_rid);
			}
		} else if (material_rid.is_valid()) {
			RSI->material_set_param(material_rid, p_param, p_value);
		}
	}
}

Variant ShaderVariantMaterial::get_shader_parameter(const String &p_param) const {
	ensure_shader_parity();
	if (uniform_cache.has(p_param)) {
		return uniform_cache[p_param];
	} else {
		return Variant();
	}
}

void ShaderVariantMaterial::_check_material_rid() const {
	ensure_shader_parity();
	MutexLock lock(material_rid_mutex);
	if (_get_material().is_null()) {
		RID shader_rid = vars->current_shader_variation;
		RID next_pass_rid;
		if (get_next_pass().is_valid()) {
			next_pass_rid = get_next_pass()->get_rid();
		}

		_set_material(RS::get_singleton()->material_create_from_shader(next_pass_rid, get_render_priority(), shader_rid));

		for (KeyValue<StringName, Variant> param : uniform_cache) {
			if (param.value.get_type() == Variant::OBJECT) {
				RID tex_rid = param.value;
				if (tex_rid.is_valid()) {
					RS::get_singleton()->material_set_param(_get_material(), param.key, tex_rid);
				} else {
					RS::get_singleton()->material_set_param(_get_material(), param.key, Variant());
				}
			} else {
				RS::get_singleton()->material_set_param(_get_material(), param.key, param.value);
			}
		}
	}
}

#if TOOLS_ENABLED
void ShaderVariantMaterial::get_argument_options(const StringName &p_function, int p_idx, List<String> *r_options) const {
	const String pf = p_function;
	if (p_idx == 0 && (pf == "get_shader_parameter" || pf == "set_shader_parameter")) {
		if (shader.is_valid()) {
			List<PropertyInfo> pl;
			shader->get_shader_uniform_list(&pl);
			for (const PropertyInfo &E : pl) {
				r_options->push_back(E.name.replace_first("shader_parameter/", "").quote());
			}
		}
	}
	Material::get_argument_options(p_function, p_idx, r_options);
}
#endif

RID ShaderVariantMaterial::get_rid() const {
	_check_material_rid();
	return Material::get_rid();
}

RID ShaderVariantMaterial::get_shader_rid() const {
	ensure_shader_parity();
	if (vars->current_shader_variation.is_valid()) {
		return vars->current_shader_variation;
	} else {
		return RID();
	}
}

Shader::Mode ShaderVariantMaterial::get_shader_mode() const {
	if (shader.is_valid() && vars && vars->current_shader_variation.is_valid()) {
		return vars->current_mode;
	}
	return Shader::Mode::MODE_SPATIAL;
}

bool ShaderVariantMaterial::_set(const StringName &p_name, const Variant &p_value) {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	if (shader.is_valid() && vars && vars->current_shader_variation.is_valid()) {
		const StringName *sn = parameter_remap_cache.getptr(p_name);
		if (sn) {
			set_shader_parameter(*sn, p_value);
			return true;
		}
		const String s = p_name;
		if (s.begins_with("shader_parameter/")) {
			String param = s.replace_first("shader_parameter/", "");
			parameter_remap_cache[s] = param;
			set_shader_parameter(param, p_value);
			return true;
		} else if (s.begins_with("feature/")) {
			String param = s.replace_first("feature/", "");
			if (p_value.get_type() == Variant::Type::BOOL) {
				if ((bool)p_value) {
					enable_feature(param);
				} else {
					disable_feature(param);
				}
				return true;
			}
			return false;
		} else if (s.begins_with("variant/")) {
			String param = s.replace_first("variant/", "");
			if (p_value.get_type() == Variant::Type::STRING || p_value.get_type() == Variant::Type::STRING_NAME) {
				String value = (String)p_value;
				set_variant(param, value);
				return true;
			}
			return false;
		}
	}
	return false;
}

bool ShaderVariantMaterial::_get(const StringName &p_name, Variant &r_ret) const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	if (shader.is_valid() && vars && vars->current_shader_variation.is_valid()) {
		const StringName *sn = parameter_remap_cache.getptr(p_name);
		if (sn) {
			// Only return a parameter if it was previously set.
			r_ret = get_shader_parameter(*sn);
			return true;
		}
		const String s = p_name;
		if (s.begins_with("feature/")) {
			String param = s.replace_first("feature/", "");
			r_ret = vars->is_using_feature(param);
			return true;
		} else if (s.begins_with("variant/")) {
			String param = s.replace_first("variant/", "");
			r_ret = vars->get_variant(param);
			return true;
		}
	}

	return false;
}
void ShaderVariantMaterial::_get_property_list(List<PropertyInfo> *p_list) const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_

	#define NEW_PROPERTY_GROUP(display, prefix) (PropertyInfo(Variant::Type::NIL, display, PROPERTY_HINT_NONE, prefix##"/", PROPERTY_USAGE_GROUP, StringName()))

	/*
	p_list->push_back(PropertyInfo(
		Variant::Type::OBJECT,
		"shader",
		PROPERTY_HINT_RESOURCE_TYPE,
		"Shader",
		PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED,
		"Shader"
	));
	*/
	if (shader.is_valid() && vars && vars->current_shader_variation.is_valid()) {
		List<PropertyInfo> list;
		vars->get_current_variant_uniform_list(&list, true);

		// Copied from Material

		HashMap<String, HashMap<String, List<PropertyInfo>>> groups;
		LocalVector<Pair<String, LocalVector<String>>> vgroups;
		{
			HashMap<String, List<PropertyInfo>> none_subgroup;
			none_subgroup.insert("<None>", List<PropertyInfo>());
			groups.insert("<None>", none_subgroup);
		}

		String last_group = "<None>";
		String last_subgroup = "<None>";

		bool is_none_group_undefined = true;
		bool is_none_group = true;

		#define SHADER_PARAMETERS_HEADER "Shader Parameters"

		for (const PropertyInfo &pi : list) {
			if (pi.usage == PROPERTY_USAGE_GROUP) {
				if (!pi.name.is_empty()) {
					Vector<String> vgroup = pi.name.split("::");
					last_group = vgroup[0];
					if (vgroup.size() > 1) {
						last_subgroup = vgroup[1];
					} else {
						last_subgroup = "<None>";
					}
					is_none_group = false;

					if (!groups.has(last_group)) {
						PropertyInfo info;
						info.usage = PROPERTY_USAGE_GROUP;
						info.name = vformat("%s%s%s", SHADER_PARAMETERS_HEADER, " ⸬ ", last_group.capitalize());
						info.hint_string = "shader_parameter/";

						List<PropertyInfo> none_subgroup;
						none_subgroup.push_back(info);

						HashMap<String, List<PropertyInfo>> subgroup_map;
						subgroup_map.insert("<None>", none_subgroup);

						groups.insert(last_group, subgroup_map);
						vgroups.push_back(Pair<String, LocalVector<String>>(last_group, { "<None>" }));
					}

					if (!groups[last_group].has(last_subgroup)) {
						PropertyInfo info;
						info.usage = PROPERTY_USAGE_SUBGROUP;
						info.name = last_subgroup.capitalize();
						info.hint_string = "shader_parameter/";

						List<PropertyInfo> subgroup;
						subgroup.push_back(info);

						groups[last_group].insert(last_subgroup, subgroup);
						for (Pair<String, LocalVector<String>> &group : vgroups) {
							if (group.first == last_group) {
								group.second.push_back(last_subgroup);
								break;
							}
						}
					}
				} else {
					last_group = "<None>";
					last_subgroup = "<None>";
					is_none_group = true;
				}
				continue; // Pass group.
			}

			if (is_none_group_undefined && is_none_group) {
				is_none_group_undefined = false;

				PropertyInfo info;
				info.usage = PROPERTY_USAGE_GROUP;
				info.name = SHADER_PARAMETERS_HEADER;
				info.hint_string = "shader_parameter/";
				groups["<None>"]["<None>"].push_back(info);

				vgroups.push_back(Pair<String, LocalVector<String>>("<None>", { "<None>" }));
			}

			#undef SHADER_PARAMETERS_HEADER

			const bool is_uniform_cached = uniform_cache.has(pi.name);
			bool is_uniform_type_compatible = true;

			if (is_uniform_cached) {
				// Check if the uniform Variant type changed, for example vec3 to vec4.
				const Variant &cached = uniform_cache.get(pi.name);

				if (cached.is_array()) {
					// Allow some array conversions for backwards compatibility.
					is_uniform_type_compatible = Variant::can_convert(pi.type, cached.get_type());
				} else {
					is_uniform_type_compatible = pi.type == cached.get_type();
				}

				if (is_uniform_type_compatible && pi.type == Variant::OBJECT && cached.get_type() == Variant::OBJECT) {
					// Check if the Object class (hint string) changed, for example Texture2D sampler to Texture3D.
					// Allow inheritance, Texture2D type sampler should also accept CompressedTexture2D.
					Object *cached_obj = cached;
					if (!cached_obj->is_class(pi.hint_string)) {
						is_uniform_type_compatible = false;
					}
				}
			}

			PropertyInfo info = pi;
			info.name = "shader_parameter/" + info.name;
			if (!is_uniform_cached || !is_uniform_type_compatible) {
				// Property has never been edited or its type changed, retrieve with default value.
				Variant default_value = RenderingServer::get_singleton()->shader_get_parameter_default(vars->current_shader_variation, pi.name);
				uniform_cache.insert(pi.name, default_value);
				parameter_remap_cache.insert(info.name, pi.name);
			}
			groups[last_group][last_subgroup].push_back(info);
		}

		for (const Pair<String, LocalVector<String>> &group_pair : vgroups) {
			String group = group_pair.first;
			for (const String &subgroup : group_pair.second) {
				List<PropertyInfo> &prop_infos = groups[group][subgroup];
				for (const PropertyInfo &item : prop_infos) {
					p_list->push_back(item);
				}
			}
		}

		p_list->push_back(NEW_PROPERTY_GROUP("Static Shader Features", "feature"));
		for (String valid_feature : vars->valid_features) {
			p_list->push_back(PropertyInfo(
				Variant::Type::BOOL,
				"feature/" + valid_feature,
				PROPERTY_HINT_NONE,
				"",
				PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_FORCE_RAW_DISPLAY_NAME | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED
			));
		}

		p_list->push_back(NEW_PROPERTY_GROUP("Static Shader Variants", "variant"));
	
		for (KeyValue<String, List<String>> valid_variant_lookup : vars->valid_variants) {
			StringBuilder result;
			for (int i = 0; i < valid_variant_lookup.value.size(); ++i) {
				result.append(valid_variant_lookup.value.get(i));
				if (i <= valid_variant_lookup.value.size() - 1) {
					result.append(",");
				}
			}
		
			p_list->push_back(PropertyInfo(
				Variant::Type::STRING_NAME,
				"variant/" + valid_variant_lookup.key,
				PROPERTY_HINT_ENUM,
				result.as_string(),
				PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_FORCE_RAW_DISPLAY_NAME | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED
			));
		}

		#undef NEW_PROPERTY_GROUP
	}
}
bool ShaderVariantMaterial::_property_can_revert(const StringName &p_name) const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	if (shader.is_valid() && vars && vars->current_shader_variation.is_valid()) {
		if (parameter_remap_cache.has(p_name)) {
			return true;
		}
		const String sname = p_name;
		if (vars->valid_features.find(sname.replace_first("feature/", ""))) {
			return true;
		}
		if (vars->valid_variants.getptr(sname.replace_first("variant/", ""))) {
			return true;
		}
		return sname == "render_priority" || sname == "next_pass";
	}
	return false;
}

bool ShaderVariantMaterial::_property_get_revert(const StringName &p_name, Variant &r_property) const {
	ensure_shader_parity();
	_THREAD_SAFE_METHOD_
	if (shader.is_valid() && vars && vars->current_shader_variation.is_valid()) {
		const StringName *pr = parameter_remap_cache.getptr(p_name);
		if (pr) {
			r_property = RenderingServer::get_singleton()->shader_get_parameter_default(vars->current_shader_variation, *pr);
			return true;
		} else if (p_name == "render_priority") {
			r_property = 0;
			return true;
		} else if (p_name == "next_pass") {
			r_property = Variant();
			return true;
		} else {
			const String sname = p_name;
			if (vars->valid_features.find(sname.replace_first("feature/", ""))) {
				r_property = false;
				return true;
			}
			List<String> *variations = vars->valid_variants.getptr(sname.replace_first("variant/", ""));
			if (variations && variations->size() > 0) {
				r_property = variations->get(0);
				return true;
			}
		}
	}
	return false;
}


void ShaderVariantMaterial::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_shader"), &ShaderVariantMaterial::get_shader);
	ClassDB::bind_method(D_METHOD("set_shader", "shader"), &ShaderVariantMaterial::set_shader);
	ClassDB::bind_method(D_METHOD("get_variant", "group"), &ShaderVariantMaterial::get_variant);
	ClassDB::bind_method(D_METHOD("set_variant", "group", "variant"), &ShaderVariantMaterial::set_variant);
	ClassDB::bind_method(D_METHOD("enable_feature", "feature"), &ShaderVariantMaterial::enable_feature);
	ClassDB::bind_method(D_METHOD("disable_feature", "feature"), &ShaderVariantMaterial::disable_feature);
	ClassDB::bind_method(D_METHOD("enable_features", "features"), &ShaderVariantMaterial::enable_features);
	ClassDB::bind_method(D_METHOD("disable_features", "features"), &ShaderVariantMaterial::disable_features);

	ADD_PROPERTY(PropertyInfo(Variant::Type::OBJECT, "shader", PROPERTY_HINT_RESOURCE_TYPE, "Shader", 6U, "Shader"), "set_shader", "get_shader");

	// GDVIRTUAL_BIND(features_or_variants_changed, "features_previously", "features_now", "variants_previously", "variants_now")
	GDVIRTUAL_BIND(compilation_changed, "new_features", "new_variants");
}
#endif
