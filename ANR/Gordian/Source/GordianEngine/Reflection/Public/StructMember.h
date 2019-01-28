// Gordian by Daniel Luna (2019)

#pragma once

namespace Gordian
{


// Stores data for a member variable
struct FStructMember
{
	const char* Name;
	size_t Offset;
	const OType* Type;
};


// Stores data for a member function
struct FStructFunction
{
	const char* Name;
	size_t Offset;
	const OType* ReturnType;
	std::vector<const OType*> ParameterTypes;
};


};
