/**************************************************************************/
/*  simulation_domain.h                                                   */
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

#ifndef SIMULATION_3D_H
#define SIMULATION_3D_H

#if !defined(PHYSICS_3D_DISABLED) && !defined(PHYSICS_2D_DISABLED) && !defined(_3D_DISABLED)

#include "core/config/project_settings.h"
#include "core/object/object.h"
#include "scene/3d/world_environment.h"
#include "scene/main/node.h"
#include "scene/main/viewport.h"
#include "scene/main/window.h"
#include "scene/resources/3d/world_3d.h"
#include "scene/resources/world_2d.h"

// #define USE_WORLD_PTR_NOT_REF

#define CONSERVATORY_VIRTUAL
#define CONSERVATORY_OVERRIDE override

#ifdef USE_WORLD_PTR_NOT_REF
#error "This causes the world pointer to be destroyed."
#define TC_DECLARE_PTR(type, name) type *name
#define TC_IS_VALID(a) (a)
#define TC_IS_NULL(a) (!a)
#define TC_INSTANTIATE(type, name) name = memnew(type)
#define TC_DELETE(a) memdelete(a)
#else
#define TC_DECLARE_PTR(type, name) Ref<type> name
#define TC_IS_VALID(a) (a.is_valid())
#define TC_IS_NULL(a) (a.is_null())
#define TC_INSTANTIATE(type, name) name.instantiate()
#define TC_DELETE(a) a = nullptr
#endif

class SimulationDomain;
class WorldEnvirionment;

class SimulationDomain : public Viewport {
	friend class Viewport;
	GDCLASS(SimulationDomain, Viewport);

	enum TheConservatoryExitCodes {

		/// <summary>
		/// This exit code is used when all is well.
		/// </summary>
		OK,

		/// <summary>
		/// The crash handler has triggered an exception. Detailed information will be
		/// in a crash dump in the game's directory.
		/// </summary>
		FATAL_GENERIC,

		/// <summary>
		/// This exit code is used when the user has hardware which is not supported.
		/// </summary>
		FATAL_UNSUPPORTED_HARDWARE,

		/// <summary>
		/// This exit code is used when the user has software which is not supported. This is primarily
		/// oriented to things like the Windows version, for example.
		/// </summary>
		FATAL_UNSUPPORTED_SOFTWARE,

		/// <summary>
		/// A mod has errored during initialization, which has resulted in a catastrophic failure
		/// that cannot be ignored (either due to the mod's instruction to hard-fail, or due to
		/// a corrupting issue caused by the mod).
		/// </summary>
		FATAL_MOD_INIT_ERROR,

		/// <summary>
		/// The game was launched and --modpack was set to some nonsensical or disallowed value.
		/// </summary>
		FATAL_GARBAGE_MODPACK_NAME,

		/// <summary>
		/// Something misused resources that are explicitly required to be used in a very specific and well-documented way,
		/// for example letting something get garbage collected that shouldn't have been garbage collected.
		/// </summary>
		FATAL_INCORRECT_RESOURCE_USAGE,

		/// <summary>
		/// This exit code is used when the user has attempted to allocate more resources than what their hardware can handle.
		/// </summary>
		FATAL_HARDWARE_LIMITS_EXCEEDED,

		/// <summary>
		/// Two or more pieces of information that were expected to have parity between each other did not have this required
		/// parity, indicating data was mishandled or another fault occurred, likely through a multithreaded environment.
		/// </summary>
		FATAL_UNEXPECTED_DATA_MISMATCH,

		/// <summary>
		/// A piece of vital information was corrupted. This crash primarily exists to prevent damage by trying to operate on
		/// broken data, preventing further (and possibly irrecoverable) corruption.
		/// </summary>
		FATAL_CORRUPTED_DATA,

		/// <summary>
		/// Some piece of code was implemented incorrectly. Typically if this error occurs, something is well-documented or has
		/// explicit instructions of how to implement it, and these instructions were not followed.
		/// </summary>
		FATAL_IMPLEMENTATION_ERROR,

		/// <summary>
		/// This is an int flag that can be added to an error code to denote it should not be reported.
		/// </summary>
		FLAG_DISALLOW_REPORTING = 0x8000,

	};

	bool predeleted = false;
	bool is_locked = false;
	bool world_instance_and_marshals_destroyed = false;
	bool created_properly = false;
	bool malformed = false;
	TC_DECLARE_PTR(World3D, world3d);
	TC_DECLARE_PTR(World2D, world2d);

	// Parameters: message, message length, context, context length, error code
	static void (*tc_crash)(const unsigned char *, int, const unsigned char *, int, int);

	// Parameters: SimulationDomain address, returns true if deletion is OK, false if not.
	static bool (*tc_destroy_validator)(const int64_t);

	// Parameters: None
	static bool (*tc_is_client)(void);

	static bool declared_cs_methods;
	//static bool simulate_physics_for_inactive;
	static List<SimulationDomain *> instances;

protected:
	void _notification(int p_what);
	static void _tc_crash(const String &p_msg, const String &p_context, int p_tc_error_code);
	static bool _tc_destroy_validator(const SimulationDomain *p_instance);
	static bool _tc_is_client();
	static void _bind_methods();

public:
	static int64_t set_conservatory_callbacks(const int64_t p_crash, const int64_t p_destroy, const int64_t p_is_client);
	static SimulationDomain *current;

	static int64_t set_conservatory_callbacks(const int64_t p_crash, const int64_t p_destroy, const int64_t p_is_client);

	bool get_is_valid() const;
	bool get_active() const;
	void set_active();
	RID get_physics_space_2d() const;
	RID get_physics_space_3d() const;
	RID get_render_canvas() const;
	RID get_render_scenario() const;
	void destroy();

	// The following code was generated by C#: ConservatoryGodotForkViewportEdit

