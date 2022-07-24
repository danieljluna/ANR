// Gordian by Daniel Luna (2019)

#include "../Public/Exceptions.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace Gordian;

AssertionFailure::AssertionFailure(const std::string& Message)
	: BaseRuntimeError(Message)
{

}

AssertionFailure::AssertionFailure(const char* Message)
	: BaseRuntimeError(Message)
{

}
