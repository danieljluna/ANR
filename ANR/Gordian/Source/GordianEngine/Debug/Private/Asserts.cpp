// Gordian by Daniel Luna (2019)

#include "../Public/Asserts.h"

#include "../Public/LogOutputManager.h"

using namespace Gordian;

/*static*/ FAssert::EAssertBehavior FAssert::OnEnsureFail(const char* LogText,
													   const char* FileName,
													   int LineNumber)
{
	GLogOutputManager.PrintEnsure(LogText, FileName, LineNumber);

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
	GLogOutputManager.PrintCheck(LogText, FileName, LineNumber);

	return EAssertBehavior::PauseThenAbort;
}
