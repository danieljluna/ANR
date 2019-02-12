// Gordian by Daniel Luna (2019)

#pragma once

#include <map>

#include <SFML/System/NonCopyable.hpp>

#include "GordianEngine/Bitset/Public/BitSet.h"
#include "GordianEngine/Containers/Public/TOptional.h"

namespace Gordian
{


enum class EConsoleColor
{
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
};

enum class EConsoleFormat
{
	// Achieved by brightening the color
	Bold = 0,
	// Underlines text
	Underline,
	// Swaps foreground and background colors
	InverseColors,
	// Used for BitSet
	MAX_VALUE,
};


// Used to change the console format for the local scope only.
// When a FScopedConsoleFormat object leaves scope / is destroyed, it automatically
//	reverts to the last scope's format.
class FScopedConsoleFormat : public sf::NonCopyable
{
public:
	FScopedConsoleFormat();
	~FScopedConsoleFormat();

	// Returns the console text color if this format were / is applied
	// This is a bit slow, avoid where possible.
	EConsoleColor GetTextColor() const;
	// Returns true iff this object would / has overridden text color.
	bool IsOverridingTextColor() const;
	// Sets the text color for this scope.
	void SetTextColor(EConsoleColor NewColor);
	// Reverts the text color to the previous scope's setting
	void RevertTextColor();

	// Returns whether the format option would / is applied.
	// This is a bit slow, avoid where possible.
	bool IsFormatOptionActive(EConsoleFormat FormatOption) const;
	// Returns true iff this object would / has overridden the format option
	bool IsOverridingFormatOption(EConsoleFormat FormatOption) const;
	// Sets the format option for this scope.
	void SetFormatOption(EConsoleFormat FormatOption, bool bValue);
	// Reverts a single format option to the previous scope's setting
	void RevertFormatOption(EConsoleFormat FormatOption);

	// Call this to apply the scoped format to the console.
	void Apply();

	// Reverts all console format / text color changes to the previous scope's settings.
	void RevertAll();

private:

	// Optional text color override for this scope
	TOptional<EConsoleColor> _TextColorOverride;
	// Optional format overrides for this scope
	std::map<EConsoleFormat, bool> _FormatOverrides;
	// Pointer to the previous context. If this is nullptr, this format has not been applied.
	const FScopedConsoleFormat* _PreviousScopeFormat;
};


// Static helper class that helps color and format console text.
// Simply add the strings returned to your text to format your output in a console.
// While background color support is not too complex, it has yet to be needed and thus is not supported.
class FConsoleFormatting
{
public:

	// Call this before attempting to use any of the other functionality
	// Returns 0 if we were able to successfully initialize
	static int InitializeFormatting();

	// Fills the InOutBuffer with a format string that sets text foreground color
	static void GetColorString(char* OutFormat, EConsoleColor NewTextColor);

	// Fills the InOutBuffer with a format string that sets text foreground color and applies the given format options
	// Returns the length of the final formatted string. If the passed in char[] is shorter than this, it will get truncated.
	static int GetFormatString(char (&OutBuffer)[16], const TOptional<EConsoleColor>& NewTextColor, const std::map<EConsoleFormat, bool>& FormatOptions);

	// Fills the OutBuffer with a format string that clears all formatting options back to the 
	// Returns the length of the final formatted string. If the passed in char[] is shorter than this, it will get truncated.
	static int GetFullResetString(char (&OutFormat)[16]);

	// Returns the format object for the current scope
	static const FScopedConsoleFormat& GetCurrentConsoleFormat();

	// Sets the format object for the current scope
	static void SetCurrentConsoleFormat(const FScopedConsoleFormat& NewScopedFormat);

private:

	// Returns the param value for a given foreground color code
	static const char* GetColorCode(const EConsoleColor& Color);
	// Returns the param value for a given format code
	static const char* GetFormatCode(const EConsoleFormat& Format, bool bShouldSet);

	// Returns the substring that denotes appropriate parameters for a list of format options
	// This substring is built of Format codes separated by ';'
	static void GetFormatSubstring(char (&InOutBuffer)[16], const std::map<EConsoleFormat, bool>& FormatOptions);

	// Stores the default console text color / format
	static FScopedConsoleFormat DefaultFormat;
	// Reference to the current scope's format
	static const FScopedConsoleFormat* CurrentScopeFormat;
	// Denotes whether or not console formatting has successfully initialized
	static bool bHasInitializedFormatting;
};


};
