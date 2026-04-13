using System;
using System.Text;
using System.Runtime.CompilerServices;

#nullable enable
namespace Godot {

    #region Class Modifications

    /*
    partial class CharFXTransform {

		/// <summary>
		/// <strong>Appended by The Conservatory's engine fork. This is not native Godot code, and it will not be available in GDScript.</strong>
		/// <para/>
		/// A <see cref="Rune"/> representing the <see cref="CharFXTransform.GlyphCodepoint"/>. Changing this value affects it and by extension
		/// <see cref="CharFXTransform.GlyphIndex"/> as well.
		/// </summary>
        /// <remarks>
        /// This is not safe to use with languages other than English at this time; this must be fixed.
        /// </remarks>
		public Rune Glyph {
			get => new Rune((uint)GlyphCodepoint);
			set => GlyphCodepoint = value.Value;
		}

	}
    */

    /*
    
	partial class SimulationDomain {


        /// <inheritdoc cref="SimulationDomain.SetIsClientPtr(long)"/>
        //[ConservatoryMkdocsSecurityDeny(Capability.CallingAndDelegation)]
        public static unsafe void SetIsClientPtr(bool* isClient) {
            SetIsClientPtr((nint)isClient);
        }

        // MAGIC BEHAVIOR: Custom docs generator looks for "Ptr" ending to disable calling and delegation.

    }
    */

    partial class ConservatoryDebugBridge {

        /// <inheritdoc cref="ConservatoryDebugBridge.SetPtrs(long, long)"/>
        //[ConservatoryMkdocsSecurityDeny(Capability.CallingAndDelegation)]
        public static unsafe void SetPtrs(byte* breakOnError, delegate* unmanaged<byte> isDebuggerAttached) {
			SetPtrs((nint)breakOnError, (nint)isDebuggerAttached);
        }

        // MAGIC BEHAVIOR: Custom docs generator looks for "Ptrs" ending to disable calling and delegation.
        public static unsafe void InterceptGodotLoggingUsingPtr(delegate* unmanaged<ulong, byte*, int, byte*, int, byte*, int, byte*, int, int, byte, byte, void> callback) {
            InterceptGodotLoggingUsingPtr((nint)callback);
        }

        /// <summary>
        /// A customized implementation of <see cref="GodotObject.Dispose(bool)"/> which prevents a crash caused by how the engine handles
        /// the <see cref="ConservatoryDebugBridge"/> type internally.
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose(bool disposing) {
            GD.PushError("Why did you create an instance of this class? This and more at 10 PM Pacific in Unsolved Mysteries, only on CBS...");
            GD.PushError("If you are reading this, congratulations! Please enjoy your complimentary access violation in the CLR garbage collection routine once the next GC cycle occurs. In the mean time, why don't you consider what you did to get here. :)");
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

    #region Attributes

    /// <summary>
    /// Primarily intended for internal Godot code, this attribute will be used by the custom documentation generator to hide a member even if user preferences wish to show it.
    /// </summary>
    [AttributeUsage(AttributeTargets.All & ~AttributeTargets.Parameter & ~AttributeTargets.GenericParameter, AllowMultiple = false)]
    internal sealed class ConservatoryMkdocsHideAttribute : Attribute { }

#pragma warning disable // i forgor (xmldoc doesn't like the manual use of T:)
    /// <summary>
    /// The same as <see cref="T:Star3D.Security.SecurityDenyAttribute"/> but usable in Godot source code.
    /// </summary>
#pragma warning restore
    [AttributeUsage(AttributeTargets.All & ~AttributeTargets.Parameter & ~AttributeTargets.GenericParameter, AllowMultiple = false)]
    internal sealed class ConservatoryMkdocsSecurityDenyAttribute : Attribute {

        internal Capability Capabilities { get; }

        internal ConservatoryMkdocsSecurityDenyAttribute(Capability capabilities) {
            Capabilities = capabilities;
        }

    }

    #endregion

    file static class PrivateStringNames {

		public static readonly StringName BBCODE_NAME = "bbcode";

	}
    [Flags]
    internal enum Capability : byte {

        None                    = 0,

        CallingAndDelegation    = 1 << 0,

        Reading                 = 1 << 1,

        Writing                 = 1 << 2,

        Adding                  = 1 << 3,

        Removing                = 1 << 4,

        Patching                = 1 << 5,

        ReadWrite               = Reading | Writing,

        AddRemove               = Adding | Removing,

        All                     = 0xFF


    }
}
