#if REAL_T_IS_DOUBLE
global using real_t = System.Double;
#else
global using real_t = System.Single;
#endif

#if USING_SYSTEM_NUMERICS_VECTORS
#if REAL_T_IS_DOUBLE
#error Cannot use double precision while USING_SYSTEM_NUMERICS_VECTORS is defined.
#endif
global using Vector2 = System.Numerics.Vector2;
global using Vector3 = System.Numerics.Vector3;
global using Vector4 = System.Numerics.Vector4;
global using Quaternion = System.Numerics.Quaternion;
global using Plane = System.Numerics.Plane;
global using Vector2Axis = Godot.Vector2Supplements.Axis;
global using Vector3Axis = Godot.Vector3Supplements.Axis;
global using Vector4Axis = Godot.Vector4Supplements.Axis;
#else
#error The project file was overwritten. Oops.
global using Vector2 = Godot.Vector2;
global using Vector3 = Godot.Vector3;
global using Vector4 = Godot.Vector4;
global using Quaternion = Godot.Quaternion;
global using Plane = Godot.Plane;
global using Vector2Axis = Godot.Vector2.Axis;
global using Vector3Axis = Godot.Vector3.Axis;
global using Vector4Axis = Godot.Vector4.Axis;
#endif
