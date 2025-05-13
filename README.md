# Godot Engine /// Xan's Workshop Fork

> [!NOTE]  
> This is not the official repository of Godot. Please visit https://godotengine.org or https://github.com/godotengine/godot for the original repo.

This fork of the Godot Engine is based on **Godot 4.5**, and features several changes made specifically for games developed by [Xan's Workshop](https://xansworkshop.com).

## What's new?
* The `RayCast3DDirect` class. Unlike its sibling, `RayCast3D`, this instance allows providing the `Rid` of the space directly, and it is not a `Node`. This means it can be used on the fly without scene tree access.
  * This provides both a static and an instance implementation.
  * This is an alternative to calling `IntersectRay` on the space state, which allocates and returns a `Dictionary` to store its results. This fact is what makes the built in method a poor candidate for high performance applications.
* Implementation of `RayCastResult` which works in tandem with `RayCast3DDirect`.
  * This technique provides context as to what *kind* of object got hit. In particular, this is important for *The Conservatory* as it has many physics objects without nodes. Being able to know if it's an area or a body is vital.
* Support for the stencil buffer from [apples](https://github.com/apples)'s fork of the engine ([Fork](https://github.com/apples/godot/tree/7174-apples-stencil) | [Pull Request](https://github.com/godotengine/godot/pull/80710))

## What's possibly planned?
* Nothing right now