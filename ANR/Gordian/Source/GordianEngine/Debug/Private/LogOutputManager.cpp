// Gordian by Daniel Luna (2019)

#include "../Public/LogOutputManager.h"

#include "GordianEngine/Platform/Public/ConsoleFormatting.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/Logging.h"

#ifdef WINDOWS
	#include <Windows.h>
#endif	// WINDOWS

using namespace Gordian;

namespace
{
	static const int k_MaxLogCharacterLength = 1024;
}

FLogOutputManager Gordian::GLogOutputManager;

FLogOutputManager::FLogOutputManager()
{
	errno_t ErrorCode;
	ErrorCode = fopen_s(&LogOutputFile, "Log.txt", "w");
	if (ErrorCode != 0)
	{
		GE_LOG(LogTemp, Fatal, "LogOutput could not be set up! Error code: %d!", ErrorCode);
	}
}

FLogOutputManager::~FLogOutputManager()
{
	std::fclose(LogOutputFile);
}

void FLogOutputManager::PrintEnsure(const char* EnsureText, 
									const char* FileName, 
									int LineNumber) const
{
	// Move this to use new console formatting fns.
	//const char* EnsureColor = "\x1b[31m";
	//std::cerr << EnsureColor << "Ensure Condition Failed: " << EnsureText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << "\x1b[37m" << std::endl;
	std::cerr << "Ensure Condition Failed: " << EnsureText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;
}

void FLogOutputManager::PrintCheck(const char* AssertText,
								   const char* FileName,
								   int LineNumber) const
{
	std::cerr << "Assert Condition Failed: " << AssertText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;
}

void FLogOutputManager::PrintLog(const char* Category,
								 ELogVerbosity Verbosity,
								 const char* LogLocation,
								 const char* LogFormat,
								 ...) const
{
	char FinalLogFormat[k_MaxLogCharacterLength];
	strcpy_s(FinalLogFormat, Category);
	strcat_s(FinalLogFormat, " - ");
	strcat_s(FinalLogFormat, LogFormat);
	strcat_s(FinalLogFormat, "\n");
	strcat_s(FinalLogFormat, LogLocation);
	strcat_s(FinalLogFormat, "\n");
	
	va_list Args;
	va_start(Args, LogFormat);
	Logf(FinalLogFormat, Args);
	va_end(Args);
}

void FLogOutputManager::Logf(const char* Format, va_list Args) const
{
	// Output to console
	vprintf(Format, Args);

	// Dupe output to log file.
	check(LogOutputFile != nullptr);
	vfprintf(LogOutputFile, Format, Args);

#ifdef WINDOWS
	// Dupe output to visual studio output.
	char VisualStudioOutputBuffer[k_MaxLogCharacterLength] = "";
	_vsnprintf_s(VisualStudioOutputBuffer, sizeof(VisualStudioOutputBuffer), Format, Args);

	OutputDebugString(VisualStudioOutputBuffer);
#endif	// WINDOWS
}
