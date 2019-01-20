// Goridan by Daniel Luna

#include "../Public/LogCategory.h"

using namespace Gordian;

FLogCategoryBase::FLogCategoryBase(const char* InName, 
								   const ELogVerbosity& InDefaultRuntimeVerbosity,
								   const ELogVerbosity& InCompileTimeVerbosity)
	: Name(InName)
	, RuntimeVerbosity(InDefaultRuntimeVerbosity)
	, DefaultRuntimeVerbosity(InDefaultRuntimeVerbosity)
	, CompileTimeVerbosity(InCompileTimeVerbosity)
{

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

void FLogCategoryBase::SetRuntimeVerboisty(const ELogVerbosity& InMaxRuntimeVerbosity)
{
	RuntimeVerbosity = InMaxRuntimeVerbosity <= CompileTimeVerbosity ? InMaxRuntimeVerbosity : CompileTimeVerbosity;
}

void FLogCategoryBase::ResetRuntimeVerboisty()
{
	RuntimeVerbosity = DefaultRuntimeVerbosity;
}
