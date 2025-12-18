/**************************************************************************/
/*  noise_ext.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                 GODOT ENGINE /// THE CONSERVATORY FORK                 */
/*          https://godotengine.org /// https://xansworkshop.com          */
/**************************************************************************/
/*                     DERIVED FROM GODOT SOURCE CODE                     */
/*                       SEE ORIGINAL LICENSE BELOW                       */
/**************************************************************************/
/* Copyright (c) 2025-present Xan's Workshop.                             */
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "noise.h"

class NoiseExt : public Noise {
	GDCLASS(NoiseExt, Noise);

protected:
	static void _bind_methods();

public:
	GDVIRTUAL1RC_REQUIRED(real_t, _get_noise_1d, real_t);
	GDVIRTUAL1RC_REQUIRED(real_t, _get_noise_2d, Vector2);
	GDVIRTUAL1RC_REQUIRED(real_t, _get_noise_3d, Vector3);

	GDVIRTUAL5RC_REQUIRED(Ref<Image>, _get_image, int, int, bool, bool, bool);
	GDVIRTUAL5RC_REQUIRED(TypedArray<Image>, _get_image_3d, int, int, int, bool, bool);

	GDVIRTUAL6RC_REQUIRED(Ref<Image>, _get_seamless_image, int, int, bool, bool, real_t, bool);
	GDVIRTUAL6RC_REQUIRED(TypedArray<Image>, _get_seamless_image_3d, int, int, int, bool, real_t, bool);

	/*
	virtual Ref<Image> get_image(int p_width, int p_height, bool p_invert = false, bool p_in_3d_space = false, bool p_normalize = true) const;
	virtual TypedArray<Image> get_image_3d(int p_width, int p_height, int p_depth, bool p_invert = false, bool p_normalize = true) const;

	virtual Ref<Image> get_seamless_image(int p_width, int p_height, bool p_invert = false, bool p_in_3d_space = false, real_t p_blend_skirt = 0.1, bool p_normalize = true) const;
	virtual TypedArray<Image> get_seamless_image_3d(int p_width, int p_height, int p_depth, bool p_invert = false, real_t p_blend_skirt = 0.1, bool p_normalize = true) const;

	*/
	real_t get_noise_2dv(Vector2 p_v) const override;
	real_t get_noise_3dv(Vector3 p_v) const override;
	real_t get_noise_1d(real_t p_x) const override;

	_FORCE_INLINE_ real_t get_noise_2d(real_t p_x, real_t p_y) const override {
		return get_noise_2dv(Vector2(p_x, p_y));
	}

	_FORCE_INLINE_ real_t get_noise_3d(real_t p_x, real_t p_y, real_t p_z) const override {
		return get_noise_3dv(Vector3(p_x, p_y, p_z));
	}

	Ref<Image> get_image(int p_width, int p_height, bool p_invert = false, bool p_in_3d_space = false, bool p_normalize = true) const override;
	TypedArray<Image> get_image_3d(int p_width, int p_height, int p_depth, bool p_invert = false, bool p_normalize = true) const override;

	Ref<Image> get_seamless_image(int p_width, int p_height, bool p_invert = false, bool p_in_3d_space = false, real_t p_blend_skirt = 0.1, bool p_normalize = true) const override;
	TypedArray<Image> get_seamless_image_3d(int p_width, int p_height, int p_depth, bool p_invert = false, real_t p_blend_skirt = 0.1, bool p_normalize = true) const override;

};
