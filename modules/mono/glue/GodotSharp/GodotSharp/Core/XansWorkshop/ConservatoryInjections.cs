using System;
using System.Text;
using System.Runtime.CompilerServices;

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

        /// <inheritdoc cref="SimulationDomain.SetConservatoryCallbacks(long, long, long, long)"/>
        public static unsafe delegate*<nint> SetConservatoryCallbacks(delegate*<byte*, int, byte*, int, int, void> crash, delegate*<long, bool> tryDestroy, delegate*<bool> isClient, delegate*<long, void> onActiveChanged) {
            return (delegate*<nint>)((nint)SetConservatoryCallbacks((nint)crash, (nint)tryDestroy, (nint)isClient, (nint)onActiveChanged));
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


        [UnsafeAccessor(UnsafeAccessorKind.Field, Name = "_memoryOwn")]
        internal static extern bool MemoryOwn(GodotObject godotObject);

        /// <summary>
        /// A customized implementation of <see cref="GodotObject.Dispose(bool)"/> which prevents a crash caused by how the engine handles
        /// the <see cref="SimulationDomain"/> type internally.
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose(bool disposing) {
            if (MemoryOwn(this)) {
                const string ERR_MSG = $"A managed (C#) {nameof(SimulationDomain)} instance was not a wrapper around an existing internal engine object. This is indicative of outright disregard for the usage guidelines of {nameof(SimulationDomain)}; if you need an isolated render scenario and/or physics space, please see: https://docs.godotengine.org/en/stable/classes/class_subviewport.html";
                GD.PushError(ERR_MSG);
            } else if (NativeInstance != IntPtr.Zero) {
                string errMsg = $"An unmanaged (C++) {nameof(SimulationDomain)} instance's wrapper (the C# instance) was {(disposing ? "disposed of" : "garbage collected")} before being deleted by the engine. This is indicative of outright disregard for the usage guidelines of {nameof(SimulationDomain)}; under no circumstances should you ever manually destroy an instance of this class using disposal, garbage collection, or the {nameof(Free)}/{nameof(QueueFree)} methods.";
                GD.PushError(errMsg);
            } else {
                return; // Actually acceptable.
            }
            GD.PushError("The game will likely crash after this message as a result of this mistake.");
            base.Dispose(disposing);
        }

    }

    partial class ConservatoryDebugBridge {

        /// <inheritdoc cref="ConservatoryDebugBridge.SetPtrs(long, long)"/>
        public static unsafe void SetPtrs(bool* breakOnError, delegate*<bool> isDebuggerAttached) {
            SetPtrs((nint)breakOnError, (nint)isDebuggerAttached);
        }

        /// <inheritdoc cref="ConservatoryDebugBridge.InterceptGodotLoggingUsing(long)"/>
        public static unsafe void InterceptGodotLoggingUsing(delegate*<void*, byte*, int, byte*, int, byte*, int, byte*, int, int, byte, bool, void> callback) {
            InterceptGodotLoggingUsing((nint)callback);
        }

    }
}
