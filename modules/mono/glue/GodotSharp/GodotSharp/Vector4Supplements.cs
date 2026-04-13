#if USING_SYSTEM_NUMERICS_VECTORS
using System;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics;

namespace Godot
{
    /// <summary>
    /// Added by Xan for Godot: Conservatory Edition
    /// <para/>
    /// Extension members for <see cref="Vector4"/> specifically. This is because the current fork of the engine relies
    /// on the type provided by <c>System.Numerics</c>, replacing the built in Godot type. This class provides extension
    /// methods and members that enable source code to have 1:1 parity with original code.
    /// </summary>
    public static class Vector4Supplements
    {
#pragma warning disable CS1591
        /// <summary>
        /// Represents an axis of a <see cref="Vector4"/>.
        /// </summary>
        public enum Axis { X, Y, Z, W }
#pragma warning restore CS1591

        /// <summary>
        /// Returns a new vector with all components in absolute values (i.e. positive).
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Abs(this Vector4 @this) => Vector4.Abs(@this);

        /// <summary>
        /// Returns a new vector with all components rounded up (towards positive infinity).
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Ceil(this Vector4 @this) => Vector4.Round(@this, MidpointRounding.ToPositiveInfinity);

        /// <summary>
        /// Returns a new vector with all components clamped between the
        /// components of <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Vector4.ClampNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="min">The vector with minimum allowed values.</param>
        /// <param name="max">The vector with maximum allowed values.</param>
        /// <returns>The vector with all components clamped.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Clamp(this Vector4 @this, Vector4 min, Vector4 max) => Vector4.ClampNative(@this, min, max);

        /// <summary>
        /// Returns a new vector with all components clamped between the
        /// <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Vector4.ClampNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="min">The minimum allowed value.</param>
        /// <param name="max">The maximum allowed value.</param>
        /// <returns>The vector with all components clamped.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Clamp(this Vector4 @this, float min, float max) => Vector4.ClampNative(@this, Vector4.Create(min), Vector4.Create(max));

        /// <summary>
        /// Performs a cubic interpolation between vectors <paramref name="pre"/>, this vector,
        /// <paramref name="to"/>, and <paramref name="post"/>, by the given amount <paramref name="weight"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The destination vector.</param>
        /// <param name="pre">A vector before this vector.</param>
        /// <param name="post">A vector after <paramref name="to"/>.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The interpolated vector.</returns>
        public static Vector4 CubicInterpolate(this Vector4 @this, Vector4 to, Vector4 pre, Vector4 post, float weight)
        {
            return 0.5f *
                    ((@this * 2.0f) +
                            (-pre + to) * weight +
                            (2.0f * pre - 5.0f * @this + 4.0f * to - post) * (weight * weight) +
                            (-pre + 3.0f * @this - 3.0f * to + post) * (weight * weight * weight));
        }

        /// <summary>
        /// Performs a cubic interpolation between vectors <paramref name="pre"/>, this vector,
        /// <paramref name="to"/>, and <paramref name="post"/>, by the given amount <paramref name="weight"/>.
        /// It can perform smoother interpolation than <see cref="CubicInterpolate"/>
        /// by the time values.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The destination vector.</param>
        /// <param name="pre">A vector before this vector.</param>
        /// <param name="post">A vector after <paramref name="to"/>.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <param name="toT"></param>
        /// <param name="preT"></param>
        /// <param name="postT"></param>
        /// <returns>The interpolated vector.</returns>
        public static Vector4 CubicInterpolateInTime(this Vector4 @this, Vector4 to, Vector4 pre, Vector4 post, float weight, float toT, float preT, float postT)
        {
            /* Barry-Goldman method */
            float t = toT * weight;//float.Lerp(0.0f, toT, weight);
            Vector4 a1 = Vector4.Lerp(pre, @this, preT == 0 ? 0.0f : (t - preT) / -preT);
            Vector4 a2 = Vector4.Lerp(@this, to, toT == 0 ? 0.5f : t / toT);
            Vector4 a3 = Vector4.Lerp(to, post, postT - toT == 0 ? 1.0f : (t - toT) / (postT - toT));
            Vector4 b1 = Vector4.Lerp(a1, a2, toT - preT == 0 ? 0.0f : (t - preT) / (toT - preT));
            Vector4 b2 = Vector4.Lerp(a2, a3, postT == 0 ? 1.0f : t / postT);
            return Vector4.Lerp(b1, b2, toT == 0 ? 0.5f : t / toT);
        }