	_FORCE_INLINE_ void _update_audio_listener_2d() override { get_parent_viewport()->_update_audio_listener_2d(); }
	_FORCE_INLINE_ void _update_global_transform() override { get_parent_viewport()->_update_global_transform(); }
	_FORCE_INLINE_ void _update_viewport_path() override { get_parent_viewport()->_update_viewport_path(); }
	_FORCE_INLINE_ void _gui_call_input(Control *p_control, const Ref<InputEvent> &p_input) override { get_parent_viewport()->_gui_call_input(p_control, p_input); }
	_FORCE_INLINE_ void _gui_call_notification(Control *p_control, int p_what) override { get_parent_viewport()->_gui_call_notification(p_control, p_what); }
	_FORCE_INLINE_ void _gui_sort_roots() override { get_parent_viewport()->_gui_sort_roots(); }
	_FORCE_INLINE_ Control *_gui_find_control_at_pos(CanvasItem *p_node, const Point2 &p_global, const Transform2D &p_xform) override { return get_parent_viewport()->_gui_find_control_at_pos(p_node, p_global, p_xform); }
	_FORCE_INLINE_ void _gui_input_event(Ref<InputEvent> p_event) override { get_parent_viewport()->_gui_input_event(p_event); }
	_FORCE_INLINE_ void _perform_drop(Control *p_control = nullptr) override { get_parent_viewport()->_perform_drop(p_control); }
	_FORCE_INLINE_ void _gui_cleanup_internal_state(Ref<InputEvent> p_event) override { get_parent_viewport()->_gui_cleanup_internal_state(p_event); }
	_FORCE_INLINE_ void _push_unhandled_input_internal(const Ref<InputEvent> &p_event) override { get_parent_viewport()->_push_unhandled_input_internal(p_event); }
	_FORCE_INLINE_ Ref<InputEvent> _make_input_local(const Ref<InputEvent> &ev) override { return get_parent_viewport()->_make_input_local(ev); }
	_FORCE_INLINE_ List<Control *>::Element *_gui_add_root_control(Control *p_control) override { return get_parent_viewport()->_gui_add_root_control(p_control); }
	_FORCE_INLINE_ void _gui_remove_root_control(List<Control *>::Element *RI) override { get_parent_viewport()->_gui_remove_root_control(RI); }
	_FORCE_INLINE_ String _gui_get_tooltip(Control *p_control, const Vector2 &p_pos, Control **r_tooltip_owner = nullptr) override { return get_parent_viewport()->_gui_get_tooltip(p_control, p_pos, r_tooltip_owner); }
	_FORCE_INLINE_ void _gui_cancel_tooltip() override { get_parent_viewport()->_gui_cancel_tooltip(); }
	_FORCE_INLINE_ void _gui_show_tooltip() override { get_parent_viewport()->_gui_show_tooltip(); }
	_FORCE_INLINE_ void _gui_show_tooltip_at(const Point2i &p_pos) override { get_parent_viewport()->_gui_show_tooltip_at(p_pos); }
	_FORCE_INLINE_ void _gui_remove_control(Control *p_control) override { get_parent_viewport()->_gui_remove_control(p_control); }
	_FORCE_INLINE_ void _gui_hide_control(Control *p_control) override { get_parent_viewport()->_gui_hide_control(p_control); }
	_FORCE_INLINE_ void _gui_update_mouse_over() override { get_parent_viewport()->_gui_update_mouse_over(); }
	_FORCE_INLINE_ void _gui_force_drag_start() override { get_parent_viewport()->_gui_force_drag_start(); }
	_FORCE_INLINE_ void _gui_force_drag_cancel() override { get_parent_viewport()->_gui_force_drag_cancel(); }
	_FORCE_INLINE_ void _gui_force_drag(Control *p_base, const Variant &p_data, Control *p_control) override { get_parent_viewport()->_gui_force_drag(p_base, p_data, p_control); }
	_FORCE_INLINE_ void _gui_set_drag_preview(Control *p_base, Control *p_control) override { get_parent_viewport()->_gui_set_drag_preview(p_base, p_control); }
	_FORCE_INLINE_ Control *_gui_get_drag_preview() override { return get_parent_viewport()->_gui_get_drag_preview(); }
	_FORCE_INLINE_ void _gui_remove_focus_for_window(Node *p_window) override { get_parent_viewport()->_gui_remove_focus_for_window(p_window); }
	_FORCE_INLINE_ void _gui_unfocus_control(Control *p_control) override { get_parent_viewport()->_gui_unfocus_control(p_control); }
	_FORCE_INLINE_ bool _gui_control_has_focus(const Control *p_control) override { return get_parent_viewport()->_gui_control_has_focus(p_control); }
	_FORCE_INLINE_ void _gui_control_grab_focus(Control *p_control) override { get_parent_viewport()->_gui_control_grab_focus(p_control); }
	_FORCE_INLINE_ void _gui_grab_click_focus(Control *p_control) override { get_parent_viewport()->_gui_grab_click_focus(p_control); }
	_FORCE_INLINE_ void _post_gui_grab_click_focus() override { get_parent_viewport()->_post_gui_grab_click_focus(); }
	_FORCE_INLINE_ void _gui_accept_event() override { get_parent_viewport()->_gui_accept_event(); }
	_FORCE_INLINE_ bool _gui_drop(Control *p_at_control, Point2 p_at_pos, bool p_just_check) override { return get_parent_viewport()->_gui_drop(p_at_control, p_at_pos, p_just_check); }
	_FORCE_INLINE_ void _canvas_layer_add(CanvasLayer *p_canvas_layer) override { get_parent_viewport()->_canvas_layer_add(p_canvas_layer); }
	_FORCE_INLINE_ void _canvas_layer_remove(CanvasLayer *p_canvas_layer) override { get_parent_viewport()->_canvas_layer_remove(p_canvas_layer); }
	_FORCE_INLINE_ void _drop_mouse_over(Control *p_until_control = nullptr) override { get_parent_viewport()->_drop_mouse_over(p_until_control); }
	_FORCE_INLINE_ void _drop_mouse_focus() override { get_parent_viewport()->_drop_mouse_focus(); }
	_FORCE_INLINE_ void _drop_physics_mouseover(bool p_paused_only = false) override { get_parent_viewport()->_drop_physics_mouseover(p_paused_only); }
	_FORCE_INLINE_ void _update_canvas_items(Node *p_node) override { get_parent_viewport()->_update_canvas_items(p_node); }
	_FORCE_INLINE_ void _sub_window_update_order() override { get_parent_viewport()->_sub_window_update_order(); }
	_FORCE_INLINE_ void _sub_window_register(Window *p_window) override { get_parent_viewport()->_sub_window_register(p_window); }
	_FORCE_INLINE_ void _sub_window_update(Window *p_window) override { get_parent_viewport()->_sub_window_update(p_window); }
	_FORCE_INLINE_ void _sub_window_grab_focus(Window *p_window) override { get_parent_viewport()->_sub_window_grab_focus(p_window); }
	_FORCE_INLINE_ void _sub_window_remove(Window *p_window) override { get_parent_viewport()->_sub_window_remove(p_window); }
	_FORCE_INLINE_ int _sub_window_find(Window *p_window) const override { return get_parent_viewport()->_sub_window_find(p_window); }
	_FORCE_INLINE_ bool _sub_windows_forward_input(const Ref<InputEvent> &p_event) override { return get_parent_viewport()->_sub_windows_forward_input(p_event); }
	_FORCE_INLINE_ SubWindowResize _sub_window_get_resize_margin(Window *p_subwindow, const Point2 &p_point) override { return get_parent_viewport()->_sub_window_get_resize_margin(p_subwindow, p_point); }
	_FORCE_INLINE_ void _update_mouse_over() override { get_parent_viewport()->_update_mouse_over(); }
	_FORCE_INLINE_ void _update_mouse_over(Vector2 p_pos) override { get_parent_viewport()->_update_mouse_over(p_pos); }
	_FORCE_INLINE_ void _mouse_leave_viewport() override { get_parent_viewport()->_mouse_leave_viewport(); }
	_FORCE_INLINE_ bool _can_consume_input_events() const override { return get_parent_viewport()->_can_consume_input_events(); }
	_FORCE_INLINE_ void _process_dirty_canvas_parent_orders() override { get_parent_viewport()->_process_dirty_canvas_parent_orders(); }
	_FORCE_INLINE_ void _propagate_world_2d_changed(Node *p_node) override { get_parent_viewport()->_propagate_world_2d_changed(p_node); }
	_FORCE_INLINE_ void _window_start_drag(Window *p_window) override { get_parent_viewport()->_window_start_drag(p_window); }
	_FORCE_INLINE_ void _window_start_resize(SubWindowResize p_edge, Window *p_window) override { get_parent_viewport()->_window_start_resize(p_edge, p_window); }
	_FORCE_INLINE_ bool _set_size(const Size2i &p_size, const Size2 &p_size_2d_override, bool p_allocated) override { return get_parent_viewport()->_set_size(p_size, p_size_2d_override, p_allocated); }
	_FORCE_INLINE_ Size2i _get_size() const override { return get_parent_viewport()->_get_size(); }
	_FORCE_INLINE_ Size2 _get_size_2d_override() const override { return get_parent_viewport()->_get_size_2d_override(); }
	_FORCE_INLINE_ bool _is_size_allocated() const override { return get_parent_viewport()->_is_size_allocated(); }
	_FORCE_INLINE_ void _process_picking() override { get_parent_viewport()->_process_picking(); }
	_FORCE_INLINE_ void _validate_property(PropertyInfo &p_property) const override { get_parent_viewport()->_validate_property(p_property); }
	_FORCE_INLINE_ void canvas_parent_mark_dirty(Node *p_node) override { get_parent_viewport()->canvas_parent_mark_dirty(p_node); }
	_FORCE_INLINE_ void canvas_item_top_level_changed() override { get_parent_viewport()->canvas_item_top_level_changed(); }
	_FORCE_INLINE_ uint64_t get_processed_events_count() const override { return get_parent_viewport()->get_processed_events_count(); }
	_FORCE_INLINE_ void cancel_tooltip() override { get_parent_viewport()->cancel_tooltip(); }
	_FORCE_INLINE_ void show_tooltip(Control *p_control) override { get_parent_viewport()->show_tooltip(p_control); }
	_FORCE_INLINE_ void update_canvas_items() override { get_parent_viewport()->update_canvas_items(); }
	_FORCE_INLINE_ Rect2 get_visible_rect() const override { return get_parent_viewport()->get_visible_rect(); }
	_FORCE_INLINE_ RID get_viewport_rid() const override { return get_parent_viewport()->get_viewport_rid(); }
	void set_world_2d(const Ref<World2D> &p_world_2d) override;
	Ref<World2D> get_world_2d() const override;
	Ref<World2D> find_world_2d() const override;
	_FORCE_INLINE_ void enable_canvas_transform_override(bool p_enable) override { get_parent_viewport()->enable_canvas_transform_override(p_enable); }
	_FORCE_INLINE_ bool is_canvas_transform_override_enabled() const override { return get_parent_viewport()->is_canvas_transform_override_enabled(); }
	_FORCE_INLINE_ void set_canvas_transform_override(const Transform2D &p_transform) override { get_parent_viewport()->set_canvas_transform_override(p_transform); }
	_FORCE_INLINE_ Transform2D get_canvas_transform_override() const override { return get_parent_viewport()->get_canvas_transform_override(); }
	_FORCE_INLINE_ void set_canvas_transform(const Transform2D &p_transform) override { get_parent_viewport()->set_canvas_transform(p_transform); }
	_FORCE_INLINE_ Transform2D get_canvas_transform() const override { return get_parent_viewport()->get_canvas_transform(); }
	_FORCE_INLINE_ void set_global_canvas_transform(const Transform2D &p_transform) override { get_parent_viewport()->set_global_canvas_transform(p_transform); }
	_FORCE_INLINE_ Transform2D get_global_canvas_transform() const override { return get_parent_viewport()->get_global_canvas_transform(); }
	_FORCE_INLINE_ Transform2D get_stretch_transform() const override { return get_parent_viewport()->get_stretch_transform(); }
	_FORCE_INLINE_ Transform2D get_final_transform() const override { return get_parent_viewport()->get_final_transform(); }
	_FORCE_INLINE_ void gui_set_root_order_dirty() override { get_parent_viewport()->gui_set_root_order_dirty(); }
	_FORCE_INLINE_ void set_transparent_background(bool p_enable) override { get_parent_viewport()->set_transparent_background(p_enable); }
	_FORCE_INLINE_ bool has_transparent_background() const override { return get_parent_viewport()->has_transparent_background(); }
	_FORCE_INLINE_ void set_use_hdr_2d(bool p_enable) override { get_parent_viewport()->set_use_hdr_2d(p_enable); }
	_FORCE_INLINE_ bool is_using_hdr_2d() const override { return get_parent_viewport()->is_using_hdr_2d(); }
	_FORCE_INLINE_ Ref<ViewportTexture> get_texture() const override { return get_parent_viewport()->get_texture(); }
	_FORCE_INLINE_ void set_positional_shadow_atlas_size(int p_size) override { get_parent_viewport()->set_positional_shadow_atlas_size(p_size); }
	_FORCE_INLINE_ int get_positional_shadow_atlas_size() const override { return get_parent_viewport()->get_positional_shadow_atlas_size(); }
	_FORCE_INLINE_ void set_positional_shadow_atlas_16_bits(bool p_16_bits) override { get_parent_viewport()->set_positional_shadow_atlas_16_bits(p_16_bits); }
	_FORCE_INLINE_ bool get_positional_shadow_atlas_16_bits() const override { return get_parent_viewport()->get_positional_shadow_atlas_16_bits(); }
	_FORCE_INLINE_ void set_positional_shadow_atlas_quadrant_subdiv(int p_quadrant, PositionalShadowAtlasQuadrantSubdiv p_subdiv) override { get_parent_viewport()->set_positional_shadow_atlas_quadrant_subdiv(p_quadrant, p_subdiv); }
	_FORCE_INLINE_ PositionalShadowAtlasQuadrantSubdiv get_positional_shadow_atlas_quadrant_subdiv(int p_quadrant) const override { return get_parent_viewport()->get_positional_shadow_atlas_quadrant_subdiv(p_quadrant); }
	_FORCE_INLINE_ void set_msaa_2d(MSAA p_msaa) override { get_parent_viewport()->set_msaa_2d(p_msaa); }
	_FORCE_INLINE_ MSAA get_msaa_2d() const override { return get_parent_viewport()->get_msaa_2d(); }
	_FORCE_INLINE_ void set_msaa_3d(MSAA p_msaa) override { get_parent_viewport()->set_msaa_3d(p_msaa); }
	_FORCE_INLINE_ MSAA get_msaa_3d() const override { return get_parent_viewport()->get_msaa_3d(); }
	_FORCE_INLINE_ void set_screen_space_aa(ScreenSpaceAA p_screen_space_aa) override { get_parent_viewport()->set_screen_space_aa(p_screen_space_aa); }
	_FORCE_INLINE_ ScreenSpaceAA get_screen_space_aa() const override { return get_parent_viewport()->get_screen_space_aa(); }
	_FORCE_INLINE_ void set_use_taa(bool p_use_taa) override { get_parent_viewport()->set_use_taa(p_use_taa); }
	_FORCE_INLINE_ bool is_using_taa() const override { return get_parent_viewport()->is_using_taa(); }
	_FORCE_INLINE_ void set_use_oversampling(bool p_oversampling) override { get_parent_viewport()->set_use_oversampling(p_oversampling); }
	_FORCE_INLINE_ bool is_using_oversampling() const override { return get_parent_viewport()->is_using_oversampling(); }
	_FORCE_INLINE_ void set_oversampling_override(float p_oversampling) override { get_parent_viewport()->set_oversampling_override(p_oversampling); }
	_FORCE_INLINE_ float get_oversampling_override() const override { return get_parent_viewport()->get_oversampling_override(); }
	_FORCE_INLINE_ float get_oversampling() const override { return get_parent_viewport()->get_oversampling(); }
	_FORCE_INLINE_ void set_scaling_3d_mode(Scaling3DMode p_scaling_3d_mode) override { get_parent_viewport()->set_scaling_3d_mode(p_scaling_3d_mode); }
	_FORCE_INLINE_ Scaling3DMode get_scaling_3d_mode() const override { return get_parent_viewport()->get_scaling_3d_mode(); }
	_FORCE_INLINE_ void set_scaling_3d_scale(float p_scaling_3d_scale) override { get_parent_viewport()->set_scaling_3d_scale(p_scaling_3d_scale); }
	_FORCE_INLINE_ float get_scaling_3d_scale() const override { return get_parent_viewport()->get_scaling_3d_scale(); }
	_FORCE_INLINE_ void set_fsr_sharpness(float p_fsr_sharpness) override { get_parent_viewport()->set_fsr_sharpness(p_fsr_sharpness); }
	_FORCE_INLINE_ float get_fsr_sharpness() const override { return get_parent_viewport()->get_fsr_sharpness(); }
	_FORCE_INLINE_ void set_texture_mipmap_bias(float p_texture_mipmap_bias) override { get_parent_viewport()->set_texture_mipmap_bias(p_texture_mipmap_bias); }
	_FORCE_INLINE_ float get_texture_mipmap_bias() const override { return get_parent_viewport()->get_texture_mipmap_bias(); }
	_FORCE_INLINE_ void set_anisotropic_filtering_level(AnisotropicFiltering p_anisotropic_filtering_level) override { get_parent_viewport()->set_anisotropic_filtering_level(p_anisotropic_filtering_level); }
	_FORCE_INLINE_ AnisotropicFiltering get_anisotropic_filtering_level() const override { return get_parent_viewport()->get_anisotropic_filtering_level(); }
	_FORCE_INLINE_ void set_use_debanding(bool p_use_debanding) override { get_parent_viewport()->set_use_debanding(p_use_debanding); }
	_FORCE_INLINE_ bool is_using_debanding() const override { return get_parent_viewport()->is_using_debanding(); }
	_FORCE_INLINE_ void set_mesh_lod_threshold(float p_pixels) override { get_parent_viewport()->set_mesh_lod_threshold(p_pixels); }
	_FORCE_INLINE_ float get_mesh_lod_threshold() const override { return get_parent_viewport()->get_mesh_lod_threshold(); }
	_FORCE_INLINE_ void set_use_occlusion_culling(bool p_us_occlusion_culling) override { get_parent_viewport()->set_use_occlusion_culling(p_us_occlusion_culling); }
	_FORCE_INLINE_ bool is_using_occlusion_culling() const override { return get_parent_viewport()->is_using_occlusion_culling(); }
	_FORCE_INLINE_ Vector2 get_camera_coords(const Vector2 &p_viewport_coords) const override { return get_parent_viewport()->get_camera_coords(p_viewport_coords); }
	_FORCE_INLINE_ Vector2 get_camera_rect_size() const override { return get_parent_viewport()->get_camera_rect_size(); }
	_FORCE_INLINE_ void push_text_input(const String &p_text) override { get_parent_viewport()->push_text_input(p_text); }
	_FORCE_INLINE_ void push_input(const Ref<InputEvent> &p_event, bool p_local_coords = false) override { get_parent_viewport()->push_input(p_event, p_local_coords); }
#ifndef DISABLE_DEPRECATED
	_FORCE_INLINE_ void push_unhandled_input(const Ref<InputEvent> &p_event, bool p_local_coords = false) override { get_parent_viewport()->push_unhandled_input(p_event, p_local_coords); }
#endif
	_FORCE_INLINE_ void notify_mouse_entered() override { get_parent_viewport()->notify_mouse_entered(); }
	_FORCE_INLINE_ void notify_mouse_exited() override { get_parent_viewport()->notify_mouse_exited(); }
	_FORCE_INLINE_ void set_disable_input(bool p_disable) override { get_parent_viewport()->set_disable_input(p_disable); }
	_FORCE_INLINE_ bool is_input_disabled() const override { return get_parent_viewport()->is_input_disabled(); }
	_FORCE_INLINE_ void set_disable_input_override(bool p_disable) override { get_parent_viewport()->set_disable_input_override(p_disable); }
	_FORCE_INLINE_ Vector2 get_mouse_position() const override { return get_parent_viewport()->get_mouse_position(); }
	_FORCE_INLINE_ void warp_mouse(const Vector2 &p_position) override { get_parent_viewport()->warp_mouse(p_position); }
	_FORCE_INLINE_ Point2 wrap_mouse_in_rect(const Vector2 &p_relative, const Rect2 &p_rect) override { return get_parent_viewport()->wrap_mouse_in_rect(p_relative, p_rect); }
	_FORCE_INLINE_ void update_mouse_cursor_state() override { get_parent_viewport()->update_mouse_cursor_state(); }
	_FORCE_INLINE_ void set_physics_object_picking(bool p_enable) override { get_parent_viewport()->set_physics_object_picking(p_enable); }
	_FORCE_INLINE_ bool get_physics_object_picking() override { return get_parent_viewport()->get_physics_object_picking(); }
	_FORCE_INLINE_ void set_physics_object_picking_sort(bool p_enable) override { get_parent_viewport()->set_physics_object_picking_sort(p_enable); }
	_FORCE_INLINE_ bool get_physics_object_picking_sort() override { return get_parent_viewport()->get_physics_object_picking_sort(); }
	_FORCE_INLINE_ void set_physics_object_picking_first_only(bool p_enable) override { get_parent_viewport()->set_physics_object_picking_first_only(p_enable); }
	_FORCE_INLINE_ bool get_physics_object_picking_first_only() override { return get_parent_viewport()->get_physics_object_picking_first_only(); }
	_FORCE_INLINE_ Variant gui_get_drag_data() const override { return get_parent_viewport()->gui_get_drag_data(); }
	_FORCE_INLINE_ String gui_get_drag_description() const override { return get_parent_viewport()->gui_get_drag_description(); }
	_FORCE_INLINE_ void gui_set_drag_description(const String &p_description) override { get_parent_viewport()->gui_set_drag_description(p_description); }
	_FORCE_INLINE_ void gui_reset_canvas_sort_index() override { get_parent_viewport()->gui_reset_canvas_sort_index(); }
	_FORCE_INLINE_ int gui_get_canvas_sort_index() override { return get_parent_viewport()->gui_get_canvas_sort_index(); }
	_FORCE_INLINE_ void gui_release_focus() override { get_parent_viewport()->gui_release_focus(); }
	_FORCE_INLINE_ Control *gui_get_focus_owner() const override { return get_parent_viewport()->gui_get_focus_owner(); }
	_FORCE_INLINE_ Control *gui_get_hovered_control() const override { return get_parent_viewport()->gui_get_hovered_control(); }
	_FORCE_INLINE_ Window *get_focused_subwindow() const override { return get_parent_viewport()->get_focused_subwindow(); }
	_FORCE_INLINE_ void set_debug_draw(DebugDraw p_debug_draw) override { get_parent_viewport()->set_debug_draw(p_debug_draw); }
	_FORCE_INLINE_ DebugDraw get_debug_draw() const override { return get_parent_viewport()->get_debug_draw(); }
	_FORCE_INLINE_ int get_render_info(RenderInfoType p_type, RenderInfo p_info) override { return get_parent_viewport()->get_render_info(p_type, p_info); }
	_FORCE_INLINE_ void set_snap_controls_to_pixels(bool p_enable) override { get_parent_viewport()->set_snap_controls_to_pixels(p_enable); }
	_FORCE_INLINE_ bool is_snap_controls_to_pixels_enabled() const override { return get_parent_viewport()->is_snap_controls_to_pixels_enabled(); }
	_FORCE_INLINE_ void set_snap_2d_transforms_to_pixel(bool p_enable) override { get_parent_viewport()->set_snap_2d_transforms_to_pixel(p_enable); }
	_FORCE_INLINE_ bool is_snap_2d_transforms_to_pixel_enabled() const override { return get_parent_viewport()->is_snap_2d_transforms_to_pixel_enabled(); }
	_FORCE_INLINE_ void set_snap_2d_vertices_to_pixel(bool p_enable) override { get_parent_viewport()->set_snap_2d_vertices_to_pixel(p_enable); }
	_FORCE_INLINE_ bool is_snap_2d_vertices_to_pixel_enabled() const override { return get_parent_viewport()->is_snap_2d_vertices_to_pixel_enabled(); }
	_FORCE_INLINE_ void set_input_as_handled() override { get_parent_viewport()->set_input_as_handled(); }
	_FORCE_INLINE_ bool is_input_handled() const override { return get_parent_viewport()->is_input_handled(); }
	_FORCE_INLINE_ void set_handle_input_locally(bool p_enable) override { get_parent_viewport()->set_handle_input_locally(p_enable); }
	_FORCE_INLINE_ bool is_handling_input_locally() const override { return get_parent_viewport()->is_handling_input_locally(); }
	_FORCE_INLINE_ bool gui_is_dragging() const override { return get_parent_viewport()->gui_is_dragging(); }
	_FORCE_INLINE_ bool gui_is_drag_successful() const override { return get_parent_viewport()->gui_is_drag_successful(); }
	_FORCE_INLINE_ void gui_cancel_drag() override { get_parent_viewport()->gui_cancel_drag(); }
	_FORCE_INLINE_ void gui_perform_drop_at(const Point2 &p_pos, Control *p_control = nullptr) override { get_parent_viewport()->gui_perform_drop_at(p_pos, p_control); }
	_FORCE_INLINE_ Control *gui_find_control(const Point2 &p_global) override { return get_parent_viewport()->gui_find_control(p_global); }
	_FORCE_INLINE_ void set_sdf_oversize(SDFOversize p_sdf_oversize) override { get_parent_viewport()->set_sdf_oversize(p_sdf_oversize); }
	_FORCE_INLINE_ SDFOversize get_sdf_oversize() const override { return get_parent_viewport()->get_sdf_oversize(); }
	_FORCE_INLINE_ void set_sdf_scale(SDFScale p_sdf_scale) override { get_parent_viewport()->set_sdf_scale(p_sdf_scale); }
	_FORCE_INLINE_ SDFScale get_sdf_scale() const override { return get_parent_viewport()->get_sdf_scale(); }
	_FORCE_INLINE_ void set_default_canvas_item_texture_filter(DefaultCanvasItemTextureFilter p_filter) override { get_parent_viewport()->set_default_canvas_item_texture_filter(p_filter); }
	_FORCE_INLINE_ DefaultCanvasItemTextureFilter get_default_canvas_item_texture_filter() const override { return get_parent_viewport()->get_default_canvas_item_texture_filter(); }
	_FORCE_INLINE_ void set_default_canvas_item_texture_repeat(DefaultCanvasItemTextureRepeat p_repeat) override { get_parent_viewport()->set_default_canvas_item_texture_repeat(p_repeat); }
	_FORCE_INLINE_ DefaultCanvasItemTextureRepeat get_default_canvas_item_texture_repeat() const override { return get_parent_viewport()->get_default_canvas_item_texture_repeat(); }
	_FORCE_INLINE_ void set_vrs_mode(VRSMode p_vrs_mode) override { get_parent_viewport()->set_vrs_mode(p_vrs_mode); }
	_FORCE_INLINE_ VRSMode get_vrs_mode() const override { return get_parent_viewport()->get_vrs_mode(); }
	_FORCE_INLINE_ void set_vrs_update_mode(VRSUpdateMode p_vrs_update_mode) override { get_parent_viewport()->set_vrs_update_mode(p_vrs_update_mode); }
	_FORCE_INLINE_ VRSUpdateMode get_vrs_update_mode() const override { return get_parent_viewport()->get_vrs_update_mode(); }
	_FORCE_INLINE_ void set_vrs_texture(Ref<Texture2D> p_texture) override { get_parent_viewport()->set_vrs_texture(p_texture); }
	_FORCE_INLINE_ Ref<Texture2D> get_vrs_texture() const override { return get_parent_viewport()->get_vrs_texture(); }
	_FORCE_INLINE_ DisplayServer::WindowID get_window_id() const override { return get_parent_viewport()->get_window_id(); }
	_FORCE_INLINE_ void set_embedding_subwindows(bool p_embed) override { get_parent_viewport()->set_embedding_subwindows(p_embed); }
	_FORCE_INLINE_ bool is_embedding_subwindows() const override { return get_parent_viewport()->is_embedding_subwindows(); }
	_FORCE_INLINE_ TypedArray<Window> get_embedded_subwindows() const override { return get_parent_viewport()->get_embedded_subwindows(); }
	_FORCE_INLINE_ void subwindow_set_popup_safe_rect(Window *p_window, const Rect2i &p_rect) override { get_parent_viewport()->subwindow_set_popup_safe_rect(p_window, p_rect); }
	_FORCE_INLINE_ Rect2i subwindow_get_popup_safe_rect(Window *p_window) const override { return get_parent_viewport()->subwindow_get_popup_safe_rect(p_window); }
	_FORCE_INLINE_ Window *get_base_window() override { return get_parent_viewport()->get_base_window(); }
	_FORCE_INLINE_ void set_canvas_cull_mask(uint32_t p_layers) override { get_parent_viewport()->set_canvas_cull_mask(p_layers); }
	_FORCE_INLINE_ uint32_t get_canvas_cull_mask() const override { return get_parent_viewport()->get_canvas_cull_mask(); }
	_FORCE_INLINE_ void set_canvas_cull_mask_bit(uint32_t p_layer, bool p_enable) override { get_parent_viewport()->set_canvas_cull_mask_bit(p_layer, p_enable); }
	_FORCE_INLINE_ bool get_canvas_cull_mask_bit(uint32_t p_layer) const override { return get_parent_viewport()->get_canvas_cull_mask_bit(p_layer); }
	_FORCE_INLINE_ bool is_visible_subviewport() const override { return get_parent_viewport()->is_visible_subviewport(); }
	_FORCE_INLINE_ bool is_size_2d_override_stretch_enabled() const override { return get_parent_viewport()->is_size_2d_override_stretch_enabled(); }
	_FORCE_INLINE_ Transform2D get_screen_transform() const override { return get_parent_viewport()->get_screen_transform(); }
	_FORCE_INLINE_ Transform2D get_screen_transform_internal(bool p_absolute_position = false) const override { return get_parent_viewport()->get_screen_transform_internal(p_absolute_position); }
	_FORCE_INLINE_ Transform2D get_popup_base_transform() const override { return get_parent_viewport()->get_popup_base_transform(); }
	_FORCE_INLINE_ Viewport *get_section_root_viewport() const override { return get_parent_viewport()->get_section_root_viewport(); }
	_FORCE_INLINE_ bool is_attached_in_viewport() const override { return get_parent_viewport()->is_attached_in_viewport(); }
	_FORCE_INLINE_ bool is_sub_viewport() const override { return get_parent_viewport()->is_sub_viewport(); }
	_FORCE_INLINE_ void _audio_listener_2d_set(AudioListener2D *p_audio_listener) override { get_parent_viewport()->_audio_listener_2d_set(p_audio_listener); }
	_FORCE_INLINE_ void _audio_listener_2d_remove(AudioListener2D *p_audio_listener) override { get_parent_viewport()->_audio_listener_2d_remove(p_audio_listener); }
	_FORCE_INLINE_ void _camera_2d_set(Camera2D *p_camera_2d) override { get_parent_viewport()->_camera_2d_set(p_camera_2d); }
	_FORCE_INLINE_ void _cleanup_mouseover_colliders(bool p_clean_all_frames, bool p_paused_only, uint64_t p_frame_reference = 0) override { get_parent_viewport()->_cleanup_mouseover_colliders(p_clean_all_frames, p_paused_only, p_frame_reference); }
	_FORCE_INLINE_ AudioListener2D *get_audio_listener_2d() const override { return get_parent_viewport()->get_audio_listener_2d(); }
	_FORCE_INLINE_ void set_as_audio_listener_2d(bool p_enable) override { get_parent_viewport()->set_as_audio_listener_2d(p_enable); }
	_FORCE_INLINE_ bool is_audio_listener_2d() const override { return get_parent_viewport()->is_audio_listener_2d(); }
	_FORCE_INLINE_ Camera2D *get_camera_2d() const override { return get_parent_viewport()->get_camera_2d(); }
	_FORCE_INLINE_ void assign_next_enabled_camera_2d(const StringName &p_camera_group) override { get_parent_viewport()->assign_next_enabled_camera_2d(p_camera_group); }
	_FORCE_INLINE_ void _update_audio_listener_3d() override { get_parent_viewport()->_update_audio_listener_3d(); }
	_FORCE_INLINE_ void _listener_transform_3d_changed_notify() override { get_parent_viewport()->_listener_transform_3d_changed_notify(); }
	_FORCE_INLINE_ void _audio_listener_3d_set(AudioListener3D *p_listener) override { get_parent_viewport()->_audio_listener_3d_set(p_listener); }
	_FORCE_INLINE_ bool _audio_listener_3d_add(AudioListener3D *p_listener) override { return get_parent_viewport()->_audio_listener_3d_add(p_listener); }
	_FORCE_INLINE_ void _audio_listener_3d_remove(AudioListener3D *p_listener) override { get_parent_viewport()->_audio_listener_3d_remove(p_listener); }
	_FORCE_INLINE_ void _audio_listener_3d_make_next_current(AudioListener3D *p_exclude) override { get_parent_viewport()->_audio_listener_3d_make_next_current(p_exclude); }
	_FORCE_INLINE_ void _collision_object_3d_input_event(CollisionObject3D *p_object, Camera3D *p_camera, const Ref<InputEvent> &p_input_event, const Vector3 &p_pos, const Vector3 &p_normal, int p_shape) override { get_parent_viewport()->_collision_object_3d_input_event(p_object, p_camera, p_input_event, p_pos, p_normal, p_shape); }
	_FORCE_INLINE_ void _camera_3d_transform_changed_notify() override { get_parent_viewport()->_camera_3d_transform_changed_notify(); }
	_FORCE_INLINE_ void _camera_3d_set(Camera3D *p_camera) override { get_parent_viewport()->_camera_3d_set(p_camera); }
	_FORCE_INLINE_ bool _camera_3d_add(Camera3D *p_camera) override { return get_parent_viewport()->_camera_3d_add(p_camera); }
	_FORCE_INLINE_ void _camera_3d_remove(Camera3D *p_camera) override { get_parent_viewport()->_camera_3d_remove(p_camera); }
	_FORCE_INLINE_ void _camera_3d_make_next_current(Camera3D *p_exclude) override { get_parent_viewport()->_camera_3d_make_next_current(p_exclude); }
	_FORCE_INLINE_ void _own_world_3d_changed() override { get_parent_viewport()->_own_world_3d_changed(); }
	_FORCE_INLINE_ void _propagate_enter_world_3d(Node *p_node) override { get_parent_viewport()->_propagate_enter_world_3d(p_node); }
	_FORCE_INLINE_ void _propagate_exit_world_3d(Node *p_node) override { get_parent_viewport()->_propagate_exit_world_3d(p_node); }
	_FORCE_INLINE_ AudioListener3D *get_audio_listener_3d() const override { return get_parent_viewport()->get_audio_listener_3d(); }
	_FORCE_INLINE_ void set_as_audio_listener_3d(bool p_enable) override { get_parent_viewport()->set_as_audio_listener_3d(p_enable); }
	_FORCE_INLINE_ bool is_audio_listener_3d() const override { return get_parent_viewport()->is_audio_listener_3d(); }
	_FORCE_INLINE_ Camera3D *get_camera_3d() const override { return get_parent_viewport()->get_camera_3d(); }
	_FORCE_INLINE_ void enable_camera_3d_override(bool p_enable) override { get_parent_viewport()->enable_camera_3d_override(p_enable); }
	_FORCE_INLINE_ bool is_camera_3d_override_enabled() const override { return get_parent_viewport()->is_camera_3d_override_enabled(); }
	_FORCE_INLINE_ void set_camera_3d_override_transform(const Transform3D &p_transform) override { get_parent_viewport()->set_camera_3d_override_transform(p_transform); }
	_FORCE_INLINE_ Transform3D get_camera_3d_override_transform() const override { return get_parent_viewport()->get_camera_3d_override_transform(); }
	_FORCE_INLINE_ void set_camera_3d_override_perspective(real_t p_fovy_degrees, real_t p_z_near, real_t p_z_far) override { get_parent_viewport()->set_camera_3d_override_perspective(p_fovy_degrees, p_z_near, p_z_far); }
	_FORCE_INLINE_ void set_camera_3d_override_orthogonal(real_t p_size, real_t p_z_near, real_t p_z_far) override { get_parent_viewport()->set_camera_3d_override_orthogonal(p_size, p_z_near, p_z_far); }
	_FORCE_INLINE_ HashMap<StringName, real_t> get_camera_3d_override_properties() const override { return get_parent_viewport()->get_camera_3d_override_properties(); }
	_FORCE_INLINE_ Vector3 camera_3d_override_project_ray_normal(const Point2 &p_pos) const override { return get_parent_viewport()->camera_3d_override_project_ray_normal(p_pos); }
	_FORCE_INLINE_ Vector3 camera_3d_override_project_ray_origin(const Point2 &p_pos) const override { return get_parent_viewport()->camera_3d_override_project_ray_origin(p_pos); }
	_FORCE_INLINE_ Vector3 camera_3d_override_project_local_ray_normal(const Point2 &p_pos) const override { return get_parent_viewport()->camera_3d_override_project_local_ray_normal(p_pos); }
	_FORCE_INLINE_ void set_disable_3d(bool p_disable) override { get_parent_viewport()->set_disable_3d(p_disable); }
	_FORCE_INLINE_ bool is_3d_disabled() const override { return get_parent_viewport()->is_3d_disabled(); }
	void set_world_3d(const Ref<World3D> &p_world_3d) override;
	Ref<World3D> get_world_3d() const override;
	Ref<World3D> find_world_3d() const override;
	void set_use_own_world_3d(bool p_use_own_world_3d) override;
	bool is_using_own_world_3d() const override;
	_FORCE_INLINE_ void set_use_xr(bool p_use_xr) override { get_parent_viewport()->set_use_xr(p_use_xr); }
	_FORCE_INLINE_ bool is_using_xr() override { return get_parent_viewport()->is_using_xr(); }

