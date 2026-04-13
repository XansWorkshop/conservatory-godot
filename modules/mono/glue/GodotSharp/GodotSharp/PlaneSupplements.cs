#if USING_SYSTEM_NUMERICS_VECTORS
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Text;

namespace Godot
{
    /// <summary>
    /// Added by Xan for Godot: Conservatory Edition
    /// <para/>
    /// Extension members for <see cref="Plane"/> specifically. This is because the current fork of the engine relies
    /// on the type provided by <c>System.Numerics</c>, replacing the built in Godot type. This class provides extension
    /// methods and members that enable source code to have 1:1 parity with original code.
    /// </summary>
    public static class PlaneSupplements
    {

        /// <summary>
        /// The same as <see cref="Plane.DotCoordinate(Plane, Vector3)"/>, but this <em>subtracts</em>
        /// <see cref="Plane.D"/> instead of adding it. For example, any hypothetical point that would be
        /// considered "above" a plane created as <c>((0, 1, 0), 0)</c>, if it had a value of
        /// <see cref="Vector3.Y"/> &gt; 0. "Above" is denoted as a return value from this method which
        /// is greater than zero.
        /// </summary>
        /// <param name="this"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        public static float DotNegativeCoordinate(this Plane @this, Vector3 value)
        {
            return Plane.Dot(@this, Vector4.Create(value, -1.0f));
        }


        /// <summary>
        /// Returns the shortest distance from this plane to the position <paramref name="point"/>. If the point is
        /// above the plane, the return value is greater than zero. Otherwise, it is less than or equal to zero when
        /// below or intersecting respectively.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="point">The position to use for the calculation.</param>
        /// <returns>The shortest distance.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float SignedDistanceTo(this Plane @this, Vector3 point) => DotNegativeCoordinate(@this, point);

        /// <summary>
        /// Returns the shortest distance from this plane to the position <paramref name="point"/>. If the point is
        /// above the plane, the return value is greater than zero. Otherwise, it is less than or equal to zero when
        /// below or intersecting respectively.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="point">The position to use for the calculation.</param>
        /// <returns>The shortest distance.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [Obsolete("The preferred method is SignedDistanceTo, which is the same as this method but with a much better name for what this function does.")]
        public static float DistanceTo(this Plane @this, Vector3 point) => DotNegativeCoordinate(@this, point);

        /// <summary>
        /// Returns the center of the plane, the point on the plane closest to the origin.
        /// The point where the normal line going through the origin intersects the plane.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <value>Equivalent to <see cref="Plane.Normal"/> multiplied by <see cref="Plane.D"/>.</value>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 GetCenter(this Plane @this) => @this.Normal * @this.D;

        /// <summary>
        /// Returns <see langword="true"/> if point is inside the plane.
        /// Comparison uses a custom minimum tolerance threshold.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="point">The point to check.</param>
        /// <param name="tolerance">The tolerance threshold.</param>
        /// <returns>A <see langword="bool"/> for whether or not the plane has the point.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool HasPoint(this Plane @this, Vector3 point, float tolerance = Mathf.Epsilon) => float.Abs(Plane.DotCoordinate(@this, point)) <= tolerance;

        /// <summary>
        /// Returns the intersection point of the three planes: <paramref name="b"/>, <paramref name="c"/>,
        /// and this plane. If no intersection is found, <see langword="null"/> is returned.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="b">One of the three planes to use in the calculation.</param>
        /// <param name="c">One of the three planes to use in the calculation.</param>
        /// <returns>The intersection, or <see langword="null"/> if none is found.</returns>
        public static Vector3? Intersect3(this Plane @this, Plane b, Plane c)
        {
            float denom = @this.Normal.Cross(b.Normal).Dot(c.Normal);
            if (Mathf.IsZeroApprox(denom)) return null;

            Vector3 result =(b.Normal.Cross(c.Normal) * @this.D) +
                            (c.Normal.Cross(@this.Normal) * b.D) +
                            (@this.Normal.Cross(b.Normal) * c.D);

            return result / denom;
        }

        /// <summary>
        /// Returns the intersection point of a ray consisting of the position <paramref name="from"/>
        /// and the direction normal <paramref name="dir"/> with this plane.
        /// If no intersection is found, <see langword="null"/> is returned.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="from">The start of the ray.</param>
        /// <param name="dir">The direction of the ray, normalized.</param>
        /// <returns>The intersection, or <see langword="null"/> if none is found.</returns>
        public static Vector3? IntersectsRay(this Plane @this, Vector3 from, Vector3 dir)
        {
            float den = Plane.DotNormal(@this, dir);
            if (Mathf.IsZeroApprox(den)) return null;

            float dist = DotNegativeCoordinate(@this, from) / den;
            if (dist > Mathf.Epsilon) return null;
            return from - (dir * dist);
        }

