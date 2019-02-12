// Gordian by Daniel Luna (2019)

#pragma once

#include <SFML/Config.hpp>

#include "GordianEngine/Utility/Public/CommonMacros.h"
#include "AssertMacros.h"
#include "LogCategory.h"
#include "LogOutputManager.h"
#include "LogVerbosity.h"

namespace Gordian
{


// ----------------------------------------------------------------
// Log macro definitions
// ----------------------------------------------------------------
#ifdef GE_FATAL_LOGS_ONLY

	// Returns true if the given log category is active at the given verbosity.
	// With GE_NO_LOG set this is only true for fatal logs
	#define GE_IS_LOG_ACTIVE(Category, Verbosity) (ELogVerbosity::Verbosity == ELogVerbosity::Fatal)

	// Outputs Text to the default log handler in Category at Verbosity.
	#define GE_LOG(Category, Verbosity, Text)									\
		if (GE_IS_LOG_ACTIVE(Category))											\
		{																		\
																				\
		}																		\

#else // !GE_FATAL_LOGS_ONLY

	namespace PrivateLogHelpers
	{
		// If our category has compile time verbosity below the passed velocity, we can decide 
		//	at compile-time that this is suppressed.
		template<ELogVerbosity Verbosity, typename Category>
		typename std::enable_if<(Category::CompileTimeVerbosity > Verbosity), bool>::type
			IsLogActive(const Category& LogCategory)
		{
			return !LogCategory.IsSuppressed(Verbosity);
		}

		// If our category has compile time verbosity above the passed velocity, we can decide 
		//	at compile-time that this is suppressed.
		template<ELogVerbosity Verbosity, typename Category>
		typename std::enable_if<(Category::CompileTimeVerbosity <= Verbosity), bool>::type
			IsLogActive(const Category& LogCategory)
		{
			return false;
		}
	};

	// Returns true if the given log category is active at the given verbosity.
	// Hides the templated helper in a macro for simplicity.
	#define GE_IS_LOG_ACTIVE(Category, Verbosity)	(Gordian::PrivateLogHelpers::IsLogActive<ELogVerbosity::Verbosity>(Category))

	#define __GE_LOG_LOCATION__(File, Function, LineNumber) File "(" LineNumber "): " Function

	// Outputs Text to the default log handler in Category at Verbosity.
	#define GE_LOG(Category, Verbosity, LogFormat, ...)															\
	do {																										\
		static_assert(IS_CHAR_ARRAY(LogFormat), "Expects Text to be a char array!");							\
		static_assert(Verbosity >= 0 && Verbosity < ELogVerbosity::Count, "Expects legal verbosity!");			\
		if (GE_IS_LOG_ACTIVE(Category, Verbosity))																\
		{																										\
			GLogOutputManager.PrintLog(Category.GetCategoryName(),												\
									   ELogVerbosity::Verbosity,												\
									   __GE_LOG_LOCATION__(__FILE__, __FUNCTIONSIG__, __TOSTRING(__LINE__)),	\
									   LogFormat,																\
									   ##__VA_ARGS__);															\
		}																										\
	} while (0);

#endif



// Declares a log category as an extern variable.
//	Should be placed in a .h file with DEFINE_LOG_CATEGORY_EXTERN in the matching .cpp
#define DECLARE_LOG_CATEGORY_EXTERN(Category,  DefaultRuntimeVerbosity, CompileTimeVerbosity)			\
	extern class FLogCategory##Category : public Gordian::FLogCategory<Gordian::ELogVerbosity::DefaultRuntimeVerbosity, Gordian::ELogVerbosity::CompileTimeVerbosity>	\
	{																									\
	public:																								\
		FLogCategory##Category() : FLogCategory(TEXT(#Category)) {}										\
	} Category;

// Defines a log category that was declared as an extern variable. 
//	Should be placed in a .cpp file with DECLARE_LOG_CATEGORY_EXTERN in the matching .h
#define DEFINE_LOG_CATEGORY_EXTERN(Category) FLogCategory##Category Category;


};
