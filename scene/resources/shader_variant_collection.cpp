#if false
/**************************************************************************/
/*  shader_variant_collection.cpp                                         */
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

#include "shader_variant_collection.h"
#include "core/string/string_builder.h"

#define HASHER_SEED_SHADERS 0xFACEAF0E // Funny reference

ShaderVariantCollection::ShaderVariantCollection() {
	owner = nullptr;
	last_set_shader = nullptr;
	base_shader = RID();
	base_shader_hash = 0;
	base_shader_code = String();
	current_shader_variation = RID();
	cache = nullptr;
	variants = HashMap<String, String>();
	features = List<String>();
	valid_features = List<String>();
	valid_variants = HashMap<String, List<String>>();
	current_mode = Shader::Mode::MODE_SPATIAL;
}

ShaderVariantCollection::~ShaderVariantCollection() {
	if (last_set_shader.is_valid()) {
		if (cache) {
			cache->finished();
			cache = nullptr;
		}
	}

	last_set_shader = nullptr;
	variants.clear();
	features.clear();
	valid_variants.clear();
	valid_features.clear();
}

String ShaderVariantCollection::preprocess_code(const String &p_code, const List<String> *p_in_defines, const bool p_get_features_and_variants) {
	ShaderPreprocessor processor;
	String file = this->last_set_shader->get_path();
	String error_text;
	List<ShaderPreprocessor::FilePosition> error_locations;

	String code = p_code;
	if (p_in_defines) {
		StringBuilder new_src;
		for (String name : *p_in_defines) {
			new_src.append("#define ");
			new_src.append(name);
			new_src.append("\n");
		}
		new_src.append("\n");
		new_src.append(code);
		code = new_src.as_string();
	}
	String result = code;

	List<String> found_features;
	HashMap<String, List<String>> found_variants;
	Error error = processor.preprocess(code, file, result, &error_text, &error_locations, nullptr, nullptr, nullptr, nullptr, nullptr, &found_features, &found_variants);

	int line = -1;
	List<ShaderPreprocessor::FilePosition>::Element *err_location = error_locations.front();
	if (err_location) {
		line = err_location->get().line;
	}
	ERR_FAIL_COND_V_MSG(error != Error::OK, vformat("%s (line %d)", error_text, line), false);

	if (p_get_features_and_variants) {
		valid_features = found_features;
		valid_variants = found_variants;
	}

	return result;
}

Ref<Shader> ShaderVariantCollection::get_shader() const {
	_THREAD_SAFE_METHOD_

	return last_set_shader;
}

bool ShaderVariantCollection::set_shader(const Ref<Shader> &p_shader) {
	_THREAD_SAFE_METHOD_

	if (p_shader == last_set_shader) {
		return false;
	}

	if (p_shader.is_valid()) {
		if (cache) {
			cache->finished();
			cache = nullptr;
		}

		last_set_shader = p_shader;
		String code = p_shader->get_code();
		base_shader = p_shader->get_rid();
		base_shader_code = code; //preprocess_code(code, nullptr, true);
		base_shader_hash = hash_shader_features_and_variants(base_shader, nullptr, nullptr);
		cache = ShaderVariantCache::get_for_base_rid(base_shader, base_shader_hash);
		String type = ShaderLanguage::get_shader_type(base_shader_code);

		if (type == "canvas_item") {
			current_mode = Shader::Mode::MODE_CANVAS_ITEM;
		} else if (type == "particles") {
			current_mode = Shader::Mode::MODE_PARTICLES;
		} else if (type == "sky") {
			current_mode = Shader::Mode::MODE_SKY;
		} else if (type == "fog") {
			current_mode = Shader::Mode::MODE_FOG;
		} else {
			current_mode = Shader::Mode::MODE_SPATIAL;
		}

		update_current_instance();

	} else {
		if (cache) {
			cache->finished();
			cache = nullptr;
		}
		last_set_shader = nullptr;
		base_shader = RID();
		base_shader_code = String();
		base_shader_hash = 0;
		current_mode = Shader::Mode::MODE_SPATIAL;

		update_current_instance();

	}

	return true;
}

