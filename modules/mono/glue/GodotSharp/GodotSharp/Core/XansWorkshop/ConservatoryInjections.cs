using System;
using System.Text;

#nullable disable
namespace Godot {

	partial class CharFXTransform {

        /// <summary>
        /// <strong>Appended by The Conservatory's engine fork. This is not native Godot code, and it will not be available in GDScript.</strong>
        /// <para/>
        /// A <see cref="Rune"/> representing the <see cref="CharFXTransform.GlyphCodepoint"/>. Changing this value affects it and by extension
        /// <see cref="CharFXTransform.GlyphIndex"/> as well.
        /// </summary>
        public Rune Glyph {
			get => new Rune((uint)GlyphCodepoint);
			set => GlyphCodepoint = value.Value;
		}

	}

    partial class SimulationDomain {

        /// <inheritdoc cref="SimulationDomain.SetConservatoryCallbacks(long, long, long)"/>
        public static unsafe delegate*<nint> SetConservatoryCallbacks(delegate*<byte*, int, byte*, int, int, void> crash, delegate*<long, bool> tryDestroy, delegate*<bool> isClient) {
            return (delegate*<nint>)((nint)SetConservatoryCallbacks((nint)crash, (nint)tryDestroy, (nint)isClient));
        }

        /// <summary>
        /// <strong>Appended by The Conservatory's engine fork. This is not native Godot code, and it will not be available in GDScript.</strong>
        /// <para/>
        /// Crashes the game; you should never free a SimulationDomain. Use Destroy() instead.
        /// </summary>
        [Obsolete("SimulationDomain disallows the use of Free(). This will crash the game.", true)]
        public new void Free() { }

        /// <summary>
        /// <strong>Appended by The Conservatory's engine fork. This is not native Godot code, and it will not be available in GDScript.</strong>
        /// <para/>
        /// Crashes the game; you should never free a SimulationDomain. Use Destroy() instead.
        /// </summary>
        [Obsolete("SimulationDomain disallows the use of QueueFree(). This will crash the game.", true)]
        public new void QueueFree() { }

    }

    partial class ConservatoryDebugBridge {

        /// <inheritdoc cref="ConservatoryDebugBridge.SetPtrs(long, long)"/>
        public static unsafe void SetPtrs(bool* breakOnError, delegate*<bool> isDebuggerAttached) {
            SetPtrs((nint)breakOnError, (nint)isDebuggerAttached);
        }

    }
}
