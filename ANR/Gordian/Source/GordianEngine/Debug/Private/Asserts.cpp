// Gordian by Daniel Luna (2019)

#include "../Public/Asserts.h"

#include "../Public/LogOutputManager.h"

using namespace Gordian;

delegate<void(const char*, const char*, int LineNumber)> FAssert::OnAnyFailure;

/*static*/ FAssert::EAssertBehavior FAssert::OnEnsureFail(const char* LogText,
													   const char* FileName,
													   int LineNumber)
{
	GLogOutputManager.PrintEnsure(LogText, FileName, LineNumber);

	if (!OnAnyFailure.isNull())
	{
		OnAnyFailure(LogText, FileName, LineNumber);
	}

#ifdef GE_RELEASE
	return EAssertBehavior::Ignore;	// never pause in release!
#else	// !GE_RELEASE
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

#ifdef GE_RELEASE
	return EAssertBehavior::Abort;	// never pause in release!
#else // !GE_RELEASE
	return EAssertBehavior::PauseThenAbort;
#endif // GE_RELEASE
}
