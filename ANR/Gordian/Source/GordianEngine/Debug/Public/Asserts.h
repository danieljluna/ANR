// Gordian by Daniel Luna

#pragma once

#include <mutex>
#include <thread>

namespace Gordian
{

#define __DEBUG_BREAK() __debugbreak()
#define __DEBUG_HALT() terminate()

#define __DEBUG_ASSERT_SWITCH()			\
	{									\
		case Assert::Pause:				\
			__DEBUG_BREAK();			\
			break;						\
		case Assert::PauseThenAbort:	\
			__DEBUG_BREAK();			\
			__DEBUG_HALT();				\
			break;						\
		case Assert::Abort:				\
			__DEBUG_HALT();				\
			break;						\
		default:						\
			break;						\
	}

// ----------------------------------------------------------------
// Ensure macro definitions
// 
// Ensures trigger debug breakpoints when compiling the engine in
//	debug config, and are ignored when compiling in release config.
//	Ensures only fire once per execution (Except ensureAlways()).
// ----------------------------------------------------------------
#ifdef GE_SHIPPING

	#define ensure(a) do { (void)sizeof(a); } while(0)
	#define ensureAlways(a) do { (void)sizeof(a); } while(0)
    #define ensureMsgf(a, b) do { (void)sizeof(a); } while(0)

#else	// GE_SHIPPING

    #define ensure(Condition) do															\
		{																					\
			static std::once_flag bHasRun;													\
			std::call_once(bHasRun, []()													\
			{																				\
				switch (Assert::OnEnsureFail(#Condition, __FILE__, __LINE__))				\
					__DEBUG_ASSERT_SWITCH()													\
			});																				\
        } while (0)

	#define ensureAlways(Condition) do														\
		{																					\
			switch (Assert::OnEnsureFail(#Condition, __FILE__, __LINE__))					\
				__DEBUG_ASSERT_SWITCH()														\
        } while (0)

	#define ensureMsgf(Condition, LogText) do												\
        {																					\
			static std::once_flag bHasRun;													\
			std::call_once(bHasRun, []()													\
			{																				\
				switch (Assert::OnEnsureFail(#LogText, __FILE__, __LINE__))					\
					__DEBUG_ASSERT_SWITCH()													\
			});																				\
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
		switch (Assert::OnAssertFail("", __FILE__, __LINE__))							\
			__DEBUG_ASSERT_SWITCH()														\
    } while (0)

#define check(Condition) do 																\
	{																						\
        if (!(Condition))																	\
		{																					\
			switch (Assert::OnAssertFail(#Condition, __FILE__, __LINE__))					\
				__DEBUG_ASSERT_SWITCH()														\
		}																					\
    } while (0)

#define checkMsgf(Condition, LogText) do 													\
    {																						\
        if (!(Condition))																	\
		{																					\
			switch (Assert::OnAssertFail(#LogText, __FILE__, __LINE__))						\
				__DEBUG_ASSERT_SWITCH()														\
		}																					\
    } while (0)


//
// Assert helpers.
//
class Assert
{
public:
	enum AssertBehavior
	{
		Ignore,
		Pause,
		PauseThenAbort,
		Abort,
	};

	static AssertBehavior OnEnsureFail(const char* LogText,
										const char* FileName,
										int LineNumber);

	static AssertBehavior OnAssertFail(const char* LogText,
										const char* FileName,
										int LineNumber);
};

}
