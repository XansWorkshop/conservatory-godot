# Godot Engine /// The Conservatory Fork

> [!NOTE]  
> This is not the official repository of Godot. Please visit https://godotengine.org or https://github.com/godotengine/godot for the original repo.

This fork of the Godot Engine is based on **Godot 4.5**, and features several changes made specifically for games developed by [Xan's Workshop](https://xansworkshop.com) (in particular, *The Conservatory*).

## Changes

* `RayCast3DDirect` (a non-`Node`-based alternative to `RayCast3D`. **Supersedes** `DirectPhysicsSpaceState3D.intersect_ray()`!)
  * `RayCastResult` (a non-`Dictionary`-based result used in `RayCast3DDirect`)
* `SimulationDomain` as a non-rendering alternative to `SubViewport`, for isolated world simulation in both 3D and 2D.
  * "non-rendering" means that it does not need to be told to render to a specific target; it overrides the main viewport when marked as active, and like a camera, only one can be active at a time. All instances still physically simulate regardless of activity. Only rendering is exclusive.
* ~~Implements [apples](https://github.com/apples)'s Stencil Buffer support. ([Fork](https://github.com/apples/godot/tree/7174-apples-stencil) | [Pull Request](https://github.com/godotengine/godot/pull/80710))~~ **This PR has been merged and is now official.**
* Support for `#pragma features` and `#pragma exclusive_variants` in the shader language, to allow static variant support.
* Expose `set_include_path` and `get_include_path` in `Shader` (includes C# property `IncludePath`).
  * This property's uage is shared with the engine: It can be used to make `#include` statements work when the shader is created during runtime without a resource path.
* Assorted improvements to `Engine::get_version_info`
  * Added `version` field, which is a stringified version following GD's display rules i.e. "4.5" instead of "4.5.0".
  * Added `commit` field mimicking the git commit, 6 characters long. It's the same as the `hash` field. Set to `??????` if an error occurs.
  * Added `modules` field, which includes stuff like `mono`, `double`, etc. as a string array, separated out by the `.` character.
* `CanvasItem::DrawEarly` event and virtual method, which invokes **before** `NOTIFICATION_DRAW`.
  * Also comes with `NOTIFICATION_DRAW_EARLY`.
* `CharFXTransform` upgrades, including:
  * A reference to the current label being rendered at the time of the effect call (init-only)
  * The font size at the currently rendered glyph (init-only)
  * The unicode codepoint of the currently rendered glyph, which should be significantly easier to use instead of `get_glyph_index`.
    * A C#-only property exists in this type, implementing [`System.Text.Rune`](https://learn.microsoft.com/en-us/dotnet/api/system.text.rune?view=net-9.0)
