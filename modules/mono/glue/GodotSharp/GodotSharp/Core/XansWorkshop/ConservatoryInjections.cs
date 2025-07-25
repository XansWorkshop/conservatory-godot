using System;
using System.Text;
using System.Runtime.CompilerServices;

#nullable enable
namespace Godot {

    #region Interfaces

    /// <summary>
    /// Container class for interoperability layers between the engine's generated C# types and The Conservatory.
    /// </summary>
    public static class ConservatoryInterop {

        

    }

    #endregion

    #region Class Modifications

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

		/// <summary>
		/// A customized implementation of <see cref="GodotObject.Dispose(bool)"/> which prevents a crash caused by how the engine handles
		/// the <see cref="SimulationDomain"/> type internally.
		/// </summary>
		/// <param name="disposing"></param>
		protected override void Dispose(bool disposing) {
			if (NativeInstance != IntPtr.Zero) {
				string errMsg = $"An unmanaged (C++) {nameof(SimulationDomain)} instance's wrapper (the C# instance) was {(disposing ? "disposed of" : "garbage collected")} before being deleted by the engine. This is indicative of outright disregard for the usage guidelines of {nameof(SimulationDomain)}; under no circumstances should you ever manually destroy an instance of this class using disposal, garbage collection, or the {nameof(Free)}/{nameof(QueueFree)} methods.";
				GD.PushError(errMsg);
			} else {
				return; // Actually acceptable.
			}
			GD.PushError("If you are reading this, congratulations! Please enjoy your complimentary access violation in the CLR garbage collection routine once the next cycle occurs. In the mean time, why don't you consider what you did to get here. :)");
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

		/// <summary>
		/// A customized implementation of <see cref="GodotObject.Dispose(bool)"/> which prevents a crash caused by how the engine handles
		/// the <see cref="ConservatoryDebugBridge"/> type internally.
		/// </summary>
		/// <param name="disposing"></param>
		protected override void Dispose(bool disposing) {
            GD.PushError("Why did you create an instance of this class? This and more at 10 in Unsolved Mysteries, on CBS...");
            GD.PushError("If you are reading this, congratulations! Please enjoy your complimentary access violation in the CLR garbage collection routine once the next cycle occurs. In the mean time, why don't you consider what you did to get here. :)");
			base.Dispose(disposing);
		}

	}

	partial class RichTextLabel {

		/// <summary>
		/// <strong>Added by The Conservatory; this API is not available in base Godot nor is it available in GDScript.</strong>
		/// <para/>
		/// Install a rich text effect by generic type. This just constructs the generic type and calls <see cref="InstallEffect(Variant)"/>.
		/// </summary>
		/// <typeparam name="T">The type of effect to install.</typeparam>
		public void InstallEffect<T>() where T : RichTextEffect, new() {
			InstallEffect(new T());
		}

        /// <summary>
        /// <strong>Added by The Conservatory; this API is not available in base Godot nor is it available in GDScript.</strong>
        /// <para/>
        /// Push a custom effect onto the tag stack, for use in procedurally building text. This is an alias that constructs the generic type,
        /// then calls <see cref="PushCustomfx(RichTextEffect, Godot.Collections.Dictionary)"/>.
        /// </summary>
        /// <typeparam name="T">The type of effect to install.</typeparam>
        public void PushCustomfx<T>(Godot.Collections.Dictionary? environment = null) where T : RichTextEffect, new() {
			PushCustomfx(new T(), environment ?? []);
		}

        /// <summary>
        /// <strong>Added by The Conservatory; this API is not available in base Godot nor is it available in GDScript.</strong>
        /// <para/>
        /// Push a custom effect onto the tag stack, for use in procedurally building text. This is an alias that constructs the generic type,
        /// then calls <see cref="PushCustomfx(RichTextEffect, Godot.Collections.Dictionary)"/>.
        /// </summary>
        /// <typeparam name="T">The type of effect to install.</typeparam>
        public void PushCustomfx<T>(System.Collections.Generic.IEnumerable<ValueTuple<Variant, Variant>>? environment) where T : RichTextEffect, new() {
			Godot.Collections.Dictionary dict = [];
			if (environment != null) {
				foreach ((Variant key, Variant value) in environment) {
					dict[key] = value;
				}
			}

			PushCustomfx(new T(), dict);
		}

	}

	partial class RichTextEffect {

		/// <summary>
		/// <strong>Added by The Conservatory for the .NET Extended API; this API is not available in official Godot builds, nor is it available from GDScript.</strong>
		/// <para/>
		/// The name of the BBCode tag that must be used.
		/// </summary>
		/// <remarks>
		/// Note: This supersedes Godot's previous requirement for a field named <c>bbcode</c>, as additional code has been added
		/// to find this specific case of the property as well for C# naming convention compliance.
		/// </remarks>
		public virtual string BBCode => (string)Get(PrivateStringNames.BBCODE_NAME);

	}

    #endregion

	file static class PrivateStringNames {

		public static readonly StringName BBCODE_NAME = "bbcode";

	}
}
