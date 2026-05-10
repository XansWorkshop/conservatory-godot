using System;
using System.Diagnostics;

namespace Godot {

    partial struct Variant {

        const string VARIANT_CAST_MESSAGE = "Variant type is not an exact match.";

        [StackTraceHidden]
        private static InvalidCastException ThrowInvalidCast() => throw new InvalidCastException(VARIANT_CAST_MESSAGE);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked bool(Variant from) => from.Type == Variant.Type.Bool ? from.AsBool() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked char(Variant from) => from.Type == Variant.Type.Int ? from.AsChar() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked sbyte(Variant from) => from.Type == Variant.Type.Int ? from.AsSByte() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked short(Variant from) => from.Type == Variant.Type.Int ? from.AsInt16() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked int(Variant from) => from.Type == Variant.Type.Int ? from.AsInt32() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked long(Variant from) => from.Type == Variant.Type.Int ? from.AsInt64() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked byte(Variant from) => from.Type == Variant.Type.Int ? from.AsByte() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked ushort(Variant from) => from.Type == Variant.Type.Int ? from.AsUInt16() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked uint(Variant from) => from.Type == Variant.Type.Int ? from.AsUInt32() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked ulong(Variant from) => from.Type == Variant.Type.Int ? from.AsUInt64() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked float(Variant from) => from.Type == Variant.Type.Float ? from.AsSingle() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked double(Variant from) => from.Type == Variant.Type.Float ? from.AsDouble() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked string(Variant from) => (from.Type == Variant.Type.String || from.Type == Variant.Type.StringName) ? from.AsString() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector2(Variant from) => from.Type == Variant.Type.Vector2 ? from.AsVector2() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector2I(Variant from) => from.Type == Variant.Type.Vector2I ? from.AsVector2I() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Rect2(Variant from) => from.Type == Variant.Type.Rect2 ? from.Rect2() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Rect2I(Variant from) => from.Type == Variant.Type.Rect2I ? from.Rect2I() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Transform2D(Variant from) => from.Type == Variant.Type.Transform2D ? from.Transform2D() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector3(Variant from) => from.Type == Variant.Type.Vector3 ? from.AsVector3() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector3I(Variant from) => from.Type == Variant.Type.Vector3I ? from.AsVector3I() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Basis(Variant from) => from.Type == Variant.Type.Basis ? from.AsBasis() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Quaternion(Variant from) => from.Type == Variant.Type.Quaternion ? from.AsQuaternion() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Transform3D(Variant from) => from.Type == Variant.Type.Transform3D ? from.AsTransform3D() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector4(Variant from) => from.Type == Variant.Type.Vector4 ? from.AsVector4() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector4I(Variant from) => from.Type == Variant.Type.Vector4I ? from.AsVector4I() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Projection(Variant from) => from.Type == Variant.Type.Projection ? from.Projection() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Aabb(Variant from) => from.Type == Variant.Type.Aabb ? from.Aabb() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Color(Variant from) => from.Type == Variant.Type.Color ? from.AsColor() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Plane(Variant from) => from.Type == Variant.Type.Plane ? from.AsPlane() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Callable(Variant from) => from.Type == Variant.Type.Callable ? from.AsCallable() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Signal(Variant from) => from.Type == Variant.Type.Signal ? from.AsSignal() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked byte[](Variant from) => from.Type == Variant.Type.PackedByteArray ? from.AsByteArray() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked int[](Variant from) => from.Type == Variant.Type.PackedInt32Array ? from.AsInt32Array() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked long[](Variant from) => from.Type == Variant.Type.PackedInt64Array ? from.AsInt64Array() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked float[](Variant from) => from.Type == Variant.Type.PackedFloat32Array ? from.AsFloat32Array() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked double[](Variant from) => from.Type == Variant.Type.PackedFloat64Array ? from.AsFloat64Array() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked string[](Variant from) => from.Type == Variant.Type.PackedStringArray ? from.AsStringArray() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector2[](Variant from) => from.Type == Variant.Type.PackedVector2Array ? from.AsVector2Array() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector3[](Variant from) => from.Type == Variant.Type.PackedVector3Array ? from.AsVector3Array() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Vector4[](Variant from) => from.Type == Variant.Type.PackedVector4Array ? from.AsVector4Array() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Color[](Variant from) => from.Type == Variant.Type.PackedColorArray ? from.AsColorArray() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked StringName[](Variant from)
        {
            Array array = checked((Array)from);
            StringName[] result = new StringName[array.Count];
            for (int i = 0; i < result.Length; i++) {
                result[i] = checked((StringName)array[i]);
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked NodePath[](Variant from)
        {
            Array array = checked((Array)from);
            NodePath[] result = new NodePath[array.Count];
            for (int i = 0; i < result.Length; i++)
            {
                result[i] = checked((NodePath)array[i]);
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Rid[](Variant from)
        {
            Array array = checked((Array)from);
            Rid[] result = new Rid[array.Count];
            for (int i = 0; i < result.Length; i++)
            {
                result[i] = checked((Rid)array[i]);
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked GodotObject(Variant from) => (from.Type == Variant.Type.Object || from.Type == Variant.Type.Nil) ? from.AsGodotObject() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked StringName(Variant from) => (from.Type == Variant.Type.String || from.Type == Variant.Type.StringName) ? from.AsStringName() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked NodePath(Variant from) => from.Type == Variant.Type.NodePath ? from.AsNodePath() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Rid(Variant from) => from.Type == Variant.Type.Rid ? from.AsRid() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Collections.Dictionary(Variant from) => from.Type == Variant.Type.Dictionary ? from.AsGodotDictionary() : throw ThrowInvalidCast();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static explicit operator checked Collections.Array(Variant from) => from.Type == Variant.Type.Array ? from.AsGodotArray() : throw ThrowInvalidCast();

    }

}
