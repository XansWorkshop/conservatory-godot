#if USING_SYSTEM_NUMERICS_VECTORS
using System;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics;

namespace Godot
{
    /// <summary>
    /// Added by Xan for Godot: Conservatory Edition
    /// <para/>
    /// Extension members for <see cref="Vector3"/> specifically. This is because the current fork of the engine relies
    /// on the type provided by <c>System.Numerics</c>, replacing the built in Godot type. This class provides extension
    /// methods and members that enable source code to have 1:1 parity with original code.
    /// </summary>
    public static class Vector3Supplements
    {
#pragma warning disable CS1591
        /// <summary>
        /// Represents an axis of a <see cref="Vector3"/>.
        /// </summary>
        public enum Axis { X, Y, Z }
#pragma warning restore CS1591

        /// <summary>
        /// Returns a new vector with all components in absolute values (i.e. positive).
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A vector with <see cref="Mathf.Abs(float)"/> called on each component.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Abs(this Vector3 @this) => Vector3.Abs(@this);

        /// <summary>
        /// Returns the unsigned minimum angle to the given vector, in radians.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to compare this vector to.</param>
        /// <returns>The unsigned angle between the two vectors, in radians.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float AngleTo(this Vector3 @this, Vector3 to)
        {
            return float.Atan2(@this.Cross(to).Length(), @this.Dot(to));
        }

        /// <summary>
        /// Returns this vector "bounced off" from a plane defined by the given normal.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="normal">The normal vector defining the plane to bounce off. Must be normalized.</param>
        /// <returns>The bounced vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Bounce(this Vector3 @this, Vector3 normal) => -Vector3.Reflect(@this, normal);

        /// <summary>
        /// Returns a new vector with all components rounded up (towards positive infinity).
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Ceil(this Vector3 @this) => Vector3.Round(@this, MidpointRounding.ToPositiveInfinity);

        /// <summary>
        /// Returns a new vector with all components clamped between the
        /// components of <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Vector3.ClampNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="min">The vector with minimum allowed values.</param>
        /// <param name="max">The vector with maximum allowed values.</param>
        /// <returns>The vector with all components clamped.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Clamp(this Vector3 @this, Vector3 min, Vector3 max) => Vector3.ClampNative(@this, min, max);

        /// <summary>
        /// Returns a new vector with all components clamped between the
        /// <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Vector3.ClampNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="min">The minimum allowed value.</param>
        /// <param name="max">The maximum allowed value.</param>
        /// <returns>The vector with all components clamped.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Clamp(this Vector3 @this, float min, float max) => Vector3.ClampNative(@this, Vector3.Create(min), Vector3.Create(max));

        /// <summary>
        /// Returns the cross product of this vector and <paramref name="with"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector.</param>
        /// <returns>The cross product vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Cross(this Vector3 @this, Vector3 with) => Vector3.Cross(@this, with);

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
        public static Vector3 CubicInterpolate(this Vector3 @this, Vector3 to, Vector3 pre, Vector3 post, float weight)
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
        public static Vector3 CubicInterpolateInTime(this Vector3 @this, Vector3 to, Vector3 pre, Vector3 post, float weight, float toT, float preT, float postT)
        {
            /* Barry-Goldman method */
            float t = toT * weight;//float.Lerp(0.0f, toT, weight);
            Vector3 a1 = Vector3.Lerp(pre, @this, preT == 0 ? 0.0f : (t - preT) / -preT);
            Vector3 a2 = Vector3.Lerp(@this, to, toT == 0 ? 0.5f : t / toT);
            Vector3 a3 = Vector3.Lerp(to, post, postT - toT == 0 ? 1.0f : (t - toT) / (postT - toT));
            Vector3 b1 = Vector3.Lerp(a1, a2, toT - preT == 0 ? 0.0f : (t - preT) / (toT - preT));
            Vector3 b2 = Vector3.Lerp(a2, a3, postT == 0 ? 1.0f : t / postT);
            return Vector3.Lerp(b1, b2, toT == 0 ? 0.5f : t / toT);
        }

