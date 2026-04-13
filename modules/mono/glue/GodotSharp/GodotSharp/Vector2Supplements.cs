#if USING_SYSTEM_NUMERICS_VECTORS
using System;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics;

namespace Godot
{
    /// <summary>
    /// Added by Xan for Godot: Conservatory Edition
    /// <para/>
    /// Extension members for <see cref="Vector2"/> specifically. This is because the current fork of the engine relies
    /// on the type provided by <c>System.Numerics</c>, replacing the built in Godot type. This class provides extension
    /// methods and members that enable source code to have 1:1 parity with original code.
    /// </summary>
    public static class Vector2Supplements
    {
#pragma warning disable CS1591
        /// <summary>
        /// Represents an axis of a <see cref="Vector2"/>.
        /// </summary>
        public enum Axis { X, Y }
#pragma warning restore CS1591

        /// <inheritdoc cref="Vector2.Abs"/>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Abs(this Vector2 @this) => Vector2.Abs(@this);

        /// <summary>
        /// Returns this vector's angle with respect to the X axis, or (1, 0) vector, in radians.
        ///
        /// Equivalent to the result of <see cref="float.Atan2(float, float)"/> when
        /// called with the vector's <see cref="Vector2.Y"/> and <see cref="Vector2.X"/> as parameters: <c>float.Atan2(v.Y, v.X)</c>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The angle of this vector, in radians.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Angle(this Vector2 @this) => float.Atan2(@this.Y, @this.X);

        /// <summary>
        /// Returns the angle to the given vector, in radians.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to compare this vector to.</param>
        /// <returns>The angle between the two vectors, in radians.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float AngleTo(this Vector2 @this, Vector2 to) => float.Atan2(@this.Cross(to), @this.Dot(to));

        /// <summary>
        /// Returns the angle between the line connecting the two points and the X axis, in radians.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to compare this vector to.</param>
        /// <returns>The angle between the two vectors, in radians.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float AngleToPoint(this Vector2 @this, Vector2 to) => float.Atan2(to.Y - @this.Y, to.X - @this.X);

        /// <summary>
        /// Returns the aspect ratio of this vector, the ratio of <see cref="Vector2.X"/> to <see cref="Vector2.Y"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The <see cref="Vector2.X"/> component divided by the <see cref="Vector2.Y"/> component.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Aspect(this Vector2 @this) => @this.X / @this.Y;

        /// <summary>
        /// Returns the vector "bounced off" from a plane defined by the given normal.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="normal">The normal vector defining the plane to bounce off. Must be normalized.</param>
        /// <returns>The bounced vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Bounce(this Vector2 @this, Vector2 normal) => -Vector2.Reflect(@this, normal);

        /// <summary>
        /// Returns a new vector with all components rounded up (towards positive infinity).
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Ceil(this Vector2 @this) => Vector2.Round(@this, MidpointRounding.ToPositiveInfinity);

        /// <summary>
        /// The same as <see cref="Vector2.ClampNative"/> which limits the value between <paramref name="min"/> and <paramref name="max"/>
        /// using the current processor's technique of handling NaN or scenarios where the min and max are invalid (i.e. min &gt; max)
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="min">The vector with minimum allowed values.</param>
        /// <param name="max">The vector with maximum allowed values.</param>
        /// <returns>The vector with all components clamped.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Clamp(this Vector2 @this, Vector2 min, Vector2 max) => Vector2.ClampNative(@this, min, max);

        /// <summary>
        /// The same as <see cref="Vector2.ClampNative"/> which limits the value between <paramref name="min"/> and <paramref name="max"/>
        /// using the current processor's technique of handling NaN or scenarios where the min and max are invalid (i.e. min &gt; max)
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="min">The vector with minimum allowed values.</param>
        /// <param name="max">The vector with maximum allowed values.</param>
        /// <returns>The vector with all components clamped.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Clamp(this Vector2 @this, float min, float max) => Vector2.ClampNative(@this, Vector2.Create(min), Vector2.Create(max));

