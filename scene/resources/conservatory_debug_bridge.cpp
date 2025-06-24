/**************************************************************************/
/*  conservatory_debug.cpp                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                 GODOT ENGINE /// THE CONSERVATORY FORK                 */
/*          https://godotengine.org /// https://xansworkshop.com          */
/**************************************************************************/
/*                     DERIVED FROM GODOT SOURCE CODE                     */
/*                       SEE ORIGINAL LICENSE BELOW                       */
/**************************************************************************/
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

#include "conservatory_debug_bridge.h"

bool *ConservatoryDebugBridge::tc_break_on_err_ptr = nullptr;
bool (*ConservatoryDebugBridge::tc_is_debugger_attached)(void);

void ConservatoryDebugBridge::_bind_methods() {
	ClassDB::bind_static_method("ConservatoryDebugBridge", D_METHOD("should_break_on_engine_error"), &ConservatoryDebugBridge::should_break_on_engine_error);
	ClassDB::bind_static_method("ConservatoryDebugBridge", D_METHOD("set_ptrs", "should_break_on_error", "is_debugger_attached"), &ConservatoryDebugBridge::set_ptrs);
	ClassDB::bind_static_method("ConservatoryDebugBridge", D_METHOD("is_engine_error_breaking_possible"), &ConservatoryDebugBridge::is_engine_error_breaking_possible);
}

bool ConservatoryDebugBridge::should_break_on_engine_error() {
	// ConservatoryDebugBridge::tc_break_on_err_ptr			is not null, and
	// ConservatoryDebugBridge::tc_is_debugger_attached		is not null, and
	// ConservatoryDebugBridge::tc_break_on_err_ptr			resolves to true, and
	// ConservatoryDebugBridge::tc_is_debugger_attached		returns true
	return ConservatoryDebugBridge::tc_break_on_err_ptr && tc_is_debugger_attached && *ConservatoryDebugBridge::tc_break_on_err_ptr && tc_is_debugger_attached();
}

void ConservatoryDebugBridge::set_ptrs(const int64_t p_tc_break_on_err_ptr, const int64_t p_tc_is_debugger_attached_ptr) {
	tc_break_on_err_ptr = (bool *)p_tc_break_on_err_ptr;
	tc_is_debugger_attached = (bool (*)(void))p_tc_is_debugger_attached_ptr;
}

bool ConservatoryDebugBridge::is_engine_error_breaking_possible() {
#if defined(_MSC_VER) && (defined(DEV_ENABLED) || defined(TC_ALLOW_BREAK_ON_ERROR))
	return true;
#else
	return false;
#endif
}
