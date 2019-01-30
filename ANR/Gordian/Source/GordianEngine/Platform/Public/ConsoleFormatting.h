// Gordian by Daniel Luna (2019)

#pragma once

#include "GordianEngine/Bitset/Public/BitSet.h"

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
	// If this bit is set, all formatting is cleared
	ResetFormatting,
	// Achieved by brightening the color
	Bold,
	// Underlines text
	Underline,
	// Swaps foreground and background colors
	InverseColors,
	// Used for BitSet
	MAX_VALUE,
};


// Static helper class that helps color and format console text.
// Simply add the strings returned to your text to format your output in a console.
// While background color support is not to complex, it has yet to be needed and thus is not supported.
class FConsoleFormatting
{
public:

	// Call this before attempting to use any of the other functionality
	// Returns 0 if we were able to successfully initialize
	static int InitializeFormatting();

	static const char* GetColorString(EConsoleColor NewTextColor);

	static const char* GetFormatString(EConsoleColor NewTextColor, TBitSet<EConsoleFormat> FormatOptions);

	static const char* GetFullResetString();

	static void SetDefaultTextColor(EConsoleColor NewDefaultTextColor);

private:

	static const char* GetColorCode(const EConsoleColor& Color);
	static const char* GetFormatCode(const EConsoleFormat& Format, bool bIsSet);

	static EConsoleColor DefaultTextColor;
	static bool bHasInitializedFormatting;
};


};