bool ShaderVariantCollection::update_current_instance() {
	bool just_created_shader_rid = false;
	RID previous = current_shader_variation;
	update_current_shader_rid_nolock(&just_created_shader_rid);
	if (just_created_shader_rid) {
		StringBuilder result_code;
		List<String> defines = features;
		for (KeyValue<String, String> variant : variants) {
			defines.push_back(variant.value);
		}
		String this_code = preprocess_code(base_shader_code, &defines, false);
		RSI->shader_set_code(current_shader_variation, this_code);
	}
	return previous != current_shader_variation;
}

void ShaderVariantCollection::get_current_variant_uniform_list(List<PropertyInfo>* p_params, bool p_get_groups) const {
	if (current_shader_variation.is_null() || last_set_shader.is_null()) {
		return;
	}
	List<PropertyInfo> local;
	RenderingServer::get_singleton()->get_shader_parameter_list(current_shader_variation, &local);

#ifdef TOOLS_ENABLED
	DocData::ClassDoc class_doc;
	class_doc.name = last_set_shader->get_path();
	class_doc.is_script_doc = true;
#endif

	for (PropertyInfo &pi : local) {
		bool is_group = pi.usage == PROPERTY_USAGE_GROUP || pi.usage == PROPERTY_USAGE_SUBGROUP;
		if (!p_get_groups && is_group) {
			continue;
		}
		if (!is_group) {
			if (last_set_shader->default_textures.has(pi.name)) { //do not show default textures
				continue;
			}
		}
		if (p_params) {
			//small little hack
			if (pi.type == Variant::RID) {
				pi.type = Variant::OBJECT;
			}
#ifdef TOOLS_ENABLED
			if (Engine::get_singleton()->is_editor_hint()) {
				DocData::PropertyDoc prop_doc;
				prop_doc.name = "shader_parameter/" + pi.name;
#ifdef MODULE_REGEX_ENABLED
				const RegEx pattern("/\\*\\*\\s([^*]|[\\r\\n]|(\\*+([^*/]|[\\r\\n])))*\\*+/\\s*uniform\\s+\\w+\\s+" + pi.name + "(?=[\\s:;=])");
				Ref<RegExMatch> pattern_ref = pattern.search(code);
				if (pattern_ref.is_valid()) {
					RegExMatch *match = pattern_ref.ptr();
					const RegEx pattern_tip("\\/\\*\\*([\\s\\S]*?)\\*/");
					Ref<RegExMatch> pattern_tip_ref = pattern_tip.search(match->get_string(0));
					RegExMatch *match_tip = pattern_tip_ref.ptr();
					const RegEx pattern_stripped("\\n\\s*\\*\\s*");
					prop_doc.description = pattern_stripped.sub(match_tip->get_string(1), "\n", true);
				}
#endif
				class_doc.properties.push_back(prop_doc);
			}
#endif
			p_params->push_back(pi);
		}
	}
#ifdef TOOLS_ENABLED
	if (Engine::get_singleton()->is_editor_hint() && !class_doc.name.is_empty() && p_params) {
		EditorHelp::add_doc(class_doc);
	}
#endif
}

const List<String> ShaderVariantCollection::get_variant_names() const {
	_THREAD_SAFE_METHOD_

	List<String> result;
	for (KeyValue<String, List<String>> binding : valid_variants) {
		result.push_back(binding.key);
	}
	return result;
}

const List<String> ShaderVariantCollection::get_available_variants(const String &p_variant_name) const {
	_THREAD_SAFE_METHOD_

	const List<String> *options = valid_variants.getptr(p_variant_name);
	ERR_FAIL_NULL_V_MSG(options, List<String>(), vformat("No such variant collection %s", p_variant_name));
	return *options;
}

const List<String> ShaderVariantCollection::get_available_features() const {
	_THREAD_SAFE_METHOD_

	return valid_features;
}

String ShaderVariantCollection::get_default_variant(const String &p_variant_name) const {
	_THREAD_SAFE_METHOD_

	const List<String> *options = valid_variants.getptr(p_variant_name);
	ERR_FAIL_NULL_V_MSG(options, String(), vformat("No such variant collection %s", p_variant_name));
	const List<String>::Element *first_element = options->front();
	ERR_FAIL_NULL_V(first_element, String());

	return first_element->get();
}

String ShaderVariantCollection::get_variant(const String &p_variant_name) const {
	_THREAD_SAFE_METHOD_

	const List<String> *options = valid_variants.getptr(p_variant_name);
	ERR_FAIL_NULL_V_MSG(options, String(), vformat("No such variant collection %s", p_variant_name));

	const String *current = variants.getptr(p_variant_name);
	CRASH_COND_MSG(current, vformat("Unexpected missing variant in colection %s", p_variant_name));
	return *current;
}

