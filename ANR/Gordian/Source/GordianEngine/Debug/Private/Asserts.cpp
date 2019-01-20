// Gordian by Daniel Luna

#include "../Public/Asserts.h"

#include <iostream>

using namespace Gordian;

/*static*/ FAssert::EAssertBehavior FAssert::OnEnsureFail(const char* LogText,
													   const char* FileName,
													   int LineNumber)
{
	std::cerr << "Ensure Condition Failed: " << LogText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;

#ifdef GE_DEBUG
	return EAssertBehavior::Pause;
#else	// !GE_DEBUG
	return EAssertBehavior::Ignore;
#endif	// GE_DEBUG
}

/*static*/ FAssert::EAssertBehavior FAssert::OnAssertFail(const char* LogText,
													   const char* FileName,
													   int LineNumber)
{
	std::cerr << "Assert Condition Failed: " << LogText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;

	return EAssertBehavior::PauseThenAbort;
}
