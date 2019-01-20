// Gordian by Daniel Luna

#pragma once

#include <string>

#include "SFML/System/NonCopyable.hpp"

#include "LogVerbosity.h"

namespace Gordian
{


// Houses information about a log category.
// Each category has it's own max verbosity that it will compile / print.
class FLogCategoryBase : public sf::NonCopyable
{
public:

	FLogCategoryBase() = delete;
	FLogCategoryBase(const char* InName, 
					 const ELogVerbosity& InDefaultRuntimeVerbosity, 
					 const ELogVerbosity& InCompileTimeVerbosity);

	// Gets the category name
	const char* GetCategoryName() const;

	// Reads the max runtime verbosity
	const ELogVerbosity& GetRuntimeVerboisty() const;

	// Reads the max compile-time verbosity
	const ELogVerbosity& GetCompileTimeVerbosity() const;

	// Sets the max runtime verbosity
	void SetRuntimeVerboisty(const ELogVerbosity& InMaxRuntimeVerbosity);

	// Resets the max runtime verbosity to its default
	void ResetRuntimeVerboisty();

private:

	// The print-friendly name of the category
	const char* Name;

	// Max verbosity to print as decided at runtime.
	ELogVerbosity RuntimeVerbosity;

	// Default verbosity level to revert to. Can be changed when loading configs.
	ELogVerbosity DefaultRuntimeVerbosity;

	// Max verbosity to print as decided at compile-time.
	//	This cannot be changed without a recompile.
	const ELogVerbosity CompileTimeVerbosity;

};


// Templated version of the base class for use in macros
template<ELogVerbosity InDefaultRuntimeVerbosity, ELogVerbosity InCompileTimeVerbosity>
class FLogCategory : public FLogCategoryBase
{
	static_assert(InDefaultRuntimeVerbosity < ELogVerbosity::Count, "Bad Log Verbosity");
	static_assert(InCompileTimeVerbosity < ELogVerbosity::Count, "Bad Log Verbosity");

public:
	FLogCategory(const char* InName)
		: FLogCategoryBase(InName, InDefaultRuntimeVerbosity, InCompileTimeVerbosity)
	{

	}
};


};
