// ANR by Daniel Luna (2019)

#pragma once

#include "TypeResolver.h"
#include "Type_Struct.h"

namespace Gordian
{


// Allows references to classes only if they are a subclass of T.
// Otherwise acts as an OType_Struct
template<typename T, typename std::enable_if<!FDefaultTypeResolver::IsReflected<T>::value, int>::type = 0>
class TSubtypeOf
{
public:

	TSubtypeOf() { _ActualType = nullptr; }
	TSubtypeOf(OType_Struct* InitialType) { _ActualType = InitialType; }

	// Returns true if the comparison type is exactly the same as the actual type
	bool operator==(const OType_Struct* ComparisonType) const { return ComparisonType == _ActualType; }

	// Sets this type. If it is not a propert SubType of T, then this is set to nullptr
	OType_Struct* operator=(const OType_Struct* NewType) 
	{
		_ActualType = NewType->IsChildClassOf(T::GetStaticType()) ? NewType : nullptr;
		return _ActualType;
	}

	const OType_Struct& operator*() const { return *_ActualType; }
	OType_Struct& operator*() { return *_ActualType; }
	operator OType_Struct*() const { return _ActualType; }

private:

	OType_Struct* _ActualType;

};


}; // namespace Gordian
