// Gordian by Daniel Luna

#include "../Public/Asserts.h"

#include <iostream>

using namespace Gordian;

/*static*/ Assert::AssertBehavior Assert::OnEnsureFail(const char* LogText,
													   const char* FileName,
													   int LineNumber)
{
	std::cerr << "Ensure Condition Failed: " << LogText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;

#ifdef GE_DEBUG
	return AssertBehavior::Pause;
#else	// GE_DEBUG
	return AssertBehavior::Ignore;
#endif	// GE_DEBUG
}

/*static*/ Assert::AssertBehavior Assert::OnAssertFail(const char* LogText,
													   const char* FileName,
													   int LineNumber)
{
	std::cerr << "Assert Condition Failed: " << LogText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;

#ifdef GE_SHIPPING
	return AssertBehavior::PauseThenAbort;
#else  // GE_SHIPPING
	return AssertBehavior::PauseThenAbort;
#endif // GE_SHIPPING
}
