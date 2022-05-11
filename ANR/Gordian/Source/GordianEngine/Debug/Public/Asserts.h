// Gordian by Daniel Luna (2019)

#pragma once

#include <SFML/Config.hpp>
#include "GordianEngine/Delegates/Delegate.h"

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

	static delegate<void(const char*, const char*, int)> OnAnyFailure;
};


// Body of switch on EAssertBehavior that handles any breaks / halts
#define __DEBUG_ASSERT_SWITCH()							\
	{													\
		case FAssert::EAssertBehavior::Ignore:			\
			break;										\
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
// Ensures are intended to verify an expected boolean expression
//	that is not fatal when it fails. When the expression is false,
//	the ensure will break in non-shipping builds - although the
//	expression is always evaluated.
// Ensures only fire once per execution (Except ensureAlways()).
// ----------------------------------------------------------------
#ifdef GE_SHIPPING

	#define ensure(Condition)						(!!(Condition))
	#define ensureAlways()							(!!(Condition))
    #define ensureMsgf(Condition, Message)			(!!(Condition))
	#define ensureAlwaysMsgf(Condition, Message)	(!!(Condition))

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

	#define ensureAlwaysMsgf(Condition, LogText) do											\
		{																					\
			if (!(Condition))																\
			{																				\
				switch (FAssert::OnEnsureFail(#LogText, __FILE__, __LINE__))				\
					__DEBUG_ASSERT_SWITCH()													\
			}																				\
        } while (0)

#endif // GE_SHIPPING


// ----------------------------------------------------------------
// Check macro definitions
//
// Checks are intended to catch fatal state. If the boolean statement
//	is false, the assert will break (if possible) and then terminate
//	the program. If a check is inactive, it will NOT evaluate the statement
//	inside of it.
// ----------------------------------------------------------------
#ifdef GE_SHIPPING

	#define checkNoEntry() 
	#define check(Condition) 
	#define checkMsgf(Condition, LogText) 

#else // !GE_SHIPPING

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
#endif	// #GE_SHIPPING


};	// namespace Gordian
