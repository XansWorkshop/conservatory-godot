using System;
using System.Text;

#nullable disable
namespace Godot {

	partial class CharFXTransform {

        /// <summary>
        /// <strong>Appended by The Conservatory's engine fork. This is not native Godot code, and it will not be available in GDScript.</strong>
        /// A <see cref="Rune"/> representing the <see cref="CharFXTransform.GlyphCodepoint"/>. Changing this value affects it and by extension
        /// <see cref="CharFXTransform.GlyphIndex"/> as well.
        /// </summary>
        public Rune Glyph {
			get => new Rune((uint)GlyphCodepoint);
			set => GlyphCodepoint = value.Value;
		}

	}

}
