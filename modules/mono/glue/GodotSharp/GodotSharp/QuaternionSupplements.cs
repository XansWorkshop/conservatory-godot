#if USING_SYSTEM_NUMERICS_VECTORS
using System;
using System.Collections;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics.X86;
using System.Text;

namespace Godot
{

    /// <summary>
    /// Added by Xan for Godot: Conservatory Edition
    /// <para/>
    /// Extension members for <see cref="Vector4"/> specifically.
    /// </summary>
    public static class QuaternionSupplements
    {
#pragma warning disable CS1591
        /// <summary>
        /// Represents an axis of a <see cref="Vector4"/>.
        /// </summary>
        public enum Axis { X, Y, Z, W }
#pragma warning restore CS1591


        /// <summary>
        /// Returns the angle between this quaternion and <paramref name="to"/>.
        /// This is the magnitude of the angle you would need to rotate
        /// by to get from one to the other.
        ///
        /// Note: This method has an abnormally high amount
        /// of floating-point error, so methods such as
        /// <see cref="Mathf.IsZeroApprox(float)"/> will not work reliably.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <param name="to">The other quaternion.</param>
        /// <returns>The angle between the quaternions.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float AngleTo(this Quaternion @this, Quaternion to)
        {
            float dot = Quaternion.Dot(@this, to);
            return float.Acos(float.Clamp(dot * dot * 2f - 1f, -1f, 1f));
        }

        /// <summary>
        /// Performs a spherical cubic interpolation between quaternions <paramref name="preA"/>, this quaternion,
        /// <paramref name="b"/>, and <paramref name="postB"/>, by the given amount <paramref name="weight"/>.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <param name="b">The destination quaternion.</param>
        /// <param name="preA">A quaternion before this quaternion.</param>
        /// <param name="postB">A quaternion after <paramref name="b"/>.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The interpolated quaternion.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Quaternion SphericalCubicInterpolate(this Quaternion @this, Quaternion b, Quaternion preA, Quaternion postB, float weight)
        {
#if DEBUG
            if (!@this.IsNormalized())
            {
                throw new InvalidOperationException("Quaternion is not normalized");
            }
            if (!b.IsNormalized())
            {
                throw new ArgumentException("Argument is not normalized", nameof(b));
            }
#endif

            // Align flip phases.
            Quaternion fromQ = new Basis(@this).GetRotationQuaternion();
            Quaternion preQ = new Basis(preA).GetRotationQuaternion();
            Quaternion toQ = new Basis(b).GetRotationQuaternion();
            Quaternion postQ = new Basis(postB).GetRotationQuaternion();

            // Flip quaternions to shortest path if necessary.
            bool flip1 = float.Sign(fromQ.Dot(preQ)) < 0;
            preQ = flip1 ? -preQ : preQ;
            bool flip2 = float.Sign(fromQ.Dot(toQ)) < 0;
            toQ = flip2 ? -toQ : toQ;
            bool flip3 = flip2 ? toQ.Dot(postQ) <= 0 : float.Sign(toQ.Dot(postQ)) < 0;
            postQ = flip3 ? -postQ : postQ;

            // Calc by Expmap in fromQ space.
            //Quaternion lnFrom = default;
            Quaternion fromQInv = fromQ.Inverse();
            Quaternion lnTo = (fromQInv * toQ).Log();
            Quaternion lnPre = (fromQInv * preQ).Log();
            Quaternion lnPost = (fromQInv * postQ).Log();
            //Quaternion ln = lnFrom.XYZ.CubicInterpolate(lnTo.XYZ, lnPre.XYZ, lnPost.XYZ, weight).AsVector4().AsQuaternion();
            Quaternion ln = Vector3.Zero.CubicInterpolate(lnTo.XYZ, lnPre.XYZ, lnPost.XYZ, weight).AsVector4().AsQuaternion();
            Quaternion q1 = fromQ * ln.Exp();

            // Calc by Expmap in toQ space.
            Quaternion toQInv = toQ.Inverse();
            //lnTo = default;
            Quaternion lnFrom = (toQInv * fromQ).Log();
            lnPre = (toQInv * preQ).Log();
            lnPost = (toQInv * postQ).Log();
            //ln = lnFrom.XYZ.CubicInterpolate(lnTo.XYZ, lnPre.XYZ, lnPost.XYZ, weight).AsVector4().AsQuaternion();
            ln = lnFrom.XYZ.CubicInterpolate(Vector3.Zero, lnPre.XYZ, lnPost.XYZ, weight).AsVector4().AsQuaternion();
            Quaternion q2 = toQ * ln.Exp();

            // To cancel error made by Expmap ambiguity, do blending.
            return q1.Slerp(q2, weight);
        }