bool ShaderVariantCollection::set_variant(const String &p_variant_name, const String &p_variant) {
	_THREAD_SAFE_METHOD_

	const List<String> *options = valid_variants.getptr(p_variant_name);
	ERR_FAIL_NULL_V_MSG(options, false, vformat("No such variant collection %s", p_variant_name));

	String *current = variants.getptr(p_variant_name);
	CRASH_COND_MSG(current, vformat("Unexpected missing variant in colection %s", p_variant_name));
	if (*current == p_variant) {
		return false;
	} else {
		*current = p_variant;
		update_current_instance();
		return true;
	}
}

bool ShaderVariantCollection::enable_feature(const String &p_feature) {
	_THREAD_SAFE_METHOD_
	if (!features.find(p_feature)) {
		features.push_back(p_feature);
		update_current_instance();
		return true;
	}
	return false;
}

bool ShaderVariantCollection::disable_feature(const String &p_feature) {
	_THREAD_SAFE_METHOD_
	List<String>::Element *element = features.find(p_feature);
	if (element) {
		features.move_to_back(element);
		features.pop_back();
		update_current_instance();
		return true;
	}
	return false;
}

bool ShaderVariantCollection::enable_features(const List<String> &p_features) {
	_THREAD_SAFE_METHOD_
	bool changed = false;
	for (String feature : p_features) {
		if (!features.find(feature)) {
			features.push_back(feature);
			changed = true;
		}
	}
	if (changed) {
		update_current_instance();
	}
	return changed;
}

bool ShaderVariantCollection::disable_features(const List<String> &p_features) {
	_THREAD_SAFE_METHOD_
	bool changed = false;
	for (String feature : p_features) {
		List<String>::Element *element = features.find(feature);
		if (element) {
			features.move_to_back(element);
			features.pop_back();
			changed = true;
		}
	}
	if (changed) {
		update_current_instance();
	}
	return changed;
}

bool ShaderVariantCollection::is_using_feature(const String &p_feature) const {
	_THREAD_SAFE_METHOD_
	return features.find(p_feature);
}

bool ShaderVariantCollection::set_all(const List<String> &p_features, const HashMap<String, String> &p_variants, List<String> *p_last_features, HashMap<String, String> *p_last_variants) {
	_THREAD_SAFE_METHOD_

	ERR_FAIL_NULL_V(p_last_features, false);
	ERR_FAIL_NULL_V(p_last_variants, false);
	get_all_nolock(p_last_features, p_last_variants);

	List<String> current_features = p_features;
	HashMap<String, String> current_variants = p_variants;

	bool has_different = false;
	if (current_features.size() == p_last_features->size()) {
		current_features.sort();
		p_last_features->sort();
		for (int i = 0; i < current_features.size(); ++i) {
			if (current_features.get(i) != p_last_features->get(i)) {
				has_different = true;
				break;
			}
		}
	} else {
		has_different = true;
	}
	if (!has_different) {
		if (current_variants.size() == p_last_variants->size()) {
			for (KeyValue<String, String> replacement : current_variants) {
				String *last_equivalent = p_last_variants->getptr(replacement.key);
				if (!last_equivalent || (*last_equivalent != replacement.value)) {
					has_different = true;
					break;
				}
			}
		} else {
			has_different = true;
		}
	}

	if (!has_different) return false;

	features = current_features;
	variants = current_variants;
	update_current_instance();
	return true;
}

void ShaderVariantCollection::get_all_nolock(List<String> *p_features, HashMap<String, String> *p_variants) {
	if (!p_features && !p_variants) {
		return;
	}

	if (p_features) {
		p_features->clear();
		for (String feature : features) {
			p_features->push_back(feature);
		}
	}
	if (p_variants) {
		p_variants->clear();
		for (KeyValue<String, String> ctr : variants) {
			p_variants->insert(ctr.key, ctr.value);
		}
	}
}
void ShaderVariantCollection::get_all(List<String> *p_features, HashMap<String, String> *p_variants) {
	if (!p_features && !p_variants) {
		return;
	}

	_THREAD_SAFE_METHOD_
	get_all_nolock(p_features, p_variants);
}

