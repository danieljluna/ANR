// Gordian by Daniel Luna

#pragma once

#include <SFML/Config.hpp>


namespace Gordian
{

// ----------------------------------------------------------------
// Checks if an expression is an array of characters
// ----------------------------------------------------------------
struct FCharArrayTester
{
	// When we pass in a char array of any size, return a an array of two chars.
	template <sf::Uint32 N>
	static char(&Test(const char(&)[N]))[2];
	// When we pass in any other type, return a an array of one chars.
	static char(&Test(...))[1];
};

#define IS_CHAR_ARRAY(expr) (sizeof(FCharArrayTester::Test(expr)) == 2)
// ----------------------------------------------------------------


};	// namespace Gordian
