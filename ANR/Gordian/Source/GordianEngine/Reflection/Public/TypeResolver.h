// Gordian by Daniel Luna (2019)

#pragma once

#include <iostream>
#include <type_traits>

#include "ReflectionMacros.h"

namespace Gordian
{

class OType;

// Handles primitive types
template<typename T>
typename std::enable_if<!std::is_pointer<T>::value, OType*>::type GetPrimitiveDescriptor();


// Deduces FTypes
struct FDefaultTypeResolver
{
	template<typename T> static char func(decltype(T::__RSTRUCT_MEMBER_STATIC));
	template<typename T> static int func(...);
	template<typename T>
	struct IsReflected
	{
		enum
		{
			value = (sizeof(func<T>(nullptr)) == sizeof(char))
		};
	};

	// Handles user reflected types
	template<typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
	static const OType* Get()
	{
		return &T::__RSTRUCT_MEMBER_STATIC;
	}

	// Handles all other cases
	template<typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
	static const OType* Get()
	{
		return GetPrimitiveDescriptor<T>();
	}
};


// Primary template for deducing FTypes
template<typename T>
struct FTypeResolver
{
	static const OType* Get()
	{
		return FDefaultTypeResolver::Get<T>();
	}
};



};	// namespace Gordian
