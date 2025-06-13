#if false
/**************************************************************************/
/*  shader_variant_collection.h                                           */
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

#ifndef SHADER_VARIANT_COLLECTION_H
#define SHADER_VARIANT_COLLECTION_H

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/templates/hash_map.h"
#include "core/templates/hash_set.h"
#include "scene/resources/shader.h"
#include "servers/rendering/shader_language.h"
#include "servers/rendering/shader_preprocessor.h"
#include "servers/rendering_server.h"
#include "editor/editor_help.h"

class ShaderVariantCache;

class ShaderVariantCollection {
	friend class ShaderVariantMaterial;
	_THREAD_SAFE_CLASS_

	ShaderVariantMaterial* owner;

	// The shader that the instance was last set to, from which base_shader is derived.
	Ref<Shader> last_set_shader;

	// The most basic form of the shader, with no features enabled and no variants selected.
	// Note that this is NOT the default shader! The default shader has all default variants selected.
	RID base_shader;

	// The hash of the base_shader for use in stored_shader_variants
	uint64_t base_shader_hash;

	// The source code of the base shader.
	String base_shader_code;

	// The current shader in use with all features and variants applied.
	RID current_shader_variation;

	// The cache used by this instance to store all variants' RIDs.
	ShaderVariantCache *cache;

	// The current enabled features.
	mutable List<String> features;

	// The current values of variants.
	mutable HashMap<String, String> variants;

	// A cache of all known valid variants.
	mutable HashMap<String, List<String>> valid_variants;

	// A list of all known valid features.
	mutable List<String> valid_features;

	// The mode of the current shader variant.
	Shader::Mode current_mode;

private:
	static uint64_t hash_shader_features_and_variants(RID p_base_shader_rid, const List<String> *p_features, const HashMap<String, String> *p_variants);
	String preprocess_code(const String &p_code, const List<String> *p_in_defines, const bool p_get_features_and_variants);
	void get_all_nolock(List<String> *p_features, HashMap<String, String> *p_variants);

public:
	Ref<Shader> get_shader() const;
	bool set_shader(const Ref<Shader> &p_shader);

	bool update_current_instance();

	void get_current_variant_uniform_list(List<PropertyInfo> *p_params, bool p_get_groups) const;

	const List<String> get_variant_names() const;
	const List<String> get_available_variants(const String &p_variant_name) const;
	const List<String> get_available_features() const;

	String get_default_variant(const String &p_variant_name) const;
	String get_variant(const String &p_variant_name) const;
	bool set_variant(const String &p_variant_name, const String &p_variant);

	bool enable_feature(const String &p_feature);
	bool disable_feature(const String &p_feature);
	bool enable_features(const List<String> &p_features);
	bool disable_features(const List<String> &p_features);
	bool is_using_feature(const String &p_feature) const;

	bool set_all(const List<String> &p_features, const HashMap<String, String> &p_variants, List<String> *p_last_features, HashMap<String, String> *p_last_variants);
	void get_all(List<String> *p_features, HashMap<String, String> *p_variants);

	void update_current_shader_rid(bool *p_is_new);
	void update_current_shader_rid_nolock(bool *p_is_new);

	ShaderVariantCollection();
	~ShaderVariantCollection();
};

// Helper class which keeps track of shader variant caches.
class ShaderVariantCache {

	// A cache of every single shader variant in existence right now.
	static HashMap<RID, ShaderVariantCache> stored_shader_variant_caches;
	static Mutex static_access_mutex;

	// Shader variants which branch off of the base shader, such as by enabling certain features or setting certain variants.
	// Keys are hashes created by combining all flags and variants.
	// Values are the stored shader RID.
	HashMap<uint64_t, RID> other_shader_variants;

	// The ID of the "base shader", which is the shader variation that has no features set, and all default variants.
	RID base_shader_rid;

	// How many times this specific cache is referenced, such as via multiple objects using this shader.
	uint64_t ref_count;

	void increment();
	bool decrement();

public:
	// Gets (or allocates) the RID for the provided hash representing a combination of features.
	RID get_rid_for(const uint64_t p_combo_hash, bool *p_is_new);

	// Marks the use of this instance as complete, disposing of it if nothing else is using the same shader.
	void finished();

	// To be called when the shader is set on a variant container. This increments the usage counter, so the result should be stored!
	static ShaderVariantCache *get_for_base_rid(const RID p_base_shader_rid, const uint64_t p_combo_hash);

	ShaderVariantCache();
	~ShaderVariantCache();
};

#endif // SHADER_VARIANT_COLLECTION_H
#endif
