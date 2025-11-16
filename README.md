# Godot Engine /// The Conservatory Fork

> [!NOTE]  
> This is not the official repository of Godot. Please visit https://godotengine.org or https://github.com/godotengine/godot for the original repo.

This fork of the Godot Engine is based on **Godot 4.5**, and features several changes made specifically for games developed by [Xan's Workshop](https://xansworkshop.com) (in particular, *The Conservatory*).

## Changes

### Build System Changes

* New SCons parameter `allow_break_on_error` and a corresponding `TC_ALLOW_BREAK_ON_ERROR` macro that is defined when enabled (see API changes)
* New SCons parameter `allow_release_memory_tracking` and a corresponding `TC_ALLOW_RELEASE_MEMORY_TRACKING` macro that is defined when enabled (see API changes)

### Public API Changes

> [!NOTE]
> Some changes are C#-only, and have no analogue nor enforcement in GDScript, making them impossible to use in GDScript or unfeasible. This is because this fork of the engine is specifically made for The Conservatory. While some features could likely be merged in with the main engine, not all of them can be (nor should they be).

The documentation for all modifications to the public API [can be found on the blog](https://xansworkshop.com/conservatory/blog/site/engine/).

### Internal API Changes

* New `ADD_READONLY_PROPERTY` macro and `ADD_INITONLY_PROPERTY` macro.
  * Read-only properties apply to C# and GDScript by simply not having a setter.
  * Init-only properties **Are C# Only** as they make use of the `init` keyword, which does not exist in GDScript. In GDScript they appear as normal properties.