        /// <summary>
        /// Returns the cross product of this vector and <paramref name="with"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector.</param>
        /// <returns>The cross product value.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Cross(this Vector2 @this, Vector2 with) => Vector2.Cross(@this, with);

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
        public static Vector2 CubicInterpolate(this Vector2 @this, Vector2 to, Vector2 pre, Vector2 post, float weight)
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
        public static Vector2 CubicInterpolateInTime(this Vector2 @this, Vector2 to, Vector2 pre, Vector2 post, float weight, float toT, float preT, float postT)
        {
            /* Barry-Goldman method */
            float t = toT * weight;//float.Lerp(0.0f, toT, weight);
            Vector2 a1 = Vector2.Lerp(pre, @this, preT == 0 ? 0.0f : (t - preT) / -preT);
            Vector2 a2 = Vector2.Lerp(@this, to, toT == 0 ? 0.5f : t / toT);
            Vector2 a3 = Vector2.Lerp(to, post, postT - toT == 0 ? 1.0f : (t - toT) / (postT - toT));
            Vector2 b1 = Vector2.Lerp(a1, a2, toT - preT == 0 ? 0.0f : (t - preT) / (toT - preT));
            Vector2 b2 = Vector2.Lerp(a2, a3, postT == 0 ? 1.0f : t / postT);
            return Vector2.Lerp(b1, b2, toT == 0 ? 0.5f : t / toT);
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
        public static Vector2 BezierInterpolate(this Vector2 @this, Vector2 control1, Vector2 control2, Vector2 end, float t)
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
        public static Vector2 BezierDerivative(this Vector2 @this, Vector2 control1, Vector2 control2, Vector2 end, float t)
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
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 DirectionTo(this Vector2 @this, Vector2 to) => Vector2.Normalize(to - @this);

        /// <summary>
        /// Returns the squared Euclidean distance between this vector and <paramref name="to"/>.
        /// This method runs faster than <see cref="DistanceTo"/>. Note that for distance
        /// comparisons, <see cref="ManhattanDistanceTo"/> is the most optimized technique.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The squared distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float DistanceSquaredTo(this Vector2 @this, Vector2 to) => Vector2.DistanceSquared(@this, to);

        /// <summary>
        /// Returns the Euclidean distance between this vector and <paramref name="to"/>.
        /// </summary>
        /// <remarks>
        /// For a visual example, refer to this image: <see href="https://en.wikipedia.org/wiki/File:Minkowski_distance_examples.svg"/>
        /// </remarks>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float DistanceTo(this Vector2 @this, Vector2 to) => Vector2.Distance(@this, to);

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Manhattan distance between this vector and <paramref name="to"/>. Manhattan distance is
        /// also sometimes referred to as "taxicab distance" in that it measures a grid-based distance
        /// with no diagonal lines. This is useful for some forms of pathfinding, and is the most optimal
        /// technique for sorting by distance.
        /// </summary>
        /// <remarks>
        /// For a visual example, refer to this image: <see href="https://en.wikipedia.org/wiki/File:Minkowski_distance_examples.svg"/>
        /// </remarks>
        /// <param name="this">This vector.</param>
        /// <param name="to">The other vector to use.</param>
        /// <returns>The Manhattan distance between the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ManhattanDistanceTo(this Vector2 @this, Vector2 to) => (@this - to).ManhattanLength();

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
        public static float ChebyshevDistanceTo(this Vector2 @this, Vector2 to) => (@this - to).ChebyshevLength();

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Manhattan length of this vector. Manhattan length is also sometimes referred to as "taxicab distance"
        /// in that it measures a grid-based distance without diagonal lines.
        /// This is by far the most optimized technique for finding length. Note that if this is used as a radius in Euclidean space,
        /// the shape is not circular, but rather a diamond. This is the best method to use for distance comparison, but note that
        /// to be accurate, <em>both distances</em> must be measured using this method.
        /// </summary>
        /// <remarks>
        /// For a visual example, refer to this image: <see href="https://en.wikipedia.org/wiki/File:Minkowski_distance_examples.svg"/>
        /// </remarks>
        /// <param name="this">This vector.</param>
        /// <returns>The Manhattan length of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ManhattanLength(this Vector2 @this) => Vector2.Sum(Vector2.Abs(@this));

        /// <summary>
        /// <admonition type="note">
        /// <strong>Part of <em>Godot Engine: Conservatory Edition</em>.</strong>
        /// This is not available in the official build of Godot.
        /// </admonition>
        /// <para/>
        /// Returns the Chebyshev length of this vector. This is similar to Manhattan length, but diagonal grid spaces are considered
        /// to be 1 unit of length as well (think of a Queen on a chessboard moving 1 tile; this "1 tile" is any of the 8 directly around
        /// the Queen, including the diagonals).
        /// Note that if this is used as a radius in Euclidean space, the shape is not circular, but rather a square.
        /// To be accurate in comparisons, <em>both distances</em> must be measured using this method.
        /// </summary>
        /// <remarks>
        /// For a visual example, refer to this image: <see href="https://en.wikipedia.org/wiki/File:Minkowski_distance_examples.svg"/>
        /// </remarks>
        /// <param name="this">This vector.</param>
        /// <returns>The Chebyshev length of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ChebyshevLength(this Vector2 @this)
        {
            @this = Vector2.Abs(@this);
            return float.Max(@this.X, @this.Y);
        }

        /// <summary>
        /// Returns the dot product of this vector and <paramref name="with"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The dot product of the two vectors.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Dot(this Vector2 @this, Vector2 with) => Vector2.Dot(@this, with);

        /// <summary>
        /// Returns a new vector with all components rounded down (towards negative infinity).
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Floor(this Vector2 @this) => Vector2.Round(@this, MidpointRounding.ToNegativeInfinity);

        /// <summary>
        /// Returns the inverse of this vector. This is the same as <c>new Vector2(1 / v.X, 1 / v.Y)</c>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The inverse of this vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Inverse(this Vector2 @this) => Vector2.Create(1) / @this;

        /// <summary>
        /// Returns <see langword="true"/> if this vector is finite. A finite value is not infinity nor is it NaN.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>Whether this vector is finite or not.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsFinite(this Vector2 @this) => Vector128.NoneWhereAllBitsSet((Vector128.IsInfinity(@this.AsVector128()) | Vector128.IsNaN(@this.AsVector128())).AsUInt32() & Vector128.Create(uint.MaxValue, uint.MaxValue, 0, 0));

        /// <summary>
        /// Returns <see langword="true"/> if the vector is normalized, and <see langword="false"/> otherwise.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A <see langword="bool"/> indicating whether or not the vector is normalized.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsNormalized(this Vector2 @this) => Mathf.IsOneApprox(@this.LengthSquared());

        /// <summary>
        /// Returns the result of the linear interpolation between
        /// this vector and <paramref name="to"/> by amount <paramref name="weight"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The destination vector for interpolation.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting vector of the interpolation.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Lerp(this Vector2 @this, Vector2 to, float weight) => Vector2.Lerp(@this, to, weight);

        /// <summary>
        /// Returns the vector with a maximum length by limiting its length to <paramref name="length"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="length">The length to limit to.</param>
        /// <returns>The vector with its length limited.</returns>
        public static Vector2 LimitLength(this Vector2 @this, float length = 1.0f)
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
        /// this vector and <paramref name="with"/> with <see cref="Vector2.MaxNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The resulting maximum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Max(this Vector2 @this, Vector2 with) => Vector2.MaxNative(@this, with);

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this vector and <paramref name="with"/> with <see cref="Vector2.MaxNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting maximum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Max(this Vector2 @this, float with) => Vector2.MaxNative(@this, Vector2.Create(with));

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this vector and <paramref name="with"/> with <see cref="Vector2.MinNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other vector to use.</param>
        /// <returns>The resulting minimum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Min(this Vector2 @this, Vector2 with) => Vector2.MinNative(@this, with);

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this vector and <paramref name="with"/> with <see cref="Vector2.MinNative"/>
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting minimum vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Min(this Vector2 @this, float with) => Vector2.MinNative(@this, Vector2.Create(with));

        /// <summary>
        /// Returns the axis of the vector's highest value. See <see cref="Axis"/>.
        /// If both components are equal, this method returns <see cref="Axis.X"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The index of the highest axis.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Axis MaxAxisIndex(this Vector2 @this) => @this.X < @this.Y ? Axis.Y : Axis.X;

        /// <summary>
        /// Returns the axis of the vector's lowest value. See <see cref="Axis"/>.
        /// If both components are equal, this method returns <see cref="Axis.Y"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The index of the lowest axis.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Axis MinAxisIndex(this Vector2 @this) => @this.X < @this.Y ? Axis.X : Axis.Y;

        /// <summary>
        /// Moves this vector toward <paramref name="to"/> by the fixed <paramref name="delta"/> amount.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The vector to move towards.</param>
        /// <param name="delta">The amount to move towards by.</param>
        /// <returns>The resulting vector.</returns>
        public static Vector2 MoveToward(this Vector2 @this, Vector2 to, float delta)
        {
            Vector2 vd = to - @this;
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
        public static Vector2 Normalized(this Vector2 @this)
        {
            if (Mathf.IsZeroApprox(@this.LengthSquared())) return default;
            while (!@this.IsNormalized())
            {
                @this = Vector2.Normalize(@this);
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
        public static Vector2 PosMod(this Vector2 @this, float mod)
        {
            return new Vector2(
                Mathf.PosMod(@this.X, mod),
                Mathf.PosMod(@this.Y, mod)
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
        public static Vector2 PosMod(this Vector2 @this, Vector2 modv)
        {
            return new Vector2(
                Mathf.PosMod(@this.X, modv.X),
                Mathf.PosMod(@this.Y, modv.Y)
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
        public static Vector2 Project(this Vector2 @this, Vector2 onNormal)
        {
            return onNormal * (@this.Dot(onNormal) / onNormal.LengthSquared());
        }

        /// <summary>
        /// Returns this vector reflected from a plane defined by the given <paramref name="normal"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="normal">The normal vector defining the plane to reflect from. Must be normalized.</param>
        /// <returns>The reflected vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Reflect(this Vector2 @this, Vector2 normal) => Vector2.Reflect(@this, normal);

        /// <summary>
        /// Rotates this vector by <paramref name="angle"/> radians.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="angle">The angle to rotate by, in radians.</param>
        /// <returns>The rotated vector.</returns>
        public static Vector2 Rotated(this Vector2 @this, float angle)
        {
            (float sin, float cos) = float.SinCos(angle);
            return new Vector2
            (
                @this.X * cos - @this.Y * sin,
                @this.X * sin + @this.Y * cos
            );
        }

        /// <summary>
        /// Returns this vector with all components rounded to the nearest integer,
        /// with halfway cases rounded towards the nearest multiple of two.
        /// </summary>
        /// <returns>The rounded vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Round(this Vector2 @this) => Vector2.Round(@this);

        /// <summary>
        /// Returns a vector with each component set to one or negative one, depending
        /// on the signs of this vector's components, or zero if the component is zero,
        /// by calling <see cref="Mathf.Sign(float)"/> on each component.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>A vector with all components as either <c>1</c>, <c>-1</c>, or <c>0</c>.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Sign(this Vector2 @this) => new(float.Sign(@this.X), float.Sign(@this.Y));

        /// <summary>
        /// Returns the result of the spherical linear interpolation between
        /// this vector and <paramref name="to"/> by amount <paramref name="weight"/>.
        ///
        /// This method also handles interpolating the lengths if the input vectors
        /// have different lengths. For the special case of one or both input vectors
        /// having zero length, this method behaves like <see cref="Lerp(Vector2, Vector2, float)"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="to">The destination vector for interpolation.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting vector of the interpolation.</returns>
        public static Vector2 Slerp(this Vector2 @this, Vector2 to, float weight)
        {
            float startLengthSquared = @this.LengthSquared();
            float endLengthSquared = to.LengthSquared();
            if (startLengthSquared == 0.0 || endLengthSquared == 0.0)
            {
                // Zero length vectors have no angle, so the best we can do is either lerp or throw an error.
                return @this.Lerp(to, weight);
            }
            float startLength = Mathf.Sqrt(startLengthSquared);
            float resultLength = Mathf.Lerp(startLength, Mathf.Sqrt(endLengthSquared), weight);
            float angle = @this.AngleTo(to);
            return @this.Rotated(angle * weight) * (resultLength / startLength);
        }

        /// <summary>
        /// Returns a new vector resulting from sliding this vector along a line with normal <paramref name="normal"/>.
        /// The resulting new vector is perpendicular to <paramref name="normal"/>, and is equivalent to this vector minus its projection on <paramref name="normal"/>.
        /// See also <see cref="Project"/>.
        /// Note: The vector <paramref name="normal"/> must be normalized. See also <see cref="Normalized"/>.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="normal">The normal vector of the plane to slide on.</param>
        /// <returns>The slid vector.</returns>
        public static Vector2 Slide(this Vector2 @this, Vector2 normal)
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
        public static Vector2 Snapped(this Vector2 @this, Vector2 step)
        {
            return new Vector2(Mathf.Snapped(@this.X, step.X), Mathf.Snapped(@this.Y, step.Y));
        }

        /// <summary>
        /// Returns a new vector with each component snapped to the nearest multiple of <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="step">The step size to snap to.</param>
        /// <returns>The snapped vector.</returns>
        public static Vector2 Snapped(this Vector2 @this, float step)
        {
            return new Vector2(Mathf.Snapped(@this.X, step), Mathf.Snapped(@this.Y, step));
        }

        /// <summary>
        /// Returns a perpendicular vector rotated 90 degrees counter-clockwise
        /// compared to the original, with the same length.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <returns>The perpendicular vector.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 Orthogonal(this Vector2 @this) => new Vector2(@this.Y, -@this.X);

        /// <summary>
        /// Returns <see langword="true"/> if this vector and <paramref name="other"/> are approximately equal,
        /// by running <see cref="Mathf.IsEqualApprox(real_t, real_t)"/> on each component.
        /// </summary>
        /// <param name="this">This vector.</param>
        /// <param name="other">The other vector to compare.</param>
        /// <returns>Whether or not the vectors are approximately equal.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsEqualApprox(this Vector2 @this, Vector2 other) => Vector2.Abs(@this - other) < Vector2.Create(Mathf.Epsilon);

        /// <summary>
        /// Returns <see langword="true"/> if this vector's values are approximately zero.
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsZeroApprox(this Vector2 @this) => Vector2.Abs(@this) < Vector2.Create(Mathf.Epsilon);

        /// <summary>
        /// Returns <see langword="true"/> if the vector is exactly equal to zero.
        /// </summary>
        /// <param name="this">This vector.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsExactlyZero(this Vector2 @this) => Vector128.AllWhereAllBitsSet(Vector2.IsZero(@this).AsVector128().AsUInt32() | Vector128.Create(0, 0, uint.MaxValue, uint.MaxValue));

        extension(Vector2 @this)
        {
            /// <inheritdoc cref="Vector2.PositiveInfinity"/>
            public static Vector2 Inf => Vector2.PositiveInfinity;

            /// <summary>
            /// Up unit vector. Y is down in 2D, so this vector points -Y.
            /// </summary>
            /// <value>Equivalent to <c>new Vector2(0, -1)</c>.</value>
            public static Vector2 Up => new(0, -1);
            /// <summary>
            /// Down unit vector. Y is down in 2D, so this vector points +Y.
            /// </summary>
            /// <value>Equivalent to <c>new Vector2(0, 1)</c>.</value>
            public static Vector2 Down => new(0, 1);
            /// <summary>
            /// Right unit vector. Represents the direction of right.
            /// </summary>
            /// <value>Equivalent to <c>new Vector2(1, 0)</c>.</value>
            public static Vector2 Right => new(1, 0);
            /// <summary>
            /// Left unit vector. Represents the direction of left.
            /// </summary>
            /// <value>Equivalent to <c>new Vector2(-1, 0)</c>.</value>
            public static Vector2 Left => new(-1, 0);

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are greater than those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator >(Vector2 left, Vector2 right) => Vector128.AllWhereAllBitsSet(
                    Vector128.BitwiseOr(
                        Vector128.GreaterThan(left.AsVector128Unsafe(), right.AsVector128Unsafe()).AsUInt32(),
                        Vector128.Create(0, 0, uint.MaxValue, uint.MaxValue)
                    )
                );
            // Vector2.GreaterThanAll(left, right); // RUNTIME BUG: This does not work, #125013

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are less than those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator <(Vector2 left, Vector2 right) => Vector128.AllWhereAllBitsSet(
                    Vector128.BitwiseOr(
                        Vector128.LessThan(left.AsVector128Unsafe(), right.AsVector128Unsafe()).AsUInt32(),
                        Vector128.Create(0, 0, uint.MaxValue, uint.MaxValue)
                    )
                );
            // Vector2.LessThanAll(left, right); // RUNTIME BUG: This does not work, #125013

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are greater than or equal to those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator >=(Vector2 left, Vector2 right) => Vector128.AllWhereAllBitsSet(
                    Vector128.BitwiseOr(
                        Vector128.GreaterThanOrEqual(left.AsVector128Unsafe(), right.AsVector128Unsafe()).AsUInt32(),
                        Vector128.Create(0, 0, uint.MaxValue, uint.MaxValue)
                    )
                );
            // Vector2.GreaterThanOrEqualAll(left, right); // RUNTIME BUG: This does not work, #125013

            /// <summary>
            /// Returns true if both components of <paramref name="left"/> are less than or equal to those of <paramref name="right"/>.
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static bool operator <=(Vector2 left, Vector2 right) => Vector128.AllWhereAllBitsSet(
                    Vector128.BitwiseOr(
                        Vector128.LessThanOrEqual(left.AsVector128Unsafe(), right.AsVector128Unsafe()).AsUInt32(),
                        Vector128.Create(0, 0, uint.MaxValue, uint.MaxValue)
                    )
                );
            // Vector2.LessThanOrEqualAll(left, right); // RUNTIME BUG: This does not work, #125013

            /// <summary>
            /// Performs the modulus of <paramref name="left"/> and <paramref name="right"/>, which returns the remainder of the division operation <c><paramref name="left"/> / <paramref name="right"/></c>
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static Vector2 operator %(Vector2 left, float right) => new Vector2(left.X % right, left.Y % right);

            /// <summary>
            /// Performs the modulus of <paramref name="left"/> and <paramref name="right"/>, which returns the remainder of the division operation <c><paramref name="left"/> / <paramref name="right"/></c>
            /// </summary>
            /// <param name="left"></param>
            /// <param name="right"></param>
            /// <returns></returns>
            public static Vector2 operator %(Vector2 left, Vector2 right) => new Vector2(left.X % right.X, left.Y % right.Y);

            /// <summary>
            /// Creates a unit Vector2 rotated to the given angle. This is equivalent to doing
            /// <c>Vector2(Mathf.Cos(angle), Mathf.Sin(angle))</c> or <c>Vector2.Right.Rotated(angle)</c>.
            /// </summary>
            /// <param name="angle">Angle of the vector, in radians.</param>
            /// <returns>The resulting vector.</returns>
            public static Vector2 FromAngle(float angle)
            {
                (float sin, float cos) = float.SinCos(angle);
                return new Vector2(cos, sin);
            }
        }
    }
}
#endif