        /// <summary>
        /// Returns the normalized vector pointing from this vector to <paramref name="to"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to point towards.</param>
        /// <returns>The direction from this vector to <paramref name="to"/>.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 DirectionTo(this Vector4 @this, Vector4 to) => Vector4.Normalize(to - @this);

        /// <summary>
        /// Returns the squared distance between this vector and <paramref name="to"/>.
        /// This method runs faster than <see cref="DistanceTo"/>. Note that for distance
        /// comparisons, <see cref="ManhattanDistanceTo"/> is the most optimized technique.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The squared distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float DistanceSquaredTo(this Vector4 @this, Vector4 to) => (to - @this).LengthSquared();

        /// <summary>
        /// Returns the distance between this vector and <paramref name="to"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float DistanceTo(this Vector4 @this, Vector4 to) => (to - @this).Length();

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Manhattan distance between this vector and <paramref name="to"/>. Manhattan distance is
        /// also sometimes referred to as "city block distance" in that it measures a grid-based distance, rather
        /// than the direct line distance. This is useful for some forms of pathfinding, and is the most optimal
        /// technique for sorting by distance as this does only addition.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The manhattan distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ManhattanDistanceTo(this Vector4 @this, Vector4 to) => (to - @this).ManhattanLength();

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Chebyshev distance between this vector and <paramref name="to"/>. Chebyshev distance is
        /// almost the same as Manhattan distance, but diagonal grid spaces are considered to be 1 unit away as well.
        /// Think of a Queen on a chessboard moving 1 tile; this "1 tile" is any of the 8 directly around the Queen,
        /// including the diagonals.
        /// </summary>
        /// <remarks>
        /// For a visual example, refer to this image: <see href="https://en.wikipedia.org/wiki/File:Minkowski_distance_examples.svg"/>
        /// </remarks>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The Chebyshev distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ChebyshevDistanceTo(this Vector4 @this, Vector4 to) => (to - @this).ChebyshevLength();

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Manhattan length of this vector. Manhattan length is also sometimes referred to as "city block distance"
        /// in that it measures a grid-based distance without diagonal lines.
        /// This is by far the most optimized technique for finding length. Note that if this is used as a radius in Euler space,
        /// the shape is not hyperspherical, but rather a 24-cell (the 4D version of an octahedron). This is the best method to use
        /// for distance comparison, but note that to be accurate, <em>both distances</em> must be measured using this method.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The Manhattan length of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ManhattanLength(this Vector4 @this) => Vector4.Sum(Vector4.Abs(@this));

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Chebyshev length of this vector. This is similar to Manhattan length, but diagonal grid spaces are considered
        /// to be 1 unit of length as well (think of a Queen on a chessboard moving 1 tile; this "1 tile" is any of the 8 directly around
        /// the Queen, including the diagonals). In 4D, the diagonals on all three axes (i.e. top front right [incomprehensible eldritch word])
        /// are also considered to be of distance 1.
        /// Note that if this is used as a radius in Euclidean space, the shape is not a hypersphere, but rather a tesseract.
        /// To be accurate in comparisons, <em>both distances</em> must be measured using this method.
        /// </summary>
        /// <remarks>
        /// For a visual example, refer to this image: <see href="https://en.wikipedia.org/wiki/File:Minkowski_distance_examples.svg"/>
        /// </remarks>
        /// <param name="this">This vector.</param>
        /// <returns>The Chebyshev length of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ChebyshevLength(this Vector4 @this)
        {
            @this = Vector4.Abs(@this);
            return float.Max(float.Max(@this.X, @this.Y), float.Max(@this.Z, @this.W));
        }

        /// <summary>
        /// Returns the dot product of this vector and <paramref name="with"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The dot product of the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Dot(this Vector4 @this, Vector4 with) => Vector4.Dot(@this, with);

        /// <summary>
        /// Returns a new vector with all components rounded down (towards negative infinity).
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A vector with <see cref="Mathf.Floor(float)"/> called on each component.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Floor(this Vector4 @this) => Vector4.Round(@this, MidpointRounding.ToNegativeInfinity);

        /// <summary>
        /// Returns the inverse of this vector. This is the same as <c>new Vector4(1 / v.X, 1 / v.Y, 1 / v.Z, 1 / v.W)</c>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The inverse of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Inverse(this Vector4 @this) => Vector4.Create(1.0f) / @this;

