// Gordian by Daniel Luna (2020)

#pragma once

#include <string>

#include "SFML/System/String.hpp"

// String Utility namespace
namespace Gordian
{


struct FStringUtil
{
	FStringUtil() = delete;

	// Trims the left side of the passed string, remove characters until we hit a non-TrimCharacter
	static inline void LeftTrim(std::string& InOutTrimTarget, const char* TrimCharacters);

	// Trims the right side of the passed string, remove characters until we hit a non-TrimCharacter
	static inline void RightTrim(sf::String& InOutTrimTarget, const char* TrimCharacters);

	// Trims both sides of the passed string, remove characters until we hit a non-TrimCharacter
	static inline void Trim(sf::String& InOutTrimTarget, const char* TrimCharacters);

};


};
