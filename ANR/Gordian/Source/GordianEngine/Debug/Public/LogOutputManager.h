// Gordian by Daniel Luna (2019)

#pragma once

#include "SFML/System/NonCopyable.hpp"

#include <cstdio>
#include <cstdarg>

#include "LogVerbosity.h"

namespace Gordian
{


// Manages log / debug output to console-like objects
class FLogOutputManager : sf::NonCopyable
{
public:

	FLogOutputManager();
	~FLogOutputManager();

	void PrintEnsure(const char* EnsureText,
					 const char* FileName,
					 int LineNumber) const;

	void PrintCheck(const char* AssertText,
					const char* FileName,
					int LineNumber) const;

	void PrintLog(const char* Category,
				  ELogVerbosity Verbosity,
				  const char* LogLocation,
				  const char* LogFormat,
				  ...) const;

private:

	void Logf(const char* Format, va_list Args) const;

	const char* DefaultConsoleColor;
	FILE* LogOutputFile;
};

// We use a global variable for macro simplicity.
extern FLogOutputManager GLogOutputManager;


};
