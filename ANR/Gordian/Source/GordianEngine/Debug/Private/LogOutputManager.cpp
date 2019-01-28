// Gordian by Daniel Luna (2019)

#include "../Public/LogOutputManager.h"

#include "GordianEngine/Platform/Public/ConsoleFormatting.h"

#include <iostream>

using namespace Gordian;

FLogOutputManager Gordian::GLogOutputManager;

FLogOutputManager::FLogOutputManager()
{

}

FLogOutputManager::~FLogOutputManager()
{

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

void FLogOutputManager::PrintLog(const char* LogText,
								 const char* Category,
								 ELogVerbosity Verbosity,
								 const char* FileName, 
								 int LineNumber) const
{
	std::cout << Category << ": " << LogText << " [File: " << FileName << "] [Line: " << LineNumber << "]" << std::endl;
}
