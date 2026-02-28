#if USING_SYSTEM_NUMERICS_VECTORS
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace Godot
{

    /// <summary>
    /// Added by Xan for Godot: Conservatory Edition
    /// <para/>
    /// Extension members for <see cref="Transform2D"/>, <see cref="Basis"/>, <see cref="Transform3D"/>, and <see cref="Projection"/>.
    /// </summary>
    public static class MatrixSupplements
    {

        /// <summary>
        /// Converts this <see cref="Matrix3x2"/> to a Godot <see cref="Transform2D"/>.
        /// </summary>
        /// <param name="matrix"></param>
        /// <returns></returns>
        public static Transform2D ToTransform2D(in this Matrix3x2 matrix) => Transform2D.FromSystemMatrix(in matrix);

        /// <summary>
        /// Converts this <see cref="Matrix4x4"/> to a Godot <see cref="Basis"/> (3x3).
        /// </summary>
        /// <param name="matrix"></param>
        /// <returns></returns>
        public static Basis ToBasis(in this Matrix4x4 matrix) => Basis.FromSystemMatrix(in matrix);

        /// <summary>
        /// Converts this <see cref="Matrix4x4"/> to a Godot <see cref="Transform3D"/> (4x3).
        /// </summary>
        /// <param name="matrix"></param>
        /// <returns></returns>
        public static Transform3D ToTransform3D(in this Matrix4x4 matrix) => Transform3D.FromSystemMatrix(in matrix);

        /// <summary>
        /// Converts this <see cref="Matrix4x4"/> to a Godot <see cref="Projection"/> (4x4).
        /// </summary>
        /// <param name="matrix"></param>
        /// <returns></returns>
        public static Projection ToProjection(in this Matrix4x4 matrix) => Projection.FromSystemMatrix(in matrix);

    }
}
#endif
