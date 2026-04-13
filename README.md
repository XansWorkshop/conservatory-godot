# Godot Engine /// The Conservatory Fork

> [!NOTE]  
> This is not the official repository of Godot. Please visit https://godotengine.org or https://github.com/godotengine/godot for the original repo.

This fork of the Godot Engine, **spanning versions 4.4 to 4.7**. It features several changes made specifically for games developed by [Xan's Workshop](https://xansworkshop.com) (in particular, *The Conservatory*).

**This repository is not intended for use by the general public for game development.** You are encouraged to use official Godot builds. The features present in this fork of the engine are volatile by nature and no consideration is made for issues that may be experienced by third parties when updates are released.

## Changes

### Build System Changes

* New SCons parameter `allow_break_on_error` and a corresponding `TC_ALLOW_BREAK_ON_ERROR` macro that is defined when enabled (see API changes)
* New SCons parameter `allow_release_memory_tracking` and a corresponding `TC_ALLOW_RELEASE_MEMORY_TRACKING` macro that is defined when enabled (see API changes)

### API Changes

> [!NOTE]
> Some changes are C#-only, and have no analogue nor enforcement in GDScript, making them impossible to use in GDScript or unfeasible. This is because this fork of the engine is specifically made for The Conservatory. While some features could likely be merged in with the main engine, not all of them can be (nor should they be).

The documentation for all modifications to the public API [can be found on the blog](https://xansworkshop.com/conservatory/blog/site/engine/).
