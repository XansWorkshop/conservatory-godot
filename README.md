# Godot Engine /// The Conservatory Fork

> [!NOTE]  
> This is not the official repository of Godot. Please visit https://godotengine.org or https://github.com/godotengine/godot for the original repo.

This fork of the Godot Engine is based on **Godot 4.5**, and features several changes made specifically for games developed by [Xan's Workshop](https://xansworkshop.com) (in particular, *The Conservatory*).

## Changes

* `RayCast3DDirect` (a non-`Node`-based alternative to `RayCast3D`. **Supersedes** `DirectPhysicsSpaceState3D.intersect_ray()`!)
* `RayCastResult` (a non-`Dictionary`-based result used in `RayCast3DDirect`)
* `SimulationDomain` as a non-rendering alternative to `SubViewport`, for isolated world simulation in both 3D and 2D.
* Implements [apples](https://github.com/apples)'s Stencil Buffer support. ([Fork](https://github.com/apples/godot/tree/7174-apples-stencil) | [Pull Request](https://github.com/godotengine/godot/pull/80710))
* Support for `#pragma features` and `#pragma exclusive_variants` in the shader language. These don't do anything on the engine level. Game code uses it.
* Assorted improvements to `Engine::get_version_info`
  * Added `version` field, which is a stringified version following GD's display rules i.e. "4.5" instead of "4.5.0".
  * Added `commit` field mimicking the git commit, 6 characters long. It's the same as the `hash` field. Set to `??????` if an error occurs.
  * Added `modules` field, which includes stuff like `mono` and etc as a string array.