        /// <summary>
        /// Returns the point at the given <paramref name="t"/> on a one-dimensional Bezier curve defined by this vector
        /// and the given <paramref name="control1"/>, <paramref name="control2"/>, and <paramref name="end"/> points.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="control1">Control point that defines the bezier curve.</param>
        /// <param name="control2">Control point that defines the bezier curve.</param>
        /// <param name="end">The destination vector.</param>
        /// <param name="t">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The interpolated vector.</returns>
        public static Vector3 BezierInterpolate(this Vector3 @this, Vector3 control1, Vector3 control2, Vector3 end, float t)
        {
            // Formula from Wikipedia article on Bezier curves
            float omt = 1.0f - t;
            float omt2 = omt * omt;
            float omt3 = omt2 * omt;
            float t2 = t * t;
            float t3 = t2 * t;
            return @this * omt3 + control1 * omt2 * t * 3.0f + control2 * omt * t2 * 3.0f + end * t3;
        }

        /// <summary>
        /// Returns the derivative at the given <paramref name="t"/> on the Bezier curve defined by this vector
        /// and the given <paramref name="control1"/>, <paramref name="control2"/>, and <paramref name="end"/> points.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="control1">Control point that defines the bezier curve.</param>
        /// <param name="control2">Control point that defines the bezier curve.</param>
        /// <param name="end">The destination value for the interpolation.</param>
        /// <param name="t">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting value of the interpolation.</returns>
        public static Vector3 BezierDerivative(this Vector3 @this, Vector3 control1, Vector3 control2, Vector3 end, float t)
        {
            // Formula from Wikipedia article on Bezier curves
            float omt = 1.0f - t;
            float omt2 = omt * omt;
            float t2 = t * t;

            return (control1 - @this) * 3.0f * omt2 + (control2 - control1) * 6.0f * omt * t + (end - control2) * 3.0f * t2;
        }

        /// <summary>
        /// Returns the normalized vector pointing from this vector to <paramref name="to"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to point towards.</param>
        /// <returns>The direction from this vector to <paramref name="to"/>.</returns>
        public static Vector3 DirectionTo(this Vector3 @this, Vector3 to) => Vector3.Normalize(to - @this);

        /// <summary>
        /// Returns the squared distance between this vector and <paramref name="to"/>.
        /// This method runs faster than <see cref="DistanceTo"/>. Note that for distance
        /// comparisons, <see cref="ManhattanDistanceTo"/> is the most optimized technique.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The squared distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float DistanceSquaredTo(this Vector3 @this, Vector3 to) => (to - @this).LengthSquared();

        /// <summary>
        /// Returns the distance between this vector and <paramref name="to"/>.
        /// </summary>
        /// <seealso cref="DistanceSquaredTo"/>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float DistanceTo(this Vector3 @this, Vector3 to) => (to - @this).Length();

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
        public static float ManhattanDistanceTo(this Vector3 @this, Vector3 to) => (to - @this).ManhattanLength();

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
        public static float ChebyshevDistanceTo(this Vector3 @this, Vector3 to) => (to - @this).ChebyshevLength();

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Manhattan length of this vector. Manhattan length is also sometimes referred to as "city block distance"
        /// in that it measures a grid-based distance without diagonal lines.
        /// This is by far the most optimized technique for finding length. Note that if this is used as a radius in Euler space,
        /// the shape is not spherical, but rather an octahedron. This is the best method to use for distance comparison, but note
        /// that to be accurate, <em>both distances</em> must be measured using this method.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The Manhattan length of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ManhattanLength(this Vector3 @this) => Vector3.Sum(Vector3.Abs(@this));

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Chebyshev length of this vector. This is similar to Manhattan length, but diagonal grid spaces are considered
        /// to be 1 unit of length as well (think of a Queen on a chessboard moving 1 tile; this "1 tile" is any of the 8 directly around
        /// the Queen, including the diagonals). In 3D, the diagonals on all three axes (i.e. top front right) are also considered to be
        /// of distance 1.
        /// Note that if this is used as a radius in Euclidean space, the shape is not spherical, but rather a cube.
        /// To be accurate in comparisons, <em>both distances</em> must be measured using this method.
        /// </summary>
        /// <remarks>
        /// For a visual example, refer to this image: <see href="https://en.wikipedia.org/wiki/File:Minkowski_distance_examples.svg"/>
        /// </remarks>
        /// <param name="this">This vector.</param>
        /// <returns>The Chebyshev length of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ChebyshevLength(this Vector3 @this)
        {
            @this = Vector3.Abs(@this);
            return float.Max(@this.X, float.Max(@this.Y, @this.Z));
        }