        /// <summary>
        /// Returns <see langword="true"/> if this vector is finite, meaning all components are neither infinity nor NaN.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>Whether this vector is finite or not.</returns>
        public static bool IsFinite(this Vector4 @this) => Vector128.NoneWhereAllBitsSet(Vector128.IsInfinity(@this.AsVector128()) | Vector128.IsNaN(@this.AsVector128()));

        /// <summary>
        /// Returns <see langword="true"/> if the vector is normalized, and <see langword="false"/> otherwise.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A <see langword="bool"/> indicating whether or not the vector is normalized.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsNormalized(this Vector4 @this) => Mathf.IsOneApprox(@this.LengthSquared());

        /// <summary>
        /// Returns the result of the linear interpolation between
        /// this vector and <paramref name="to"/> by amount <paramref name="weight"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The destination vector for interpolation.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting vector of the interpolation.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Lerp(this Vector4 @this, Vector4 to, float weight) => Vector4.Lerp(@this, to, weight);

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this vector and <paramref name="with"/> using <see cref="Vector4.MaxNative"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The resulting maximum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Max(this Vector4 @this, Vector4 with) => Vector4.MaxNative(@this, with);

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this vector and <paramref name="with"/> using <see cref="Vector4.MaxNative"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting maximum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Max(this Vector4 @this, float with) => Vector4.MaxNative(@this, Vector4.Create(with));

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this vector and <paramref name="with"/> using <see cref="Vector4.MinNative"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The resulting minimum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Min(this Vector4 @this, Vector4 with) => Vector4.MinNative(@this, with);

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this vector and <paramref name="with"/> using <see cref="Vector4.MinNative"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting minimum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Min(this Vector4 @this, float with) => Vector4.MinNative(@this, Vector4.Create(with));

        /// <summary>
        /// Returns the axis of the vector's highest value. See <see cref="Axis"/>.
        /// If all components are equal, this method returns <see cref="Axis.X"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The index of the highest axis.</returns>
        public static Axis MaxAxisIndex(this Vector4 @this)
        {
            int max_index = 0;
            float max_value = @this.X;
            for (int i = 1; i < 4; i++)
            {
                if (@this[i] > max_value)
                {
                    max_index = i;
                    max_value = @this[i];
                }
            }
            return (Axis)max_index;
        }

        /// <summary>
        /// Returns the axis of the vector's lowest value. See <see cref="Axis"/>.
        /// If all components are equal, this method returns <see cref="Axis.W"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The index of the lowest axis.</returns>
        public static Axis MinAxisIndex(this Vector4 @this)
        {
            int min_index = 0;
            float min_value = @this.X;
            for (int i = 1; i < 4; i++)
            {
                if (@this[i] <= min_value)
                {
                    min_index = i;
                    min_value = @this[i];
                }
            }
            return (Axis)min_index;
        }

        /// <summary>
        /// Returns the vector scaled to unit length. Equivalent to <c>v / v.Length()</c>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A normalized version of the vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Normalized(this Vector4 @this)
        {
            if (Mathf.IsZeroApprox(@this.LengthSquared())) return default;
            while (!@this.IsNormalized())
            {
                @this = Vector4.Normalize(@this);
            }
            return @this;
        }

        /// <summary>
        /// Returns a vector composed of the <see cref="Mathf.PosMod(float, float)"/> of this vector's components
        /// and <paramref name="mod"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="mod">A value representing the divisor of the operation.</param>
        /// <returns>
        /// A vector with each component <see cref="Mathf.PosMod(float, float)"/> by <paramref name="mod"/>.
        /// </returns>
        public static Vector4 PosMod(this Vector4 @this, float mod)
        {
            return new Vector4(
                Mathf.PosMod(@this.X, mod),
                Mathf.PosMod(@this.Y, mod),
                Mathf.PosMod(@this.Z, mod),
                Mathf.PosMod(@this.W, mod)
            );
        }

        /// <summary>
        /// Returns a vector composed of the <see cref="Mathf.PosMod(float, float)"/> of this vector's components
        /// and <paramref name="modv"/>'s components.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="modv">A vector representing the divisors of the operation.</param>
        /// <returns>
        /// A vector with each component <see cref="Mathf.PosMod(float, float)"/> by <paramref name="modv"/>'s components.
        /// </returns>
        public static Vector4 PosMod(this Vector4 @this, Vector4 modv)
        {
            return new Vector4(
                Mathf.PosMod(@this.X, modv.X),
                Mathf.PosMod(@this.Y, modv.Y),
                Mathf.PosMod(@this.Z, modv.Z),
                Mathf.PosMod(@this.W, modv.W)
            );
        }

