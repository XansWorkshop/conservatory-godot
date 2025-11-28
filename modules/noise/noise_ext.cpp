/**************************************************************************/
/*  noise_ext.cpp                                                         */
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

#include "noise_ext.h"

real_t NoiseExt::get_noise_1d(real_t p_x) const {
	real_t result;
	GDVIRTUAL_CALL(_get_noise_1d, p_x, result);
	return result;
}

real_t NoiseExt::get_noise_2dv(Vector2 p_v) const {
	real_t result;
	GDVIRTUAL_CALL(_get_noise_2d, p_v, result);
	return result;
}

real_t NoiseExt::get_noise_3dv(Vector3 p_v) const {
	real_t result;
	GDVIRTUAL_CALL(_get_noise_3d, p_v, result);
	return result;
}

void NoiseExt::_bind_methods() {
	GDVIRTUAL_BIND(_get_noise_1d, "scalar");
	GDVIRTUAL_BIND(_get_noise_2d, "vector");
	GDVIRTUAL_BIND(_get_noise_3d, "vector");
}
