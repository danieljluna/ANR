// Gordian by Daniel Luna (2022)

#pragma once

#include <stdexcept>

namespace Gordian
{

using BaseRuntimeError = std::runtime_error;

struct AssertionFailure : public BaseRuntimeError
{
public:
	AssertionFailure(const std::string& Message);
	AssertionFailure(const char* Message);
};


};
