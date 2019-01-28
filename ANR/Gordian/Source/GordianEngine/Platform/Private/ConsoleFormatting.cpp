// Gordian by Daniel Luna (2019)

#include "../Public/ConsoleFormatting.h"

#include "GordianEngine/Platform/Public/Platform.h"

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/Logging.h"

using namespace Gordian;

EConsoleColor FConsoleFormatting::DefaultTextColor = EConsoleColor::White;
bool FConsoleFormatting::bHasInitializedFormatting = false;

// Formatting comes from these docs:
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences


/*static*/ bool FConsoleFormatting::InitializeFormatting()
{
	if (bHasInitializedFormatting)
	{
		return true;
	}

#ifdef WINDOWS
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		GE_LOG(LogTemp, Warning, "Failed to get console handle!");
		return false;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		GE_LOG(LogTemp, Warning, "Failed to get console mode!");
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		GE_LOG(LogTemp, Warning, "Failed to set Console Mode!");
		return false;
	}

	bHasInitializedFormatting = true;
#else
	// We don't know our OS, so assume we can't get this to work! (although linux generally supports this)
#endif

	return bHasInitializedFormatting;
}

/*static*/ const char* FConsoleFormatting::GetColorString(EConsoleColor NewTextColor)
{
	

	return "";
}

/*static*/ const char* FConsoleFormatting::GetFormatString(EConsoleColor NewTextColor, TBitSet<EConsoleFormat> FormatOptions)
{
	return "";
}

/*static*/ const char* FConsoleFormatting::GetFullResetString()
{
	return "";
}

/*static*/ void FConsoleFormatting::SetDefaultTextColor(EConsoleColor NewDefaultTextColor)
{
	DefaultTextColor = NewDefaultTextColor;
}

/*static*/ const char* FConsoleFormatting::GetColorCode(const EConsoleColor& Color)
{
	switch (Color)
	{
		case EConsoleColor::Black:
			return "30";
		case EConsoleColor::Red:
			return "31";
		case EConsoleColor::Green:
			return "32";
		case EConsoleColor::Yellow:
			return "33";
		case EConsoleColor::Blue:
			return "34";
		case EConsoleColor::Magenta:
			return "35";
		case EConsoleColor::Cyan:
			return "36";
		case EConsoleColor::White:
			return "37";
		default:
			return GetColorCode(DefaultTextColor);
	}
}

/*static*/ const char* FConsoleFormatting::GetFormatCode(const EConsoleFormat& Format, bool bIsSet)
{
	switch (Format)
	{
		case EConsoleFormat::ResetFormatting:
			return bIsSet ? "0" : "";
		case EConsoleFormat::Bold:
			return bIsSet ? "1" : "21";
		case EConsoleFormat::Underline:
			return bIsSet ? "4" : "24";
		case EConsoleFormat::InverseColors:
			return bIsSet ? "7" : "27";
		default:
			checkNoEntry();
			return "";
	}
}