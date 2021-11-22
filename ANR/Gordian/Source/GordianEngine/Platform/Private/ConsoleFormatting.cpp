// Gordian by Daniel Luna (2019)

#include "../Public/ConsoleFormatting.h"

#include <SFML/Config.hpp>

#include "GordianEngine/Platform/Public/Platform.h"
#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/Logging.h"

using namespace Gordian;

FScopedConsoleFormat FConsoleFormatting::DefaultFormat;
const FScopedConsoleFormat* FConsoleFormatting::CurrentScopeFormat = nullptr;
bool FConsoleFormatting::bHasInitializedFormatting = false;

// Formatting comes from these docs:
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences

namespace
{
	static const sf::Uint16 k_MaxFormatStringLength = 16;
	static const char* k_ConsoleStringPrefix = "\x1b[";
	static const char* k_ConsoleStringSuffix = "m";
}

//////////////////////////////////////////////////////////////////////////////////
/// FScopedConsoleFormat
//////////////////////////////////////////////////////////////////////////////////

FScopedConsoleFormat::FScopedConsoleFormat()
{

}

EConsoleColor FScopedConsoleFormat::GetTextColor() const
{
	if (_TextColorOverride.IsSet())
	{
		return _TextColorOverride.Get();
	}

	const FScopedConsoleFormat& NextMostRelevantFormat = _PreviousScopeFormat != nullptr ? *_PreviousScopeFormat : FConsoleFormatting::GetCurrentConsoleFormat();
	return NextMostRelevantFormat.GetTextColor();
}

bool FScopedConsoleFormat::IsOverridingTextColor() const
{
	return _TextColorOverride.IsSet();
}

void FScopedConsoleFormat::SetTextColor(EConsoleColor NewColor)
{
	_TextColorOverride.Set(NewColor);
}

void FScopedConsoleFormat::RevertTextColor()
{
	// Avoid erasing the value in-case we revert lose scope before settings are applied
	if (_TextColorOverride.IsSet())
	{
		if (_PreviousScopeFormat != nullptr)
		{
			// Avoid erasing the value in-case we lose scope before settings are applied
			_TextColorOverride.Set(_PreviousScopeFormat->GetTextColor());
		}
		else
		{
			// Not currently applied, just clear out the override
			_TextColorOverride.Clear();
		}
	}
}

bool FScopedConsoleFormat::IsFormatOptionActive(EConsoleFormat FormatOption) const
{
	const auto It = _FormatOverrides.find(FormatOption);
	if (It != _FormatOverrides.cend())
	{
		return It->second;
	}

	if (_PreviousScopeFormat == this)
	{
		// Recursive self loop early out for when no override exists
		return false;
	}

	const FScopedConsoleFormat& NextMostRelevantFormat = _PreviousScopeFormat != nullptr ? *_PreviousScopeFormat : FConsoleFormatting::GetCurrentConsoleFormat();
	return NextMostRelevantFormat.IsFormatOptionActive(FormatOption);
}

bool FScopedConsoleFormat::IsOverridingFormatOption(EConsoleFormat FormatOption) const
{
	const auto It = _FormatOverrides.find(FormatOption);
	return It != _FormatOverrides.cend();
}

void FScopedConsoleFormat::SetFormatOption(EConsoleFormat FormatOption, bool bValue)
{
	const auto It = _FormatOverrides.find(FormatOption);
	if (It != _FormatOverrides.end())
	{
		It->second = bValue;
	}
	else
	{
		_FormatOverrides.emplace(FormatOption, bValue);
	}
}

void FScopedConsoleFormat::RevertFormatOption(EConsoleFormat FormatOption)
{
	const auto It = _FormatOverrides.find(FormatOption);
	if (It != _FormatOverrides.end())
	{
		if (_PreviousScopeFormat != nullptr)
		{
			// Avoid erasing the value in-case we revert lose scope before settings are applied
			It->second = _PreviousScopeFormat->IsFormatOptionActive(FormatOption);
		}
		else
		{
			// Just remove the value as we are not currently applied
			_FormatOverrides.erase(It);
		}
	}
}

void FScopedConsoleFormat::Apply()
{
// We only have a console in debug mode
#ifdef GE_DEBUG
	const FScopedConsoleFormat* const CurrentConsoleFormat = &FConsoleFormatting::GetCurrentConsoleFormat();

	// We cannot handle updating a format after it has been overridden by another
	check(_PreviousScopeFormat == nullptr || CurrentConsoleFormat == this);

	// Generate and apply Format string
	char FormatString[k_MaxFormatStringLength] = "";
	int StringLen = FConsoleFormatting::GetFormatString(FormatString, _TextColorOverride, _FormatOverrides);
	check(StringLen < k_MaxFormatStringLength);
	printf(FormatString);

	// Be careful not to set create a loop when activated twice
	if (_PreviousScopeFormat != CurrentConsoleFormat)
	{
		_PreviousScopeFormat = CurrentConsoleFormat;
		FConsoleFormatting::SetCurrentConsoleFormat(*this);
	}

#endif	// GE_DEBUG
}

