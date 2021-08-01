// Gordian by Daniel Luna (2019)

#pragma once

#include <mutex>
#include <thread>

#include <SFML/Config.hpp>

#include "GordianEngine/Platform/Public/Platform.h"

namespace Gordian
{

// ----------------------------------------------------------------
// Checks that value is of type during compile time.
//	Is not wrapped in do while, only use inside another macro.
// ----------------------------------------------------------------
#define _GE_COMPILE_TIME_TYPECHECK(value, type) __typeof(value) *__tmp; __tmp = (type *)nullptr;

// ----------------------------------------------------------------
// Simple no-op statement that compiles to nothing.
//	Is not wrapped in do while, only use inside another macro.
// ----------------------------------------------------------------
#define _GE_NO_OP(x) (void)sizeof(x)

// ----------------------------------------------------------------
// Simple no-op. Useful when you need a non-statement no-op.
//	Is not wrapped in do while, only use inside another macro.
// ----------------------------------------------------------------
#define _GE_EMPTY_BLOCK {}

// ----------------------------------------------------------------
// Simple no-op statement that compiles to nothing.
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
		std::call_once(bHasRun, []() { code; });

// ----------------------------------------------------------------
// Converts the input string to unicode or ANSI as appropriate.
// ----------------------------------------------------------------
#ifndef TEXT

	#ifdef UNICODE
		#define __TEXT(string) L##string
	#else	// ifndef UNICODE
		#define __TEXT(string) string
	#endif	// UNICODE

	#define TEXT(string) __TEXT(string)

#endif	// ifndef TEXT
// ----------------------------------------------------------------

// ----------------------------------------------------------------
// Fetches the current function signature as a string
// ----------------------------------------------------------------
#ifndef __FUNCTIONSIG__

	#ifdef WINDOWS
		#define __FUNCTIONSIG__ __FUNCSIG__
	#else
		#define __FUNCTIONSIG__ __PRETTY_FUNCTION__
	#endif	// WINDOWS

#endif	// ifndef __FUNCTIONSIG__
// ----------------------------------------------------------------


// ----------------------------------------------------------------
// Fetches the current function signature as a string
// ----------------------------------------------------------------
#ifndef __STRINGIFY

#define __STRINGIFY(x) #x

#endif	// ifndef __STRINGIFY
// ----------------------------------------------------------------

// ----------------------------------------------------------------
// This double wrapping of the # symbol allows thinks like __LINE__ 
//	to be turned into strings
// ----------------------------------------------------------------
#ifndef __TOSTRING

#define __TOSTRING(x) __STRINGIFY(x)

#endif	// ifndef __TOSTRING
// ----------------------------------------------------------------

};	// namespace Gordian
