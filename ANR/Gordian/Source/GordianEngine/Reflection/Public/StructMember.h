// Gordian by Daniel Luna (2019)

#pragma once

namespace Gordian
{


class OType;

// Stores data for a member variable
struct FStructMember
{
	const char* Name;
	size_t Offset;
	const OType* Type;
};


};
