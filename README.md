# Godot Engine /// The Conservatory Fork

> [!NOTE]  
> This is not the official repository of Godot. Please visit https://godotengine.org or https://github.com/godotengine/godot for the original repo.

This fork of the Godot Engine is based on **Godot 4.5**, and features several changes made specifically for games developed by [Xan's Workshop](https://xansworkshop.com) (in particular, *The Conservatory*).

## Changes

### Build System Changes

* New SCons parameter `allow_break_on_error` and `TC_ALLOW_BREAK_ON_ERROR` macro that is defined when enabled (see API changes)

### API Changes

* New `ConservatoryDebugBridge` type for development builds of the engine.
  * C++ code now contains a breakpoint in all error macros. It is only available if `TC_ALLOW_BREAK_ON_ERROR` is defined (see Build System Changes above).
  * Initializing the debug bridge requires providing a pointer to a static field from C#, this field is a boolean value and determines whether or not the breakpoint should be triggered. It also uses the managed (C#) debugger for `__debugbreak()` which allows it to be used on all platforms (albeit in a very hacky way).
* Non-node-based `RayCast3DDirect` and `ShapeCast3DDirect` types, which allow for raycasting and shapecasting on demand in a manner much faster than the `PhysicsServer3D.intersect_ray` and `PhysicsServer3D.intersect_shape` methods, by avoiding the `Dictionary` return type.
* `SimulationDomain` as a non-rendering alternative to `SubViewport`, for isolated world simulation in both 3D and 2D.
  * "non-rendering" means that it does not need to be told to render to a specific target; it overrides the main viewport when marked as active, and like a camera, only one can be active at a time. All instances still physically simulate regardless of activity. Only rendering is exclusive.
* Support for `#pragma features` and `#pragma exclusive_variants` in the shader language, to allow static variant support.
* Expose `set_include_path` and `get_include_path` in `Shader` (includes C# property `IncludePath`).
  * This property's usage is shared with the engine: It can be used to make `#include` statements work when the shader is created during runtime, without requiring its resource path to be set.
* Assorted improvements to `Engine::get_version_info`
  * Added `version` field, which is a stringified version following GD's display rules i.e. "4.5" instead of "4.5.0".
  * Added `commit` field mimicking the git commit, 6 characters long. It's the same as the `hash` field. Set to `??????` if an error occurs.
  * Added `modules` field, which includes stuff like `mono`, `double`, etc. as a string array, separated out by the `.` character.
* `CharFXTransform` upgrades, including:
  * A reference to the current label being rendered at the time of the effect call (init-only)
  * The font size at the currently rendered glyph (init-only)
  * The unicode codepoint of the currently rendered glyph, which should be significantly easier to use instead of `get_glyph_index`.
    * A C#-only property exists in this type, implementing [`System.Text.Rune`](https://learn.microsoft.com/en-us/dotnet/api/system.text.rune?view=net-9.0)
* `MaterialStorage::global_shader_parameter_get` is no longer an error (The Conservatory uses this to initialize global shader parameters in a manager class).
* `PhysicsServer3D::body_get_shape_disabled` and `PhysicsServer3D::area_get_shape_disabled` API members added.
* `PhysicsServer3D::BODY_PARAM_INVERSE_INERTIA_TENSOR` is a new parameter for `PhysicsServer3D::body_get_param`.