        /// <summary>
        /// Returns the dot product of this vector and <paramref name="with"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The dot product of the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Dot(this Vector3 @this, Vector3 with) => Vector3.Dot(@this, with);

        /// <summary>
        /// Returns a new vector with all components rounded down (towards negative infinity).
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Floor(this Vector3 @this) => Vector3.Round(@this, MidpointRounding.ToNegativeInfinity);

        /// <summary>
        /// Returns the inverse of this vector. This is the same as <c>new Vector3(1 / v.X, 1 / v.Y, 1 / v.Z)</c>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The inverse of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Inverse(this Vector3 @this) => Vector3.Create(1) / @this;

        /// <summary>
        /// Returns <see langword="true"/> if this vector is finite, meaning all components are neither infinity nor NaN.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>Whether this vector is finite or not.</returns>
        public static bool IsFinite(this Vector3 @this) => Vector128.NoneWhereAllBitsSet((Vector128.IsInfinity(@this.AsVector128()) | Vector128.IsNaN(@this.AsVector128())).AsUInt32() & Vector128.Create(uint.MaxValue, uint.MaxValue, uint.MaxValue, 0));

        /// <summary>
        /// Returns <see langword="true"/> if the vector is normalized, and <see langword="false"/> otherwise.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A <see langword="bool"/> indicating whether or not the vector is normalized.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsNormalized(this Vector3 @this) => Mathf.IsOneApprox(@this.LengthSquared());

        /// <summary>
        /// Returns the result of the linear interpolation between
        /// this vector and <paramref name="to"/> by amount <paramref name="weight"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The destination vector for interpolation.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting vector of the interpolation.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Lerp(this Vector3 @this, Vector3 to, float weight) => Vector3.Lerp(@this, to, weight);

        /// <summary>
        /// Returns the vector with a maximum length by limiting its length to <paramref name="length"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="length">The length to limit to.</param>
        /// <returns>The vector with its length limited.</returns>
        public static Vector3 LimitLength(this Vector3 @this, float length = 1.0f)
        {
            float l = @this.Length();

            if (l > 0 && length < l)
            {
                @this /= l;
                @this *= length;
            }

            return @this;
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this vector and <paramref name="with"/> with <see cref="Vector3.MaxNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The resulting maximum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Max(this Vector3 @this, Vector3 with) => Vector3.MaxNative(@this, with);

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this vector and <paramref name="with"/> with <see cref="Vector3.MaxNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting maximum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Max(this Vector3 @this, float with) => Vector3.MaxNative(@this, Vector3.Create(with));

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this vector and <paramref name="with"/> with <see cref="Vector3.MinNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The resulting minimum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Min(this Vector3 @this, Vector3 with) => Vector3.MinNative(@this, with);

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this vector and <paramref name="with"/> with <see cref="Vector3.MinNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting minimum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Min(this Vector3 @this, float with) => Vector3.MinNative(@this, Vector3.Create(with));

        /// <summary>
        /// Returns the axis of the vector's highest value. See <see cref="Axis"/>.
        /// If all components are equal, this method returns <see cref="Axis.X"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The index of the highest axis.</returns>
        public static Axis MaxAxisIndex(this Vector3 @this)
        {
            return @this.X < @this.Y ? (@this.Y < @this.Z ? Axis.Z : Axis.Y) : (@this.X < @this.Z ? Axis.Z : Axis.X);
        }

        /// <summary>
        /// Returns the axis of the vector's lowest value. See <see cref="Axis"/>.
        /// If all components are equal, this method returns <see cref="Axis.Z"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The index of the lowest axis.</returns>
        public static Axis MinAxisIndex(this Vector3 @this)
        {
            return @this.X < @this.Y ? (@this.X < @this.Z ? Axis.X : Axis.Z) : (@this.Y < @this.Z ? Axis.Y : Axis.Z);
        }

        /// <summary>
        /// Moves this vector toward <paramref name="to"/> by the fixed <paramref name="delta"/> amount.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The vector to move towards.</param>
        /// <param name="delta">The amount to move towards by.</param>
        /// <returns>The resulting vector.</returns>
        public static Vector3 MoveToward(this Vector3 @this, Vector3 to, float delta)
        {
            Vector3 vd = to - @this;
            float len = vd.Length();
            if (len <= delta || len < Mathf.Epsilon)
                return to;

            return @this + (vd / len * delta);
        }

        /// <summary>
        /// Returns the vector scaled to unit length. Equivalent to <c>v / v.Length()</c>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A normalized version of the vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Normalized(this Vector3 @this) {
            if (Mathf.IsZeroApprox(@this.LengthSquared())) return default;
            while (!@this.IsNormalized())
            {
                @this = Vector3.Normalize(@this);
            }
            return @this;
        }

