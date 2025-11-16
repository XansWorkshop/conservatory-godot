/**************************************************************************/
/*  conservatory_debug_bridge.cpp                                         */
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

#include "conservatory_debug_bridge.h"
#include "core/error/error_macros.h"
#include "core/string/print_string.h"

bool *ConservatoryDebugBridge::tc_break_on_err_ptr = nullptr;
bool ConservatoryDebugBridge::has_already_intercepted_logging = false;
bool (*ConservatoryDebugBridge::tc_is_debugger_attached)(void);
void (*ConservatoryDebugBridge::tc_managed_log)(void *p_userdata, const unsigned char *p_message, int p_message_length, const unsigned char *p_error, int p_error_length, const unsigned char *p_function_name, int p_function_name_length, const unsigned char *p_file_name, int p_file_name_length, int p_line, unsigned char p_severity_rating, bool p_is_bbcode);
PrintHandlerList ConservatoryDebugBridge::print_handler;
ErrorHandlerList ConservatoryDebugBridge::error_handler;

void ConservatoryDebugBridge::_bind_methods() {
	ClassDB::bind_static_method("ConservatoryDebugBridge", D_METHOD("should_break_on_engine_error"), &ConservatoryDebugBridge::should_break_on_engine_error);
	ClassDB::bind_static_method("ConservatoryDebugBridge", D_METHOD("set_ptrs", "should_break_on_error", "is_debugger_attached"), &ConservatoryDebugBridge::set_ptrs);
	ClassDB::bind_static_method("ConservatoryDebugBridge", D_METHOD("intercept_godot_logging_using", "managed_error_handler"), &ConservatoryDebugBridge::intercept_godot_logging_using);
	ClassDB::bind_static_method("ConservatoryDebugBridge", D_METHOD("is_engine_error_breaking_possible"), &ConservatoryDebugBridge::is_engine_error_breaking_possible);
	ClassDB::bind_static_method("ConservatoryDebugBridge", D_METHOD("are_engine_memory_metrics_available"), &ConservatoryDebugBridge::are_engine_memory_metrics_available);
}

bool ConservatoryDebugBridge::should_break_on_engine_error() {
	// ConservatoryDebugBridge::tc_break_on_err_ptr			is not null, and
	// ConservatoryDebugBridge::tc_is_debugger_attached		is not null, and
	// ConservatoryDebugBridge::tc_break_on_err_ptr			resolves to true, and
	// ConservatoryDebugBridge::tc_is_debugger_attached		returns true
	return ConservatoryDebugBridge::tc_break_on_err_ptr && tc_is_debugger_attached && *ConservatoryDebugBridge::tc_break_on_err_ptr && tc_is_debugger_attached();
}

void ConservatoryDebugBridge::set_ptrs(const int64_t p_tc_break_on_err_ptr, const int64_t p_tc_is_debugger_attached_ptr) {
	CRASH_COND_MSG(ConservatoryDebugBridge::tc_break_on_err_ptr || ConservatoryDebugBridge::tc_is_debugger_attached, "Security Violation: Something attempted to modify the pointers used for debugger interactions when they were already set.");
	tc_break_on_err_ptr = (bool *)p_tc_break_on_err_ptr;
	tc_is_debugger_attached = (bool (*)(void))p_tc_is_debugger_attached_ptr;
}

void ConservatoryDebugBridge::intercept_godot_logging_using(const int64_t p_managed_error_handler) {
	CRASH_COND_MSG(ConservatoryDebugBridge::tc_break_on_err_ptr || ConservatoryDebugBridge::tc_is_debugger_attached, "Security Violation: Something attempted to modify the log callback when it was already set.");
	ERR_FAIL_COND_MSG(p_managed_error_handler == 0, "Error handler pointer is null.");
	ConservatoryDebugBridge::has_already_intercepted_logging = true;
	ConservatoryDebugBridge::tc_managed_log = (void (*)(void *p_userdata, const unsigned char *p_message, int p_message_length, const unsigned char *p_error, int p_error_length, const unsigned char *p_function_name, int p_function_name_length, const unsigned char *p_file_name, int p_file_name_length, int p_line, unsigned char p_severity_rating, bool p_is_bbcode))((size_t)p_managed_error_handler);

	print_handler.printfunc = handle_print;
	print_handler.next = nullptr;
	print_handler.userdata = nullptr;
	add_print_handler(&print_handler);

	error_handler.errfunc = handle_error;
	error_handler.next = nullptr;
	error_handler.userdata = nullptr;
	add_error_handler(&error_handler);
}

#define STRING_TO_UTF8_WITH_LENGTH(str, ptrvar, sizevar) \
    const unsigned char *ptrvar = nullptr;               \
    int sizevar = 0;                                     \
    PackedByteArray str##utf8____;                       \
    if (str) {                                           \
        str##utf8____ = String(str).to_utf8_buffer();    \
        sizevar = str##utf8____.size();                  \
        ptrvar = str##utf8____.ptr();                    \
    }                                                    \
    ((void)0)

#define STRING_TO_UTF8_WITH_LENGTH_STRING_CLASS(str, ptrvar, sizevar) \
    const unsigned char *ptrvar = nullptr;                            \
    int sizevar = 0;                                                  \
    PackedByteArray str##utf8____;                                    \
	{                                                                 \
        str##utf8____ = str.to_utf8_buffer();                         \
        sizevar = str##utf8____.size();                               \
        ptrvar = str##utf8____.ptr();                                 \
    }                                                                 \
    ((void)0)

void ConservatoryDebugBridge::handle_print(void *p_userdata, const String &p_string, bool p_error, bool p_rich) {
	if (ConservatoryDebugBridge::tc_managed_log) {
		STRING_TO_UTF8_WITH_LENGTH_STRING_CLASS(p_string, message_utf8, message_length);
		ConservatoryDebugBridge::tc_managed_log(p_userdata, message_utf8, message_length, nullptr, 0, nullptr, 0, nullptr, 0, 0, p_error ? 2 : 0, p_rich);
	}
}

void ConservatoryDebugBridge::handle_error(void *p_userdata, const char *function, const char *file, int p_line, const char *err, const char *message, bool p_editor_notify, ErrorHandlerType p_type) {
	if (ConservatoryDebugBridge::tc_managed_log) {
		STRING_TO_UTF8_WITH_LENGTH(function, function_utf8, function_length);
		STRING_TO_UTF8_WITH_LENGTH(file, file_utf8, file_length);
		STRING_TO_UTF8_WITH_LENGTH(err, err_utf8, err_length);
		STRING_TO_UTF8_WITH_LENGTH(message, message_utf8, message_length);
		ConservatoryDebugBridge::tc_managed_log(p_userdata, message_utf8, message_length, err_utf8, err_length, function_utf8, function_length, file_utf8, file_length, p_line, 2, false);
	}
}
#undef STRING_TO_UTF8_WITH_LENGTH

bool ConservatoryDebugBridge::is_engine_error_breaking_possible() {
#if defined(_MSC_VER) && (defined(DEV_ENABLED) || defined(TC_ALLOW_BREAK_ON_ERROR))
	return true;
#else
	return false;
#endif
}

bool ConservatoryDebugBridge::are_engine_memory_metrics_available() {
#if defined(DEBUG_ENABLED) || defined(TC_ALLOW_RELEASE_MEMORY_TRACKING)
	return true;
#else
	return false;
#endif
}