        /// <summary>
        /// Returns this vector with all components rounded to the nearest integer,
        /// with halfway cases rounded towards the nearest multiple of two.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The rounded vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Round(this Vector4 @this) => Vector4.Round(@this);

        /// <summary>
        /// Returns a vector with each component set to one or negative one, depending
        /// on the signs of this vector's components, or zero if the component is zero,
        /// by calling <see cref="float.Sign(float)"/> on each component.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A vector with all components as either <c>1</c>, <c>-1</c>, or <c>0</c>.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 Sign(this Vector4 @this)
        {
            return new Vector4(
                float.Sign(@this.X),
                float.Sign(@this.Y),
                float.Sign(@this.Z),
                float.Sign(@this.W)
            );
        }

        /// <summary>
        /// Returns a new vector with each component snapped to the nearest multiple of the corresponding component in <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="step">A vector value representing the step size to snap to.</param>
        /// <returns>The snapped vector.</returns>
        public static Vector4 Snapped(this Vector4 @this, Vector4 step)
        {
            return new Vector4(
                Mathf.Snapped(@this.X, step.X),
                Mathf.Snapped(@this.Y, step.Y),
                Mathf.Snapped(@this.Z, step.Z),
                Mathf.Snapped(@this.W, step.W)
            );
        }

        /// <summary>
        /// Returns a new vector with each component snapped to the nearest multiple of <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="step">The step size to snap to.</param>
        /// <returns>The snapped vector.</returns>
        public static Vector4 Snapped(this Vector4 @this, float step)
        {
            return new Vector4(
                Mathf.Snapped(@this.X, step),
                Mathf.Snapped(@this.Y, step),
                Mathf.Snapped(@this.Z, step),
                Mathf.Snapped(@this.W, step)
            );
        }


        /// <summary>
        /// Returns <see langword="true"/> if this vector and <paramref name="other"/> are approximately equal,
        /// by running <see cref="Mathf.IsEqualApprox(real_t, real_t)"/> on each component.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="other">The other vector to compare.</param>
        /// <returns>Whether or not the vectors are approximately equal.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsEqualApprox(this Vector4 @this, Vector4 other) => Vector4.LessThanAll(Vector4.Abs(@this - other), Vector4.Create(Mathf.Epsilon));

        /// <summary>
        /// Returns <see langword="true"/> if this vector's values are approximately zero.
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsZeroApprox(this Vector4 @this) => Vector4.Abs(@this) < Vector4.Create(Mathf.Epsilon);

        /// <summary>
        /// Returns <see langword="true"/> if the vector is exactly equal to zero.
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsExactlyZero(this Vector4 @this) => Vector4.AllWhereAllBitsSet(Vector4.IsZero(@this));


        extension(Vector4 @this)
        {
            /// <inheritdoc cref="Vector4.PositiveInfinity"/>
            public static Vector4 Inf => Vector4.PositiveInfinity;

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are greater than those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator >(Vector4 left, Vector4 right) => Vector4.GreaterThanAll(left, right);

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are less than those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator <(Vector4 left, Vector4 right) => Vector4.LessThanAll(left, right);

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are greater than or equal to those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator >=(Vector4 left, Vector4 right) => Vector4.GreaterThanOrEqualAll(left, right);

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are less than or equal to those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator <=(Vector4 left, Vector4 right) => Vector4.LessThanOrEqualAll(left, right);

            /// <summary>
            /// Performs the modulus of <paramref name="left"/> and <paramref name="right"/>, which returns the remainder of the division operation <c><paramref name="left"/> / <paramref name="right"/></c>
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static Vector4 operator %(Vector4 left, float right) => new Vector4(left.X % right, left.Y % right, left.Z % right, left.W % right);

            /// <summary>
            /// Performs the modulus of <paramref name="left"/> and <paramref name="right"/>, which returns the remainder of the division operation <c><paramref name="left"/> / <paramref name="right"/></c>
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static Vector4 operator %(Vector4 left, Vector4 right) => new Vector4(left.X % right.X, left.Y % right.Y, left.Z % right.Z, left.W % right.W);
        }
    }
}
#endif