        /// <summary>
        /// Returns the outer product with <paramref name="with"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector.</param>
        /// <returns>A <see cref="Basis"/> representing the outer product matrix.</returns>
        public static Basis Outer(this Vector3 @this, Vector3 with)
        {
            return new Basis {
                Row0 = Vector3.Create(@this.X) * with,
                Row1 = Vector3.Create(@this.Y) * with,
                Row2 = Vector3.Create(@this.Z) * with
            };
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
        public static Vector3 PosMod(this Vector3 @this, float mod)
        {
            return new Vector3(
                Mathf.PosMod(@this.X, mod),
                Mathf.PosMod(@this.Y, mod),
                Mathf.PosMod(@this.Z, mod)
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
        public static Vector3 PosMod(this Vector3 @this, Vector3 modv)
        {
            return new Vector3(
                Mathf.PosMod(@this.X, modv.X),
                Mathf.PosMod(@this.Y, modv.Y),
                Mathf.PosMod(@this.Z, modv.Z)
            );
        }

        /// <summary>
        /// Returns a new vector resulting from projecting this vector onto the given vector <paramref name="onNormal"/>.
        /// The resulting new vector is parallel to <paramref name="onNormal"/>.
        /// See also <see cref="Slide"/>.
        /// Note: If the vector <paramref name="onNormal"/> is a zero vector, the components of the resulting new vector will be <see cref="float.NaN"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="onNormal">The vector to project onto.</param>
        /// <returns>The projected vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Project(this Vector3 @this, Vector3 onNormal)
        {
            return onNormal * (@this.Dot(onNormal) / onNormal.LengthSquared());
        }

        /// <summary>
        /// Returns this vector reflected from a plane defined by the given <paramref name="normal"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="normal">The normal vector defining the plane to reflect from. Must be normalized.</param>
        /// <returns>The reflected vector.</returns>
        public static Vector3 Reflect(this Vector3 @this, Vector3 normal) => Vector3.Reflect(@this, normal);

        /// <summary>
        /// Rotates this vector around a given <paramref name="axis"/> vector by <paramref name="angle"/> (in radians).
        /// The <paramref name="axis"/> vector must be a normalized vector.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="axis">The vector to rotate around. Must be normalized.</param>
        /// <param name="angle">The angle to rotate by, in radians.</param>
        /// <returns>The rotated vector.</returns>
        public static Vector3 Rotated(this Vector3 @this, Vector3 axis, float angle)
        {
#if DEBUG
            if (!axis.IsNormalized())
            {
                throw new ArgumentException("Argument is not normalized.", nameof(axis));
            }
#endif
            return new Basis(axis, angle) * @this;
        }

        /// <summary>
        /// Returns this vector with all components rounded to the nearest integer,
        /// with halfway cases rounded towards the nearest multiple of two.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The rounded vector.</returns>
        public static Vector3 Round(this Vector3 @this) => Vector3.Round(@this);

        /// <summary>
        /// Returns a vector with each component set to one or negative one, depending
        /// on the signs of this vector's components, or zero if the component is zero,
        /// by calling <see cref="float.Sign(float)"/> on each component.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A vector with all components as either <c>1</c>, <c>-1</c>, or <c>0</c>.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Sign(this Vector3 @this)
        {
            return new Vector3(
                float.Sign(@this.X),
                float.Sign(@this.Y),
                float.Sign(@this.Z)
            );
        }

        /// <summary>
        /// Returns the signed angle to the given vector, in radians.
        /// The sign of the angle is positive in a counter-clockwise
        /// direction and negative in a clockwise direction when viewed
        /// from the side specified by the <paramref name="axis"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to compare this vector to.</param>
        /// <param name="axis">The reference axis to use for the angle sign.</param>
        /// <returns>The signed angle between the two vectors, in radians.</returns>
        public static float SignedAngleTo(this Vector3 @this, Vector3 to, Vector3 axis)
        {
            Vector3 crossTo = @this.Cross(to);
            float unsignedAngle = float.Atan2(crossTo.Length(), @this.Dot(to));
            float sign = crossTo.Dot(axis);
            return (sign < 0) ? -unsignedAngle : unsignedAngle;
        }

        /// <summary>
        /// Returns the result of the spherical linear interpolation between
        /// this vector and <paramref name="to"/> by amount <paramref name="weight"/>.
        ///
        /// This method also handles interpolating the lengths if the input vectors
        /// have different lengths. For the special case of one or both input vectors
        /// having zero length, this method behaves like <see cref="Lerp"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The destination vector for interpolation.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting vector of the interpolation.</returns>
        public static Vector3 Slerp(this Vector3 @this, Vector3 to, float weight)
        {
            float startLengthSquared = @this.LengthSquared();
            float endLengthSquared = to.LengthSquared();
            if (startLengthSquared == 0.0 || endLengthSquared == 0.0)
            {
                // Zero length vectors have no angle, so the best we can do is either lerp or throw an error.
                return @this.Lerp(to, weight);
            }
            Vector3 axis = @this.Cross(to);
            float axisLengthSquared = axis.LengthSquared();
            if (axisLengthSquared == 0.0)
            {
                // Colinear vectors have no rotation axis or angle between them, so the best we can do is lerp.
                return @this.Lerp(to, weight);
            }
            axis /= float.Sqrt(axisLengthSquared);
            float startLength = float.Sqrt(startLengthSquared);
            float resultLength = float.Lerp(startLength, float.Sqrt(endLengthSquared), weight);
            float angle = @this.AngleTo(to);
            return @this.Rotated(axis, angle * weight) * (resultLength / startLength);
        }

        /// <summary>
        /// Returns a new vector resulting from sliding this vector along a plane with normal <paramref name="normal"/>.
        /// The resulting new vector is perpendicular to <paramref name="normal"/>, and is equivalent to this vector minus its projection on <paramref name="normal"/>.
        /// See also <see cref="Project"/>.
        /// Note: The vector <paramref name="normal"/> must be normalized. See also <see cref="Normalized"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="normal">The normal vector of the plane to slide on.</param>
        /// <returns>The slid vector.</returns>
        public static Vector3 Slide(this Vector3 @this, Vector3 normal)
        {
            return @this - (normal * @this.Dot(normal));
        }

        /// <summary>
        /// Returns a new vector with each component snapped to the nearest multiple of the corresponding component in <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="step">A vector value representing the step size to snap to.</param>
        /// <returns>The snapped vector.</returns>
        public static Vector3 Snapped(this Vector3 @this, Vector3 step)
        {
            return new Vector3
            (
                Mathf.Snapped(@this.X, step.X),
                Mathf.Snapped(@this.Y, step.Y),
                Mathf.Snapped(@this.Z, step.Z)
            );
        }

        /// <summary>
        /// Returns a new vector with each component snapped to the nearest multiple of <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="step">The step size to snap to.</param>
        /// <returns>The snapped vector.</returns>
        public static Vector3 Snapped(this Vector3 @this, float step)
        {
            return new Vector3
            (
                Mathf.Snapped(@this.X, step),
                Mathf.Snapped(@this.Y, step),
                Mathf.Snapped(@this.Z, step)
            );
        }

        /// <summary>
        /// Returns the octahedral-encoded (oct32) form of this Vector3 as a Vector2. Since a Vector2 occupies 1/3 less memory compared to Vector3,
        /// this form of compression can be used to pass greater amounts of normalized Vector3s without increasing storage or memory requirements.
        /// See also <see cref="Normalized"/>, <see cref="OctahedronDecode(Vector2)"/>.
        /// Note: OctahedronEncode can only be used for normalized vectors. OctahedronEncode does not check whether this Vector3 is normalized,
        /// and will return a value that does not decompress to the original value if the Vector3 is not normalized.
		/// Note: Octahedral compression is lossy, although visual differences are rarely perceptible in real world scenarios.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The encoded Vector2.</returns>
        public static Vector2 OctahedronEncode(this Vector3 @this)
        {
            @this /= Vector3.Sum(Vector3.Abs(@this));
            Vector2 o;
            if (@this.Z >= 0.0f)
            {
                o = @this.AsVector2();
            }
            else
            {
                o = (Vector2.One - Vector2.Abs(Vector2.Create(@this.Y, @this.X))) * Vector2.ConditionalSelect(Vector2.GreaterThanOrEqual(@this.AsVector2(), Vector2.Zero), Vector2.One, -Vector2.One);
            }
            return (o * 0.5f) + Vector2.Create(0.5f);
        }

        /// <summary>
        /// Returns the Vector3 from an octahedral-compressed form created using <see cref="OctahedronEncode"/> (stored as a Vector2).
        /// </summary>
        /// <param name="oct">Encoded Vector2</param>
        /// <returns>The decoded normalized Vector3.</returns>
        public static Vector3 OctahedronDecode(Vector2 oct)
        {
            var f = new Vector2(oct.X * 2.0f - 1.0f, oct.Y * 2.0f - 1.0f);
            var n = new Vector3(f.X, f.Y, 1.0f - float.Abs(f.X) - float.Abs(f.Y));
            float t = float.Clamp(-n.Z, 0.0f, 1.0f);
            n.X += n.X >= 0 ? -t : t;
            n.Y += n.Y >= 0 ? -t : t;
            return n.Normalized();
        }

        /// <summary>
        /// Returns <see langword="true"/> if this vector and <paramref name="other"/> are approximately equal,
        /// by running <see cref="Mathf.IsEqualApprox(real_t, real_t)"/> on each component.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="other">The other vector to compare.</param>
        /// <returns>Whether or not the vectors are approximately equal.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsEqualApprox(this Vector3 @this, Vector3 other) => Vector3.Abs(@this - other) < Vector3.Create(Mathf.Epsilon);

        /// <summary>
        /// Returns <see langword="true"/> if this vector's values are approximately zero.
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsZeroApprox(this Vector3 @this) => Vector3.Abs(@this) < Vector3.Create(Mathf.Epsilon);

        /// <summary>
        /// Returns <see langword="true"/> if the vector is exactly equal to zero.
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsExactlyZero(this Vector3 @this) => Vector128.AllWhereAllBitsSet(Vector3.IsZero(@this).AsVector128().AsUInt32() | Vector128.Create(0, 0, 0, uint.MaxValue));

        extension(Vector3 @this)
        {
            /// <inheritdoc cref="Vector3.PositiveInfinity"/>
            public static Vector3 Inf => Vector3.PositiveInfinity;

            /// <summary>
            /// Up unit vector.
            /// </summary>
            /// <value>Equivalent to <c>new Vector3(0, 1, 0)</c>.</value>
            public static Vector3 Up => new(0, 1, 0);
            /// <summary>
            /// Down unit vector.
            /// </summary>
            /// <value>Equivalent to <c>new Vector3(0, -1, 0)</c>.</value>
            public static Vector3 Down => new(0, -1, 0);
            /// <summary>
            /// Right unit vector. Represents the local direction of right,
            /// and the global direction of east.
            /// </summary>
            /// <value>Equivalent to <c>new Vector3(1, 0, 0)</c>.</value>
            public static Vector3 Right => new(1, 0, 0);
            /// <summary>
            /// Left unit vector. Represents the local direction of left,
            /// and the global direction of west.
            /// </summary>
            /// <value>Equivalent to <c>new Vector3(-1, 0, 0)</c>.</value>
            public static Vector3 Left => new(-1, 0, 0);
            /// <summary>
            /// Forward unit vector. Represents the local direction of forward,
            /// and the global direction of north.
            /// </summary>
            /// <value>Equivalent to <c>new Vector3(0, 0, -1)</c>.</value>
            public static Vector3 Forward => new(0, 0, -1);
            /// <summary>
            /// Back unit vector. Represents the local direction of back,
            /// and the global direction of south.
            /// </summary>
            /// <value>Equivalent to <c>new Vector3(0, 0, 1)</c>.</value>
            public static Vector3 Back => new(0, 0, 1);

            /// <summary>
            /// Unit vector pointing towards the left side of imported 3D assets.
            /// </summary>
            public static Vector3 ModelLeft => get_Left();
            /// <summary>
            /// Unit vector pointing towards the right side of imported 3D assets.
            /// </summary>
            public static Vector3 ModelRight => get_Right();
            /// <summary>
            /// Unit vector pointing towards the top side (up) of imported 3D assets.
            /// </summary>
            public static Vector3 ModelTop => get_Up();
            /// <summary>
            /// Unit vector pointing towards the bottom side (down) of imported 3D assets.
            /// </summary>
            public static Vector3 ModelBottom => get_Down();
            /// <summary>
            /// Unit vector pointing towards the front side (facing forward) of imported 3D assets.
            /// </summary>
            public static Vector3 ModelFront => get_Back(); // Not front!
            /// <summary>
            /// Unit vector pointing towards the rear side (back) of imported 3D assets.
            /// </summary>
            public static Vector3 ModelRear => get_Forward(); // Not back!

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are greater than those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator >(Vector3 left, Vector3 right) => Vector128.AllWhereAllBitsSet(
                    Vector128.BitwiseOr(
                        Vector128.GreaterThan(left.AsVector128Unsafe(), right.AsVector128Unsafe()).AsUInt32(),
                        Vector128.Create(0, 0, 0, uint.MaxValue)
                    )
                );
                // Vector3.GreaterThanAll(left, right); // RUNTIME BUG: This does not work, #125013

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are less than those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator <(Vector3 left, Vector3 right) => Vector128.AllWhereAllBitsSet(
                    Vector128.BitwiseOr(
                        Vector128.LessThan(left.AsVector128Unsafe(), right.AsVector128Unsafe()).AsUInt32(),
                        Vector128.Create(0, 0, 0, uint.MaxValue)
                    )
                );
            // Vector3.LessThanAll(left, right); // RUNTIME BUG: This does not work, #125013

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are greater than or equal to those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator >=(Vector3 left, Vector3 right) => Vector128.AllWhereAllBitsSet(
                    Vector128.BitwiseOr(
                        Vector128.GreaterThanOrEqual(left.AsVector128Unsafe(), right.AsVector128Unsafe()).AsUInt32(),
                        Vector128.Create(0, 0, 0, uint.MaxValue)
                    )
                );
            // Vector3.GreaterThanOrEqualAll(left, right); // RUNTIME BUG: This does not work, #125013

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are less than or equal to those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator <=(Vector3 left, Vector3 right) => Vector128.AllWhereAllBitsSet(
                    Vector128.BitwiseOr(
                        Vector128.LessThanOrEqual(left.AsVector128Unsafe(), right.AsVector128Unsafe()).AsUInt32(),
                        Vector128.Create(0, 0, 0, uint.MaxValue)
                    )
                );
            // Vector3.LessThanOrEqualAll(left, right); // RUNTIME BUG: This does not work, #125013

            /// <summary>
            /// Performs the modulus of <paramref name="left"/> and <paramref name="right"/>, which returns the remainder of the division operation <c><paramref name="left"/> / <paramref name="right"/></c>
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static Vector3 operator %(Vector3 left, float right) => new Vector3(left.X % right, left.Y % right, left.Z % right);

            /// <summary>
            /// Performs the modulus of <paramref name="left"/> and <paramref name="right"/>, which returns the remainder of the division operation <c><paramref name="left"/> / <paramref name="right"/></c>
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static Vector3 operator %(Vector3 left, Vector3 right) => new Vector3(left.X % right.X, left.Y % right.Y, left.Z % right.Z);
        }
    }
}
#endif