        /// <summary>
        /// Returns the intersection point of a line segment from
        /// position <paramref name="begin"/> to position <paramref name="end"/> with this plane.
        /// If no intersection is found, <see langword="null"/> is returned.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="begin">The start of the line segment.</param>
        /// <param name="end">The end of the line segment.</param>
        /// <returns>The intersection, or <see langword="null"/> if none is found.</returns>
        public static Vector3? IntersectsSegment(this Plane @this, Vector3 begin, Vector3 end)
        {
            Vector3 segment = begin - end;
            float den = Plane.DotNormal(@this, segment);
            if (Mathf.IsZeroApprox(den)) return null;

            float dist = DotNegativeCoordinate(@this, begin) / den;
            if (dist < -Mathf.Epsilon || dist > 1.0f + Mathf.Epsilon) return null;
            return begin - (segment * dist);
        }

        /// <summary>
        /// Returns <see langword="true"/> if this plane is finite, by calling
        /// <see cref="Mathf.IsFinite(float)"/> on each component.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <returns>Whether this vector is finite or not.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsFinite(this Plane @this) => @this.Normal.IsFinite() && Mathf.IsFinite(@this.D);

        /// <summary>
        /// Returns <see langword="true"/> if <paramref name="point"/> is located above the plane.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="point">The point to check.</param>
        /// <returns>A <see langword="bool"/> for whether or not the point is above the plane.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsPointOver(this Plane @this, Vector3 point) => Plane.DotNormal(@this, point) > @this.D;

        /// <summary>
        /// Returns <see langword="true"/> if <paramref name="point"/> is located above the plane, or lies within
        /// the plane.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="point">The point to check.</param>
        /// <returns>A <see langword="bool"/> for whether or not the point is above or within the plane.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsPointOnOrOver(this Plane @this, Vector3 point) => Plane.DotNormal(@this, point) >= @this.D;

        /// <summary>
        /// Returns the plane scaled to unit length.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <returns>A normalized version of the plane.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Plane Normalized(this Plane @this) => Plane.Normalize(@this);

        /// <summary>
        /// Returns the orthogonal projection of <paramref name="point"/> into the plane.
        /// </summary>
        /// <param name="this">This plane.</param>
        /// <param name="point">The point to project.</param>
        /// <returns>The projected point.</returns>
        public static Vector3 Project(this Plane @this, Vector3 point)
        {
            return point - (@this.Normal * @this.SignedDistanceTo(point));
        }

        extension (Plane @this)
        {

            /// <summary>
            /// The X component of the plane's normal vector.
            /// </summary>
            /// <value>Equivalent to <see cref="Plane.Normal"/>'s X value.</value>
            public float X
            {
                get
                {
                    return @this.Normal.X;
                }
                set
                {
                    @this.Normal = @this.Normal with { X = value };
                }
            }

            /// <summary>
            /// The Y component of the plane's normal vector.
            /// </summary>
            /// <value>Equivalent to <see cref="Plane.Normal"/>'s Y value.</value>
            public float Y
            {
                get
                {
                    return @this.Normal.Y;
                }
                set
                {
                    @this.Normal = @this.Normal with { Y = value };
                }
            }

            /// <summary>
            /// The Z component of the plane's normal vector.
            /// </summary>
            /// <value>Equivalent to <see cref="Plane.Normal"/>'s Z value.</value>
            public float Z
            {
                get
                {
                    return @this.Normal.Z;
                }
                set
                {
                    @this.Normal = @this.Normal with { Z = value };
                }
            }

            /// <summary>
            /// A <see cref="Plane"/> that extends in the Y and Z axes (normal vector points +X).
            /// </summary>
            /// <value>Equivalent to <c>new Plane(1, 0, 0, 0)</c>.</value>
            public static Plane PlaneYZ => new Plane(1, 0, 0, 0);

            /// <summary>
            /// A <see cref="Plane"/> that extends in the X and Z axes (normal vector points +Y).
            /// </summary>
            /// <value>Equivalent to <c>new Plane(0, 1, 0, 0)</c>.</value>
            public static Plane PlaneXZ => new Plane(0, 1, 0, 0);

            /// <summary>
            /// A <see cref="Plane"/> that extends in the X and Y axes (normal vector points +Z).
            /// </summary>
            /// <value>Equivalent to <c>new Plane(0, 0, 1, 0)</c>.</value>
            public static Plane PlaneXY => new Plane(0, 0, 1, 0);

            /// <summary>
            /// Returns the negative value of the <see cref="Plane"/>.
            /// This is the same as writing <c>new Plane(-p.Normal, -p.D)</c>.
            /// This operation flips the direction of the normal vector and
            /// also flips the distance value, resulting in a Plane that is
            /// in the same place, but facing the opposite direction.
            /// </summary>
            /// <param name="plane">The plane to negate/flip.</param>
            /// <returns>The negated/flipped plane.</returns>
            public static Plane operator -(Plane plane)
            {
                return (-plane.AsVector4()).AsPlane();
            }

            /// <summary>
            /// Create a new <see cref="Plane"/> at the provided <paramref name="point"/> with
            /// the provided <paramref name="normal"/>.
            /// </summary>
            /// <param name="point">The location of the plane's center.</param>
            /// <param name="normal">The direction that the plane is facing.</param>
            public static Plane CreateAt(Vector3 point, Vector3 normal) {
                return new Plane(Vector4.Create(normal, normal.Dot(point)));
            }
        }
    }
}
#endif
