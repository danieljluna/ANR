// Gordian by Daniel Luna

#pragma once

#include <SFML/Config.hpp>

#include "GordianEngine/Utility/Public/CommonMacros.h"

namespace Gordian
{


//
// Assert helpers.
//
class FAssert
{
public:
	enum class EAssertBehavior : sf::Uint8
	{
		Ignore,
		Pause,
		PauseThenAbort,
		Abort,
	};

	static EAssertBehavior OnEnsureFail(const char* LogText,
										const char* FileName,
										int LineNumber);

	static EAssertBehavior OnAssertFail(const char* LogText,
										const char* FileName,
										int LineNumber);
};


// Body of switch on EAssertBehavior that handles any breaks / halts
#define __DEBUG_ASSERT_SWITCH()							\
	{													\
		case FAssert::EAssertBehavior::Pause:			\
			_GE_DEBUG_BREAK();							\
			break;										\
		case FAssert::EAssertBehavior::PauseThenAbort:	\
			_GE_DEBUG_BREAK();							\
			_GE_DEBUG_HALT();							\
			break;										\
		case FAssert::EAssertBehavior::Abort:			\
			_GE_DEBUG_HALT();							\
			break;										\
		default:										\
			break;										\
	}

// ----------------------------------------------------------------
// Ensure macro definitions
// 
// Ensures trigger debug breakpoints when compiling the engine in
//	debug config, and are ignored when compiling in release config.
//	Ensures only fire once per execution (Except ensureAlways()).
// ----------------------------------------------------------------
#ifdef GE_SHIPPING

	#define ensure(a) GE_NO_OP(a)
	#define ensureAlways(a) GE_NO_OP(a)
    #define ensureMsgf(a, b) GE_NO_OP(a)

#else	// !GE_SHIPPING

    #define ensure(Condition) do															\
		{																					\
			if (!(Condition))																\
			{																				\
				_GE_RUN_ONCE(switch (FAssert::OnEnsureFail(#Condition, __FILE__, __LINE__)) __DEBUG_ASSERT_SWITCH());	\
			}																				\
        } while (0)

	#define ensureAlways(Condition) do														\
		{																					\
			if (!(Condition))																\
			{																				\
				switch (FAssert::OnEnsureFail(#Condition, __FILE__, __LINE__))				\
					__DEBUG_ASSERT_SWITCH()													\
			}																				\
        } while (0)

	#define ensureMsgf(Condition, LogText) do												\
        {																					\
			if (!(Condition))																\
			{																				\
				_GE_RUN_ONCE(switch (FAssert::OnEnsureFail(#LogText, __FILE__, __LINE__)) __DEBUG_ASSERT_SWITCH());	\
			}																				\
        } while (0)

#endif // GE_SHIPPING


// ----------------------------------------------------------------
// Check macro definitions
//
// Checks will terminate the program upon failure. If you have a debugger attached,
//	they will also pause the program for easier debugging.
// ----------------------------------------------------------------
#define checkNoEntry() do 																\
	{																					\
		switch (FAssert::OnAssertFail("", __FILE__, __LINE__))							\
			__DEBUG_ASSERT_SWITCH()														\
    } while (0)

#define check(Condition) do 																\
	{																						\
        if (!(Condition))																	\
		{																					\
			switch (FAssert::OnAssertFail(#Condition, __FILE__, __LINE__))					\
				__DEBUG_ASSERT_SWITCH()														\
		}																					\
    } while (0)

#define checkMsgf(Condition, LogText) do 													\
    {																						\
        if (!(Condition))																	\
		{																					\
			switch (FAssert::OnAssertFail(#LogText, __FILE__, __LINE__))					\
				__DEBUG_ASSERT_SWITCH()														\
		}																					\
    } while (0)


};	// namespace Gordian
