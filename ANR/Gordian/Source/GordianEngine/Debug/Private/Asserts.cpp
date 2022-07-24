// Gordian by Daniel Luna (2019)

#include "../Public/Asserts.h"

#include "../Public/Exceptions.h"
#include "../Public/LogOutputManager.h"

using namespace Gordian;

delegate<void(const char*, const char*, int)> FAssert::OnAnyFailure;

/*static*/ FAssert::EAssertBehavior FAssert::OnEnsureFail(const char* LogText,
													   const char* FileName,
													   int LineNumber)
{
	GLogOutputManager.PrintEnsure(LogText, FileName, LineNumber);

	if (!OnAnyFailure.isNull())
	{
		OnAnyFailure(LogText, FileName, LineNumber);
	}

#if defined(GE_RELEASE) || defined(GE_TEST) || defined(GE_SKIP_BREAKPOINTS)
	return EAssertBehavior::Ignore;
#else	// !GE_RELEASE && !GE_TEST
	return EAssertBehavior::Pause;
#endif	// GE_RELEASE
}

/*static*/ FAssert::EAssertBehavior FAssert::OnAssertFail(const char* LogText,
													   const char* FileName,
													   int LineNumber)
{
	GLogOutputManager.PrintCheck(LogText, FileName, LineNumber);

	if (!OnAnyFailure.isNull())
	{
		OnAnyFailure(LogText, FileName, LineNumber);
	}

#if GE_RELEASE || GE_TEST || GE_SKIP_BREAKPOINTS
	return EAssertBehavior::Abort;
#else // !GE_RELEASE && !GE_TEST && !GE_SKIP_BREAKPOINTS
	return EAssertBehavior::PauseThenAbort;
#endif // GE_RELEASE || GE_TEST || GE_SKIP_BREAKPOINTS
}
