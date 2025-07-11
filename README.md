# Godot Engine /// The Conservatory Fork

> [!NOTE]  
> This is not the official repository of Godot. Please visit https://godotengine.org or https://github.com/godotengine/godot for the original repo.

This fork of the Godot Engine is based on **Godot 4.5**, and features several changes made specifically for games developed by [Xan's Workshop](https://xansworkshop.com) (in particular, *The Conservatory*).

## Changes

### Build System Changes

* New SCons parameter `allow_break_on_error` and a corresponding `TC_ALLOW_BREAK_ON_ERROR` macro that is defined when enabled (see API changes)

### Public API Changes

> [!NOTE]
> Some changes are C#-only, and have no analogue nor enforcement in GDScript, making them impossible to use in GDScript or unfeasible. This is because this fork of the engine is specifically made for The Conservatory. While some features could likely be merged in with the main engine, not all of them can be (nor should they be).

* New `ConservatoryDebugBridge` type for development builds of the engine.
  * C++ code now contains a breakpoint in all error macros. It is only available if `TC_ALLOW_BREAK_ON_ERROR` is defined (see Build System Changes above).
  * Initializing the debug bridge requires providing a pointer to a static field from C#, this field is a boolean value and determines whether or not the breakpoint should be triggered. It also wants a pointer to the getter for `Debugger.IsAttached`.
  * This system ensures `__debugbreak()` only gets fired when allowed to by the engine (for code cleanliness in release builds) and when a managed debugger is attached.
* Non-node-based `RayCast3DDirect` and `ShapeCast3DDirect` types, which allow for fast, compartmentalized raycasting and shapecasting on demand.
  * This operates much faster than the `PhysicsServer3D.intersect_ray` and `PhysicsServer3D.intersect_shape` methods, by avoiding the `Dictionary` return type.
  * This is "compartmentalized" in that these types still have the properties needed to store both parameters for the cast, and the result of said cast, making them isolated objects that can be dispatched to represent a single operation.
  * If desired, static methods to cast also exist that accept parameters and return a result.
* `SimulationDomain` is a class extending `Viewport` designed for the sole purpose of isolated simulation of both a 3D and 2D world.
  * Unlike `SubViewport`, this type *can not* render to a target. Instead, on the game client, it must be made active. This will cause it to override the rendering of the main window by setting the worlds (both 3D and 2D) of the game's root viewport to that of the `SimulationDomain`.
  * While `SimulationDomain` does inherit all properties and methods of `Viewport`, using these methods will always redirect the call to the active viewport. This ensures that the rather hacky nature of its existence remains compatible.
* Support for `#pragma features` and `#pragma exclusive_variants` in the shader language, to allow static variant support.
* Expose `set_include_path` and `get_include_path` in `Shader` (includes C# property `IncludePath`).
  * This property's usage is shared with the engine: It can be used to make `#include` statements work when the shader is created during runtime, without requiring its resource path to be set.
* Assorted improvements to `Engine::get_version_info`
  * Added `version` field, which is a stringified version following GD's display rules i.e. "4.5" instead of "4.5.0".
  * Added `commit` field mimicking the git commit, 6 characters long. It's the same as the `hash` field. Set to `000000` if an error occurs.
  * Added `modules` field, which includes stuff like `mono`, `double`, etc. as a string array, separated out by the `.` character.
* `CharFXTransform` upgrades, including:
  * A reference to the current label being rendered at the time of the effect call (init-only)
  * The font size at the currently rendered glyph (init-only)
  * The unicode codepoint of the currently rendered glyph, which should be significantly easier to use instead of `get_glyph_index`.
    * A C#-only property exists in this type, implementing [`System.Text.Rune`](https://learn.microsoft.com/en-us/dotnet/api/system.text.rune?view=net-9.0)
* `MaterialStorage::global_shader_parameter_get` is no longer an error case. The Conservatory uses this to initialize global shader parameters in a manager class.
* Added `PhysicsServer3D::body_get_shape_disabled` and `PhysicsServer3D::area_get_shape_disabled` methods.
* Added `PhysicsServer3D::BODY_PARAM_INVERSE_INERTIA_TENSOR` as a new parameter for `PhysicsServer3D::body_get_param`.
* Added `Control.pivot_is_relative` which allows the `pivot_offset` property of a `Control` to be declared as a percentage (a range from 0.0f to 1.0f) of the size, rather than in pixels, for dynamically resized nodes.

### Internal API Changes

* New `ADD_READONLY_PROPERTY` macro and `ADD_INITONLY_PROPERTY` macro.
  * Read-only properties apply to C# and GDScript by simply not having a setter.
  * Init-only properties **Are C# Only** as they make use of the `init` keyword, which does not exist in GDScript. In GDScript they appear as normal properties.
