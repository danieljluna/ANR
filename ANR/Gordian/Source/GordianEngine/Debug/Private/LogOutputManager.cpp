// Gordian by Daniel Luna (2019)

#include "../Public/LogOutputManager.h"

#include "GordianEngine/Platform/Public/ConsoleFormatting.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <locale>

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/Logging.h"

#ifdef WINDOWS
	#include <Windows.h>
#endif	// WINDOWS

using namespace Gordian;

namespace
{
	static const int k_MaxLogCharacterLength = 1024;
	static const int k_MaxLogTimestampLength = 29;
}

FLogOutputManager Gordian::GLogOutputManager;

FLogOutputManager::FLogOutputManager()
{
	errno_t ErrorCode;
	ErrorCode = fopen_s(&LogOutputFile, "Log.txt", "a");
	if (ErrorCode != 0)
	{
		GE_LOG(LogFileIO, Fatal, "LogOutput file could not be set up! Error code: %d!", ErrorCode);
	}
}

FLogOutputManager::~FLogOutputManager()
{
	if (LogOutputFile != nullptr)
	{
		std::fclose(LogOutputFile);
	}
}

void FLogOutputManager::FetchLogTimestamp(char* OutString) const
{
	std::time_t Time;
	std::time(&Time);

	check(Time != std::time_t(-1));
	std::tm OutLocalTime;

	errno_t ErrorCode = localtime_s(&OutLocalTime, &Time);
	ensure(ErrorCode == 0);

	size_t FinalStringSize = std::strftime(OutString, k_MaxLogTimestampLength, "[%Y.%m.%d %H:%M:%S %z] ", &OutLocalTime);
	check(FinalStringSize > 0 && FinalStringSize < k_MaxLogTimestampLength);
}

void FLogOutputManager::PrintEnsure(const char* EnsureText, 
									const char* FileName, 
									int LineNumber) const
{
	FScopedConsoleFormat EnsureFormat;
	EnsureFormat.SetTextColor(EConsoleColor::Yellow);
	EnsureFormat.SetFormatOption(EConsoleFormat::Bold, true);
	EnsureFormat.Apply();

	// Move this to use new console formatting fns.
	//const char* EnsureColor = "\x1b[31m";
	//std::cerr << EnsureColor << "Ensure Condition Failed: " << EnsureText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << "\x1b[37m" << std::endl;
	std::cerr << "Ensure Condition Failed: " << EnsureText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;
}

void FLogOutputManager::PrintCheck(const char* AssertText,
								   const char* FileName,
								   int LineNumber) const
{
	FScopedConsoleFormat CheckFormat;
	CheckFormat.SetTextColor(EConsoleColor::Red);
	CheckFormat.SetFormatOption(EConsoleFormat::Bold, true);
	CheckFormat.Apply();

	std::cerr << "Assert Condition Failed: " << AssertText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;
}

void FLogOutputManager::PrintLog(const char* Category,
								 ELogVerbosity Verbosity,
								 const char* LogLocation,
								 const char* LogFormat,
								 ...) const
{
	FScopedConsoleFormat CheckFormat;
	switch (Verbosity)
	{
		case ELogVerbosity::Fatal:
			CheckFormat.SetFormatOption(EConsoleFormat::Underline, true);
			// Falls through to error...
		case ELogVerbosity::Error:
			CheckFormat.SetTextColor(EConsoleColor::Red);
			CheckFormat.SetFormatOption(EConsoleFormat::Bold, true);
			break;
		case ELogVerbosity::Warning:
			CheckFormat.SetTextColor(EConsoleColor::Yellow);
			CheckFormat.SetFormatOption(EConsoleFormat::Bold, true);
			break;
		default:
			break;
	}

	CheckFormat.Apply();

	char FinalLogFormat[k_MaxLogCharacterLength];
	snprintf(FinalLogFormat, k_MaxLogCharacterLength, "%s - %s\n", Category, LogFormat);
	
	va_list Args;
	va_start(Args, LogFormat);
	vFullLogf(LogLocation, FinalLogFormat, Args);
	va_end(Args);
}

void FLogOutputManager::Logf(const char* LogFormat, ...) const
{
	va_list Args;
	va_start(Args, LogFormat);
	vLogf(LogFormat, Args);
	va_end(Args);
}

void FLogOutputManager::vLogf(const char* LogFormat, va_list Args) const
{
	char Timestamp[k_MaxLogTimestampLength];
	FetchLogTimestamp(Timestamp);

	// Output to console, but don't output location
	printf(Timestamp);
	vprintf(LogFormat, Args);

	// Dupe output to log file.
	if (LogOutputFile != nullptr)
	{
		fprintf(LogOutputFile, Timestamp);
		vfprintf(LogOutputFile, LogFormat, Args);
		fflush(LogOutputFile);
	}

#ifdef WINDOWS
	// Dupe output to visual studio output.
	OutputDebugString(Timestamp);

	char VisualStudioOutputBuffer[k_MaxLogCharacterLength] = "";
	_vsnprintf_s(VisualStudioOutputBuffer, sizeof(VisualStudioOutputBuffer), LogFormat, Args);
	OutputDebugString(VisualStudioOutputBuffer);
#endif	// WINDOWS
}

void FLogOutputManager::FullLogf(const char* LogLocation, const char* LogFormat, ...) const
{
	va_list Args;
	va_start(Args, LogFormat);
	vFullLogf(LogLocation, LogFormat, Args);
	va_end(Args);
}

void FLogOutputManager::vFullLogf(const char* LogLocation, const char* LogFormat, va_list Args) const
{
	char Timestamp[k_MaxLogTimestampLength];
	FetchLogTimestamp(Timestamp);

	// Output to console, but don't output location or timestamp
	printf(Timestamp);
	vprintf(LogFormat, Args);

	// Dupe output to log file.
	if (LogOutputFile != nullptr)
	{
		fprintf(LogOutputFile, "%s\n%s", LogLocation, Timestamp);
		vfprintf(LogOutputFile, LogFormat, Args);
		fflush(LogOutputFile);
	}

#ifdef WINDOWS
	// Dupe output to visual studio output.
	OutputDebugString(LogLocation);
	OutputDebugString("\n");
	OutputDebugString(Timestamp);

	char VisualStudioOutputBuffer[k_MaxLogCharacterLength] = "";
	_vsnprintf_s(VisualStudioOutputBuffer, sizeof(VisualStudioOutputBuffer), LogFormat, Args);
	OutputDebugString(VisualStudioOutputBuffer);
#endif	// WINDOWS
}