        /// <summary>
        /// Performs a spherical cubic interpolation between quaternions <paramref name="preA"/>, this quaternion,
        /// <paramref name="b"/>, and <paramref name="postB"/>, by the given amount <paramref name="weight"/>.
        /// It can perform smoother interpolation than <see cref="SphericalCubicInterpolate"/>
        /// by the time values.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <param name="b">The destination quaternion.</param>
        /// <param name="preA">A quaternion before this quaternion.</param>
        /// <param name="postB">A quaternion after <paramref name="b"/>.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <param name="bT"></param>
        /// <param name="preAT"></param>
        /// <param name="postBT"></param>
        /// <returns>The interpolated quaternion.</returns>
        public static Quaternion SphericalCubicInterpolateInTime(this Quaternion @this, Quaternion b, Quaternion preA, Quaternion postB, float weight, float bT, float preAT, float postBT)
        {
#if DEBUG
            if (!@this.IsNormalized())
            {
                throw new InvalidOperationException("Quaternion is not normalized");
            }
            if (!b.IsNormalized())
            {
                throw new ArgumentException("Argument is not normalized", nameof(b));
            }
#endif

            // Align flip phases.
            Quaternion fromQ = new Basis(@this).GetRotationQuaternion();
            Quaternion preQ = new Basis(preA).GetRotationQuaternion();
            Quaternion toQ = new Basis(b).GetRotationQuaternion();
            Quaternion postQ = new Basis(postB).GetRotationQuaternion();

            // Flip quaternions to shortest path if necessary.
            bool flip1 = float.Sign(fromQ.Dot(preQ)) < 0;
            preQ = flip1 ? -preQ : preQ;
            bool flip2 = float.Sign(fromQ.Dot(toQ)) < 0;
            toQ = flip2 ? -toQ : toQ;
            bool flip3 = flip2 ? toQ.Dot(postQ) <= 0 : float.Sign(toQ.Dot(postQ)) < 0;
            postQ = flip3 ? -postQ : postQ;

            // Calc by Expmap in fromQ space.
            //Quaternion lnFrom = default;
            Quaternion fromQInv = fromQ.Inverse();
            Quaternion lnTo = (fromQInv * toQ).Log();
            Quaternion lnPre = (fromQInv * preQ).Log();
            Quaternion lnPost = (fromQInv * postQ).Log();
            //Quaternion ln = lnFrom.XYZ.CubicInterpolateInTime(lnTo.XYZ, lnPre.XYZ, lnPost.XYZ, weight, bT, preAT, postBT).AsVector4().AsQuaternion();
            Quaternion ln = Vector3.Zero.CubicInterpolateInTime(lnTo.XYZ, lnPre.XYZ, lnPost.XYZ, weight, bT, preAT, postBT).AsVector4().AsQuaternion(); ;
            Quaternion q1 = fromQ * ln.Exp();

            // Calc by Expmap in toQ space.
            Quaternion toQInv = toQ.Inverse();
            //lnTo = default;
            Quaternion lnFrom = (toQInv * fromQ).Log();
            lnPre = (toQInv * preQ).Log();
            lnPost = (toQInv * postQ).Log();
            //ln = lnFrom.XYZ.CubicInterpolateInTime(lnTo.XYZ, lnPre.XYZ, lnPost.XYZ, weight, bT, preAT, postBT).AsVector4().AsQuaternion();
            ln = lnFrom.XYZ.CubicInterpolateInTime(Vector3.Zero, lnPre.XYZ, lnPost.XYZ, weight, bT, preAT, postBT).AsVector4().AsQuaternion();
            Quaternion q2 = toQ * ln.Exp();

            // To cancel error made by Expmap ambiguity, do blending.
            return q1.Slerp(q2, weight);
        }

        /// <summary>
        /// Returns the dot product of two quaternions.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <param name="b">The other quaternion.</param>
        /// <returns>The dot product.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Dot(this Quaternion @this, Quaternion b) => Quaternion.Dot(@this, b);

        /// <summary>
        /// (No documentation was provided for this method and I (Xan) have no clue what it does, sorry).
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <returns></returns>
        public static Quaternion Exp(this Quaternion @this)
        {
            Vector3 v = @this.XYZ;
            float theta = v.Length();
            if (theta < Mathf.Epsilon)
            {
                return Quaternion.Identity;
            }
            return new Quaternion(v.Normalized(), theta);
        }

