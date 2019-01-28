// Goridan by Daniel Luna

#include "../Public/LogCategory.h"

#include "../Public/Asserts.h"

using namespace Gordian;

FLogCategoryBase::FLogCategoryBase(const char* InName, 
								   const ELogVerbosity& InDefaultRuntimeVerbosity,
								   const ELogVerbosity& InCompileTimeVerbosity)
	: Name(InName)
	, DefaultRuntimeVerbosity(InDefaultRuntimeVerbosity)
	, CompileTimeVerbosity(InCompileTimeVerbosity)
{
	RuntimeVerbosity = DefaultRuntimeVerbosity <= CompileTimeVerbosity ? DefaultRuntimeVerbosity : CompileTimeVerbosity;
}

const char* FLogCategoryBase::GetCategoryName() const
{
	return Name;
}

const ELogVerbosity& FLogCategoryBase::GetRuntimeVerboisty() const
{
	return RuntimeVerbosity;
}

const ELogVerbosity& FLogCategoryBase::GetCompileTimeVerbosity() const
{
	return CompileTimeVerbosity;
}

bool FLogCategoryBase::IsSuppressed(const ELogVerbosity& LogVerbosity) const
{
	// IsSuppressed should not be called against compile-time verbosity
	ensure(LogVerbosity <= CompileTimeVerbosity);

	return LogVerbosity <= RuntimeVerbosity;
}

void FLogCategoryBase::SetRuntimeVerboisty(const ELogVerbosity& InMaxRuntimeVerbosity)
{
	RuntimeVerbosity = InMaxRuntimeVerbosity <= CompileTimeVerbosity ? InMaxRuntimeVerbosity : CompileTimeVerbosity;
}

void FLogCategoryBase::ResetRuntimeVerboisty()
{
	RuntimeVerbosity = DefaultRuntimeVerbosity;
}
