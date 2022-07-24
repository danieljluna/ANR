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

	void SetIsEnabled(bool bInIsEnabled);

private:

	// Prints log to all appropriate streams.
	void Logf(const char* LogFormat, ...) const;

	// Prints log to all appropriate streams.
	void vLogf(const char* LogFormat, va_list Args) const;

	// Prints log to all appropriate streams.
	void FullLogf(const char* LogLocation, const char* LogFormat, ...) const;

	// Prints log to all appropriate streams, but the log location does not get
	// output to the console.
	void vFullLogf(const char* LogLocation, const char* LogFormat, va_list Args) const;

	// Fetches the current time as a file name for the log
	void GenerateLogFilename(char* OutString) const;

	// Fetches the current time as a string for log lines
	void FetchLogTimestamp(char* OutString) const;

	bool bIsEnabled;

	FILE* LogOutputFile;
};

// We use a global variable for macro simplicity.
extern FLogOutputManager GLogOutputManager;


};
