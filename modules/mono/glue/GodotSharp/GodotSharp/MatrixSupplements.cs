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
    /// This is because the current fork of the engine provides interoperability with the types provided by <c>System.Numerics</c>,
    /// supplementing the built in Godot type. This class provides methods that allow translation between the two.
    /// <para/>
    /// In general, System types should be preferred when you plan to do a large amount of mathematical operations, as they are hardware
    /// accelerated and can leverage parts of your CPU specifically designed for this purpose.
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
