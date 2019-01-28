// Gordian by Daniel Luna (2019)

#pragma once

#include "SFML/System/NonCopyable.hpp"

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

	void PrintLog(const char* LogText,
				  const char* Category,
				  ELogVerbosity Verbosity,
				  const char* FileName,
				  int LineNumber) const;

private:

	const char* DefaultConsoleColor;
};

// We use a global variable for macro simplicity.
extern FLogOutputManager GLogOutputManager;


};
