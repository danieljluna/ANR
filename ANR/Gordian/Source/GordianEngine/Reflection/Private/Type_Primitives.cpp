// Gordian by Daniel Luna (2019)

#pragma once

#include <iostream>

#include "../Public/Type.h"
#include "../Public/TypeResolver.h"

namespace Gordian
{


//////////////////////////////////////////////////////////////
// Type Info for bools
//////////////////////////////////////////////////////////////

class OType_Bool : public OType
{
public:
	OType_Bool() : OType("bool", sizeof(bool)) {}

protected:
	virtual void Dump_Internal(const void* Data, int) const override
	{
		std::clog << std::boolalpha << "bool {" << *(static_cast<const bool*>(Data)) << "}";
	}

};


template<>
OType* GetPrimitiveDescriptor<bool>()
{
	static OType_Bool TypeDescription;
	return &TypeDescription;
}


//////////////////////////////////////////////////////////////
// Type Info for integers
//////////////////////////////////////////////////////////////

class OType_Int : public OType
{
public:
	OType_Int() : OType{"int", sizeof(int)} {}

protected:
	virtual void Dump_Internal(const void* Data, int) const override
	{
		std::clog << std::dec << std::noshowbase << "int {" << *(static_cast<const int*>(Data)) << "}";
	}
};

template<>
OType* GetPrimitiveDescriptor<int>()
{
	static OType_Int TypeDescription;
	return &TypeDescription;
}


//////////////////////////////////////////////////////////////
// Type info for std::strings
//////////////////////////////////////////////////////////////

class OType_StdString : public OType
{
public:
	OType_StdString() : OType{ "string", sizeof(std::string) } {}

protected:
	virtual void Dump_Internal(const void* Data, int) const override
	{
		std::clog << "string {\"" << *(static_cast<const std::string*>(Data)) << "\"}";
	}
};

template<>
OType* GetPrimitiveDescriptor<std::string>()
{
	static OType_StdString TypeDescription;
	return &TypeDescription;
}


};	// namespace Gordian


