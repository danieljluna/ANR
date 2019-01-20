// Gordian by Daniel Luna

#pragma once

#include <mutex>
#include <thread>

// ----------------------------------------------------------------
// Checks that value is of type during compile time
// ----------------------------------------------------------------
#define __GE_COMPILE_TIME_TYPECHECK(value, type) do { __typeof(value) *__tmp; __tmp = (type *)nullptr; } while(0)

// ----------------------------------------------------------------
// Simple no-op that compiles to nothing
// ----------------------------------------------------------------
#define __GE_NO_OP(x) do { (void)sizeof(x); } while(0)

// ----------------------------------------------------------------
// Method used to force a debug break into the code
// ----------------------------------------------------------------
#define __GE_DEBUG_BREAK() do { __debugbreak(); } while(0)

// ----------------------------------------------------------------
// Method used to force the program to shut down
// ----------------------------------------------------------------
#define __GE_DEBUG_HALT() do { terminate(); } while(0)

// ----------------------------------------------------------------
// Allows a code block to only be run once
// ----------------------------------------------------------------
#define __GE_RUN_ONCE(code) do						\
	{												\
		static std::once_flag bHasRun;				\
		std::call_once(bHasRun, []() { code; });	\
	} while(0)
