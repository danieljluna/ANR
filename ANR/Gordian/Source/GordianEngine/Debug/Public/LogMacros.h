// Gordian by Daniel Luna

#pragma once

#include "GordianEngine/Utility/Public/CommonMacros.h"
#include "GordianEngine/Debug/Public/LogCategory.h"

namespace Gordian
{


// ----------------------------------------------------------------
// Log macro definitions
// ----------------------------------------------------------------
#ifdef GE_NO_LOG

	#define LOG(Category, Level, Text) GE_NO_OP(TEXT(Text))

#else // !GE_NO_LOG

		

#endif


// Declares a log category as an extern variable. 
//	Should be placed in a .h file with DEFINE_LOG_CATEGORY_EXTERN in the matching .cpp
#define DECLARE_LOG_CATEGORY_EXTERN(Category,  DefaultRuntimeVerbosity, CompileTimeVerbosity)												\
	extern struct FLogCategory##Category : public FLogCategory<ELogVerbosity::DefaultRuntimeVerbosity, ELogVerbosity::CompileTimeVerbosity>	\
	{																																		\
		FLogCategory##Category() : FLogCategory(TEXT(#Category)) {}																			\
	} Category;

// Defines a log category that was declared as an extern variable. 
//	Should be placed in a .cpp file with DECLARE_LOG_CATEGORY_EXTERN in the matching .h
#define DEFINE_LOG_CATEGORY_EXTERN(Category) FLogCategory##Category Category;


};