void ShaderVariantCollection::update_current_shader_rid(bool *p_is_new) {
	_THREAD_SAFE_METHOD_
	update_current_shader_rid_nolock(p_is_new);
}

void ShaderVariantCollection::update_current_shader_rid_nolock(bool *p_is_new) {
	if (!cache || base_shader.is_null()) {
		current_shader_variation = RID();
		return;
	}
	uint64_t default_shader_hash = hash_shader_features_and_variants(base_shader, &features, &variants);
	current_shader_variation = cache->get_rid_for(default_shader_hash, p_is_new);
}

uint64_t ShaderVariantCollection::hash_shader_features_and_variants(RID p_base_shader_rid, const List<String> *p_features, const HashMap<String, String> *p_variants) {
#define BOOST_HASH_TECHNIQUE(seed, hash) seed ^= hash + 0x9E3779B9 + (seed << 6) + (seed >> 2)

	uint64_t seed = p_base_shader_rid.get_id() ^ HASHER_SEED_SHADERS;

	if (p_features) {
		List<String> feats = *p_features;
		for (const String feature : feats) {
			BOOST_HASH_TECHNIQUE(seed, feature.hash());
		}
	}
	if (p_variants) {
		HashMap<String, String> vars = *p_variants;
		for (const KeyValue<String, String> variant : vars) {
			BOOST_HASH_TECHNIQUE(seed, variant.key.hash());
			BOOST_HASH_TECHNIQUE(seed, variant.value.hash());
		}
	}

	return seed;

#undef BOOST_HASH_TECHNIQUE
}

/////////////////////////////////

HashMap<RID, ShaderVariantCache> ShaderVariantCache::stored_shader_variant_caches;
Mutex ShaderVariantCache::static_access_mutex;
void ShaderVariantCache::increment() {
	ref_count++;
}

bool ShaderVariantCache::decrement() {
	CRASH_COND_MSG(ref_count == 0, vformat("Something caused a miscount in how many times shader RID:%d was used, resulting in -1 objects using the shader. For reasons that are (hopefully) obvious, this is very wrong.", base_shader_rid.get_id()));
	ref_count--;
	return ref_count == 0;
}

RID ShaderVariantCache::get_rid_for(const uint64_t p_combo_hash, bool *p_is_new) {
	RID *rid = other_shader_variants.getptr(p_combo_hash);
	if (!rid) {
		if (p_is_new) {
			*p_is_new = true;
		}
		return RSI->shader_create();
	} else {
		if (p_is_new) {
			*p_is_new = false;
		}
		return *rid;
	}
}

void ShaderVariantCache::finished() {
	MutexLock lock(static_access_mutex);
	ShaderVariantCache *instance = stored_shader_variant_caches.getptr(base_shader_rid);
	CRASH_COND_MSG(instance == nullptr, vformat("Invalid attempt to discard a variant cache for shader with RID %d, which is not in the cache.", base_shader_rid.get_id()));
	if (instance->decrement()) {
		stored_shader_variant_caches.erase(base_shader_rid);
		memdelete(instance);
	}
}

ShaderVariantCache *ShaderVariantCache::get_for_base_rid(const RID p_base_shader_rid, const uint64_t p_combo_hash) {
	MutexLock lock(static_access_mutex);
	ShaderVariantCache *instance = stored_shader_variant_caches.getptr(p_base_shader_rid);
	if (!instance) {
		instance = memnew(ShaderVariantCache);
		instance->base_shader_rid = p_base_shader_rid;
		instance->increment();
		// Future Xan: C++'s byref (& on argument) is the same as a pointer arg.
		// When dereferencing here, it's kind of a red herring. The address it
		// receives is the exact same, its a syntax thing, basically this would
		// be like the method receiving T* and then we pass in *(&instance)
		stored_shader_variant_caches.insert(p_base_shader_rid, *instance);
	}
	return instance;
}

ShaderVariantCache::ShaderVariantCache() {
	other_shader_variants = HashMap<uint64_t, RID>();
	base_shader_rid = RID();
	ref_count = 0;
}

ShaderVariantCache::~ShaderVariantCache() {
	RenderingServer *rs = RSI;
	for (KeyValue<uint64_t, RID> id : other_shader_variants) {
		rs->free(id.value);
	}
	rs->free(base_shader_rid);
	other_shader_variants.clear();
}
#endif