	/*

	CONSERVATORY_VIRTUAL void _gui_remove_focus_for_window(Node *p_window) CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void _process_picking() CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void canvas_parent_mark_dirty(Node *p_node) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void canvas_item_top_level_changed() CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL uint64_t get_processed_events_count() const CONSERVATORY_OVERRIDE { return event_count; }

	CONSERVATORY_VIRTUAL void cancel_tooltip() CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void show_tooltip(Control *p_control) CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL Rect2 get_visible_rect() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL RID get_viewport_rid() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_world_2d(const Ref<World2D> &p_world_2d) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Ref<World2D> get_world_2d() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Ref<World2D> find_world_2d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_canvas_transform(const Transform2D &p_transform) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Transform2D get_canvas_transform() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_global_canvas_transform(const Transform2D &p_transform) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Transform2D get_global_canvas_transform() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL Transform2D get_stretch_transform() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL virtual Transform2D get_final_transform() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_transparent_background(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool has_transparent_background() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_use_hdr_2d(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_using_hdr_2d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL Ref<ViewportTexture> get_texture() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_positional_shadow_atlas_size(int p_size) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL int get_positional_shadow_atlas_size() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_positional_shadow_atlas_16_bits(bool p_16_bits) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool get_positional_shadow_atlas_16_bits() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_positional_shadow_atlas_quadrant_subdiv(int p_quadrant, PositionalShadowAtlasQuadrantSubdiv p_subdiv) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL PositionalShadowAtlasQuadrantSubdiv get_positional_shadow_atlas_quadrant_subdiv(int p_quadrant) const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_msaa_2d(MSAA p_msaa) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL MSAA get_msaa_2d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_msaa_3d(MSAA p_msaa) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL MSAA get_msaa_3d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_screen_space_aa(ScreenSpaceAA p_screen_space_aa) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL ScreenSpaceAA get_screen_space_aa() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_use_taa(bool p_use_taa) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_using_taa() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_use_oversampling(bool p_oversampling) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_using_oversampling() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_oversampling_override(float p_oversampling) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL float get_oversampling_override() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL float get_oversampling() const CONSERVATORY_OVERRIDE { return Viewport::font_oversampling; }

	CONSERVATORY_VIRTUAL void set_scaling_3d_mode(Scaling3DMode p_scaling_3d_mode) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Scaling3DMode get_scaling_3d_mode() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_scaling_3d_scale(float p_scaling_3d_scale) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL float get_scaling_3d_scale() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_fsr_sharpness(float p_fsr_sharpness) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL float get_fsr_sharpness() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_texture_mipmap_bias(float p_texture_mipmap_bias) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL float get_texture_mipmap_bias() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_anisotropic_filtering_level(AnisotropicFiltering p_anisotropic_filtering_level) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL AnisotropicFiltering get_anisotropic_filtering_level() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_use_debanding(bool p_use_debanding) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_using_debanding() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_mesh_lod_threshold(float p_pixels) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL float get_mesh_lod_threshold() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_use_occlusion_culling(bool p_us_occlusion_culling) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_using_occlusion_culling() const CONSERVATORY_OVERRIDE;



	CONSERVATORY_VIRTUAL void push_text_input(const String &p_text) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void push_input(const Ref<InputEvent> &p_event, bool p_local_coords = false) CONSERVATORY_OVERRIDE;
#ifndef DISABLE_DEPRECATED
	CONSERVATORY_VIRTUAL void push_unhandled_input(const Ref<InputEvent> &p_event, bool p_local_coords = false) CONSERVATORY_OVERRIDE;
#endif // DISABLE_DEPRECATED
	CONSERVATORY_VIRTUAL void notify_mouse_entered() CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void notify_mouse_exited() CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_disable_input(bool p_disable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_input_disabled() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_physics_object_picking(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool get_physics_object_picking() CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void set_physics_object_picking_sort(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool get_physics_object_picking_sort() CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void set_physics_object_picking_first_only(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool get_physics_object_picking_first_only() CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL Variant gui_get_drag_data() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL String gui_get_drag_description() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void gui_release_focus() CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Control *gui_get_focus_owner() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Control *gui_get_hovered_control() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_debug_draw(DebugDraw p_debug_draw) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL DebugDraw get_debug_draw() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL int get_render_info(RenderInfoType p_type, RenderInfo p_info) CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_snap_controls_to_pixels(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_snap_controls_to_pixels_enabled() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_snap_2d_transforms_to_pixel(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_snap_2d_transforms_to_pixel_enabled() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_snap_2d_vertices_to_pixel(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_snap_2d_vertices_to_pixel_enabled() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_input_as_handled() CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_input_handled() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_handle_input_locally(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_handling_input_locally() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL bool gui_is_dragging() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool gui_is_drag_successful() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void gui_cancel_drag() CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_sdf_oversize(SDFOversize p_sdf_oversize) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL SDFOversize get_sdf_oversize() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_sdf_scale(SDFScale p_sdf_scale) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL SDFScale get_sdf_scale() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_default_canvas_item_texture_filter(DefaultCanvasItemTextureFilter p_filter) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL DefaultCanvasItemTextureFilter get_default_canvas_item_texture_filter() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_default_canvas_item_texture_repeat(DefaultCanvasItemTextureRepeat p_repeat) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL DefaultCanvasItemTextureRepeat get_default_canvas_item_texture_repeat() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_vrs_mode(VRSMode p_vrs_mode) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL VRSMode get_vrs_mode() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_vrs_update_mode(VRSUpdateMode p_vrs_update_mode) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL VRSUpdateMode get_vrs_update_mode() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_vrs_texture(Ref<Texture2D> p_texture) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Ref<Texture2D> get_vrs_texture() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_embedding_subwindows(bool p_embed) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_embedding_subwindows() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL TypedArray<Window> get_embedded_subwindows() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_canvas_cull_mask(uint32_t p_layers) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL uint32_t get_canvas_cull_mask() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_canvas_cull_mask_bit(uint32_t p_layer, bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool get_canvas_cull_mask_bit(uint32_t p_layer) const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL AudioListener2D *get_audio_listener_2d() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void set_as_audio_listener_2d(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_audio_listener_2d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL Camera2D *get_camera_2d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL AudioListener3D *get_audio_listener_3d() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL void set_as_audio_listener_3d(bool p_enable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_audio_listener_3d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL Camera3D *get_camera_3d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_disable_3d(bool p_disable) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_3d_disabled() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_world_3d(const Ref<World3D> &p_world_3d) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Ref<World3D> get_world_3d() const CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL Ref<World3D> find_world_3d() const CONSERVATORY_OVERRIDE;

	CONSERVATORY_VIRTUAL void set_use_own_world_3d(bool p_use_own_world_3d) CONSERVATORY_OVERRIDE;
	CONSERVATORY_VIRTUAL bool is_using_own_world_3d() const CONSERVATORY_OVERRIDE;

	*/

	_FORCE_INLINE_ static const int64_t static_construct() {
		SimulationDomain *instance = memnew(SimulationDomain);
		return (int64_t)instance;
	}

	SimulationDomain();
	~SimulationDomain();
};

#undef CONSERVATORY_VIRTUAL
#undef CONSERVATORY_OVERRIDE

#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
#endif // SIMULATION_3D_H
