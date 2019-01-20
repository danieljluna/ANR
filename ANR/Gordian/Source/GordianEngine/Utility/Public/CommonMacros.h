// Gordian by Daniel Luna

#pragma once

#include <mutex>
#include <thread>

#include <SFML/Config.hpp>

namespace Gordian
{

// ----------------------------------------------------------------
// Checks that value is of type during compile time.
//	Is not wrapped in do while, only use inside another macro.
// ----------------------------------------------------------------
#define _GE_COMPILE_TIME_TYPECHECK(value, type) __typeof(value) *__tmp; __tmp = (type *)nullptr;

// ----------------------------------------------------------------
// Simple no-op that compiles to nothing.
//	Is not wrapped in do while, only use inside another macro.
// ----------------------------------------------------------------
#define _GE_NO_OP(x) (void)sizeof(x)

// ----------------------------------------------------------------
// Simple no-op that compiles to nothing.
// ----------------------------------------------------------------
#define GE_NO_OP(x) do { _GE_NO_OP(x); } while(0)

// ----------------------------------------------------------------
// Method used to force a debug break into the code.
//	Is not wrapped in do while, only use inside another macro.
// ----------------------------------------------------------------
#define _GE_DEBUG_BREAK() __debugbreak()

// ----------------------------------------------------------------
// Method used to force the program to shut down.
//	Is not wrapped in do while, only use inside another macro.
// ----------------------------------------------------------------
#define _GE_DEBUG_HALT() terminate()

// ----------------------------------------------------------------
// Allows a code block to only be run once.
//	Is not wrapped in do while, only use inside another macro.
// ----------------------------------------------------------------
#define _GE_RUN_ONCE(code)							\
		static std::once_flag bHasRun;				\
		std::call_once(bHasRun, []() { code; });	\

// ----------------------------------------------------------------
// Converts the input string to unicode or ANSI as appropriate.
// ----------------------------------------------------------------
#ifndef TEXT

	#define TEXT(string) _TEXT(string)

	#ifdef UNICODE
		#define __TEXT(string) L##string
	#else	// ifndef UNICODE
		#define _TEXT(string) string
	#endif	// UNICODE

#endif	// ifndef TEXT
// ----------------------------------------------------------------

};	// namespace Gordian
