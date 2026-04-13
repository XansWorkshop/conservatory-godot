/**************************************************************************/
/*  conservatory_debug_bridge.h                                           */
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

#include "core/object/object.h"
#include "core/object/class_db.h"

class ConservatoryDebugBridge : public Object {
	GDCLASS(ConservatoryDebugBridge, Object);

	static void handle_print(void *p_userdata, const String &p_string, bool p_error, bool p_rich);
	static void handle_error(void *p_userdata, const char *function, const char *file, int p_line, const char *err, const char *msg, bool p_editor_notify, ErrorHandlerType p_type);

protected:
	static void _bind_methods();

public:
	static uint8_t *tc_break_on_err_ptr;
	static bool has_already_intercepted_logging;
	static PrintHandlerList print_handler;
	static ErrorHandlerList error_handler;

	// Parameters: None
	static uint8_t (*tc_is_debugger_attached)(void);

	// The error and message parameters are different
	static void (*tc_managed_log)(const uint64_t p_userdata, const uint8_t *p_message, const int32_t p_message_length, const uint8_t *p_error, const int32_t p_error_length, const uint8_t *p_function_name, const int32_t p_function_name_length, const uint8_t *p_file_name, const int32_t p_file_name_length, const int32_t p_line, const uint8_t p_severity_rating, const uint8_t p_is_bbcode);
	static void (*tc_debugbreak)(void);

	static bool should_break_on_engine_error();
	// Future Xan: "ptr" and "ptrs" are magic suffixes (as is the "privileged" prefix) which tells CasCore to lock it out.
	static void set_ptrs(const int64_t p_tc_break_on_err_ptr, const int64_t p_tc_is_debugger_attached_ptrr);
	static void intercept_godot_logging_using_ptr(const int64_t p_managed_error_handler);
	static bool is_engine_error_breaking_possible();
	static bool are_engine_memory_metrics_available();
	
};