void FScopedConsoleFormat::RevertAll()
{
// We only have a console in debug mode
#ifdef GE_DEBUG

	if (_PreviousScopeFormat == nullptr)
	{
		// This was never applied, no need to revert
		return;
	}

	// First we revert our format overrides map so we can undo our changes
	for (auto FormatOptionIt : _FormatOverrides)
	{
		FormatOptionIt.second = _PreviousScopeFormat->IsFormatOptionActive(FormatOptionIt.first);
	}

	// Next we fetch our parent color if we overrode it
	if (_TextColorOverride.IsSet())
	{
		_TextColorOverride.Set(_PreviousScopeFormat->GetTextColor());
	}

	// Our overrides are now inverted, so if we apply them it will undo previous applications
	char FormatString[k_MaxFormatStringLength] = "";
	int StringLen = FConsoleFormatting::GetFormatString(FormatString, _TextColorOverride, _FormatOverrides);
	check(StringLen < k_MaxFormatStringLength);
	printf(FormatString);

	// Clear our state
	_FormatOverrides.clear();
	_TextColorOverride.Clear();

	const FScopedConsoleFormat* const CurrentConsoleFormat = &FConsoleFormatting::GetCurrentConsoleFormat();
	check(_PreviousScopeFormat != nullptr && CurrentConsoleFormat == this);

	FConsoleFormatting::SetCurrentConsoleFormat(*_PreviousScopeFormat);
	if (_PreviousScopeFormat != this)
	{
		_PreviousScopeFormat = nullptr;
	}
#endif
}

FScopedConsoleFormat::~FScopedConsoleFormat()
{
	RevertAll();

	check(_PreviousScopeFormat == nullptr || _PreviousScopeFormat == this);
}

//////////////////////////////////////////////////////////////////////////////////
/// FConsoleFormatting
//////////////////////////////////////////////////////////////////////////////////

/*static*/ int FConsoleFormatting::InitializeFormatting()
{
	if (bHasInitializedFormatting)
	{
		return 0;
	}

#ifdef WINDOWS
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
#ifdef GE_DEBUG
		GE_LOG(LogFileIO, Display, "Failed to get a handle to the console!");
		return GetLastError();
#else	// !GE_DEBUG
		GE_LOG(LogFileIO, Display, "No console found while attempting to format.");
		return 0;
#endif	// GE_DEBUG
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		GE_LOG(LogFileIO, Warning, "Failed to get console mode!");
		return GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		GE_LOG(LogFileIO, Warning, "Failed to set Console Mode!");
		return GetLastError();
	}

	bHasInitializedFormatting = true;
	GE_LOG(LogFileIO, Verbose, "Successfully initiated ConsoleFormatting");
#else
	// We don't know our OS, so assume we can't get this to work! (although linux generally supports this)
#endif


	DefaultFormat.SetTextColor(EConsoleColor::White);
	DefaultFormat.Apply();

	CurrentScopeFormat = &DefaultFormat;

	return 0;
}

/*static*/ void FConsoleFormatting::GetColorString(char* OutFormat, EConsoleColor NewTextColor)
{
	check(bHasInitializedFormatting);

	//const char* EnsureColor = "\x1b[31m";
	//char* ColorFormatString;
}

/*static*/ int FConsoleFormatting::GetFormatString(char (&OutBuffer)[16], 
												   const TOptional<EConsoleColor>& NewTextColor, 
												   const std::map<EConsoleFormat, bool>& FormatOptions)
{
	if (!bHasInitializedFormatting)
	{
		InitializeFormatting();
	}

	char FormatSubString[k_MaxFormatStringLength] = "";
	GetFormatSubstring(FormatSubString, FormatOptions);
	return snprintf(OutBuffer,
					k_MaxFormatStringLength,
					"%s%s%s%s%s",
					k_ConsoleStringPrefix,
					NewTextColor.IsSet() ? GetColorCode(NewTextColor.Get()) : "",
					NewTextColor.IsSet() && !FormatOptions.empty() ? ";" : "",
					FormatSubString,
					k_ConsoleStringSuffix);
}

/*static*/ int FConsoleFormatting::GetFullResetString(char (&OutFormat)[16])
{
	check(bHasInitializedFormatting);

	return snprintf(OutFormat,
					k_MaxFormatStringLength,
					"%s%s%s",
					k_ConsoleStringPrefix,
					"0",
					k_ConsoleStringSuffix);
}

/*static*/ const FScopedConsoleFormat& FConsoleFormatting::GetCurrentConsoleFormat()
{
	// Because ScopedConsoleFormats call this on creation to cache a reference, we cannot ensure
	//	formatting has been initialized. Thus, we must be prepared to handle CurrentScopeFormat == nullptr
	return CurrentScopeFormat != nullptr ? *CurrentScopeFormat : DefaultFormat;
}

/*static*/ void FConsoleFormatting::SetCurrentConsoleFormat(const FScopedConsoleFormat& NewScopedFormat)
{
	check(bHasInitializedFormatting);

	CurrentScopeFormat = &NewScopedFormat;
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
			checkNoEntry();
			return "";
	}
}

/*static*/ void FConsoleFormatting::GetFormatSubstring(char (&InOutBuffer)[16], const std::map<EConsoleFormat, bool>& FormatOptions)
{
	check(bHasInitializedFormatting);

	bool bIsFirstFormatCode = true;

	for (const auto FormatOptionIt : FormatOptions)
	{
		if (!bIsFirstFormatCode)
		{
			strcat_s(InOutBuffer, ";");
		}

		strcat_s(InOutBuffer, GetFormatCode(FormatOptionIt.first, FormatOptionIt.second));
		bIsFirstFormatCode = false;
	}
}

/*static*/ const char* FConsoleFormatting::GetFormatCode(const EConsoleFormat& Format, bool bShouldSet)
{
	switch (Format)
	{
		case EConsoleFormat::Bold:
			return bShouldSet ? "1" : "21";
		case EConsoleFormat::Underline:
			return bShouldSet ? "4" : "24";
		case EConsoleFormat::InverseColors:
			return bShouldSet ? "7" : "27";
		default:
			checkNoEntry();
			return "";
	}
}

//////////////////////////////////////////////////////////////////////////////////
