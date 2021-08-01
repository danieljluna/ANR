// Gordian by Daniel Luna (2020)

#include "../Public/StringUtility.h"

#include <algorithm>
#include <functional>
#include <locale>

using namespace Gordian;

void FStringUtil::LeftTrim(std::string& InOutTrimTarget, const char* TrimCharacters)
{
	bool bIsSpace = std::isspace(static_cast<unsigned char>(' '), std::locale(""));
// 	std::string::iterator FirstNonTrimCharPos = std::find_if(InOutTrimTarget.begin(), 
// 											InOutTrimTarget.end(),
// 											[](unsigned char c) { return !std::isspace(c); });
// 	InOutTrimTarget.erase(InOutTrimTarget.begin(), FirstNonTrimCharPos);
}

void FStringUtil::RightTrim(sf::String& InOutTrimTarget, const char* TrimCharacters)
{

}

void FStringUtil::Trim(sf::String& InOutTrimTarget, const char* TrimCharacters)
{

}