        /// <summary>
        /// The opposite of <see cref="Quaternion.CreateFromAxisAngle(Vector3, float)"/>; this returns the angle parameter.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float GetAngle(this Quaternion @this)
        {
            return 2f * float.Acos(@this.W);
        }

        /// <summary>
        /// The opposite of <see cref="Quaternion.CreateFromAxisAngle(Vector3, float)"/>; this returns the axis parameter.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 GetAxis(this Quaternion @this)
        {
            float w = @this.W;
            if (float.Abs(w) > 1 - Mathf.Epsilon)
            {
                return @this.XYZ;
            }

            return @this.XYZ / float.Sqrt(1f - w * w);
        }

        /// <summary>
        /// Returns Euler angles (in the YXZ convention: when decomposing,
        /// first Z, then X, and Y last) corresponding to the rotation
        /// represented by the unit quaternion. Returned vector contains
        /// the rotation angles in the format (X angle, Y angle, Z angle).
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <param name="order">The order by which each individual axis's rotation must be applied to get this rotation back from euler angles.</param>
        /// <returns>The Euler angle representation of this quaternion.</returns>
        public static Vector3 GetEuler(this Quaternion @this, EulerOrder order = EulerOrder.Yxz)
        {
#if DEBUG
            if (!@this.IsNormalized())
            {
                throw new InvalidOperationException("Quaternion is not normalized.");
            }
#endif
            return (new Basis(@this)).GetEuler(order);
        }

        /// <summary>
        /// Returns the inverse of the quaternion.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <returns>The inverse quaternion.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Quaternion Inverse(this Quaternion @this) => Quaternion.Inverse(@this);

        /// <summary>
        /// Returns <see langword="true"/> if this quaternion is finite, that is, no component is infinity nor NaN.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <returns>Whether this vector is finite or not.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsFinite(this Quaternion @this) => @this.AsVector4().IsFinite();

        /// <summary>
        /// Returns whether the quaternion is normalized or not.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <returns>A <see langword="bool"/> for whether the quaternion is normalized or not.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsNormalized(this Quaternion @this) => @this.AsVector4().IsNormalized();

        /// <summary>
        /// (No documentation was provided for this method and I (Xan) have no clue what it does, sorry).
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Quaternion Log(this Quaternion @this)
        {
            Vector3 v = @this.GetAxis() * @this.GetAngle();
            return v.AsVector4().AsQuaternion();
        }

        /// <summary>
        /// Returns a copy of the quaternion, normalized to unit length.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <returns>The normalized quaternion.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Quaternion Normalized(this Quaternion @this) => Quaternion.Normalize(@this);

        /// <summary>
        /// Returns the result of the spherical linear interpolation between
        /// this quaternion and <paramref name="to"/> by amount <paramref name="weight"/>.
        ///
        /// Note: Both quaternions must be normalized.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <param name="to">The destination quaternion for interpolation. Must be normalized.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting quaternion of the interpolation.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Quaternion Slerp(this Quaternion @this, Quaternion to, float weight) => Quaternion.Slerp(@this, to, weight);

        /// <summary>
        /// Returns the result of the spherical linear interpolation between
        /// this quaternion and <paramref name="to"/> by amount <paramref name="weight"/>, but without
        /// checking if the rotation path is not bigger than 90 degrees.
        /// </summary>
        /// <param name="this">This quaternion.</param>
        /// <param name="to">The destination quaternion for interpolation. Must be normalized.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting quaternion of the interpolation.</returns>
        public static Quaternion Slerpni(this Quaternion @this, Quaternion to, float weight)
        {
#if DEBUG
            if (!@this.IsNormalized())
            {
                throw new InvalidOperationException("Quaternion is not normalized");
            }
            if (!to.IsNormalized())
            {
                throw new ArgumentException("Argument is not normalized", nameof(to));
            }
#endif

            float dot = @this.Dot(to);
            if (float.Abs(dot) > 0.9999f) return @this;
            float theta = float.Acos(dot);
            float sinT = 1.0f / float.Sin(theta);
            float newFactor = float.Sin(weight * theta) * sinT;
            float invFactor = float.Sin((1.0f - weight) * theta) * sinT;
            return ((@this.AsVector4() * invFactor) + (newFactor * to.AsVector4())).AsQuaternion();
        }


        extension(Quaternion @this)
        {

            /// <summary>
            /// Returns the X, Y, and Z values of this quaternion as a <see cref="Vector3"/>.
            /// </summary>
            public Vector3 XYZ => @this.AsVector4().AsVector3();

        }
    }
}
#endif
