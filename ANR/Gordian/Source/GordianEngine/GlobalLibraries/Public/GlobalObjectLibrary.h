// Gordian by Daniel Luna (2019)

#pragma once

#include <type_traits>

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Reflection/Public/Type.h"
#include "GordianEngine/Reflection/Public/Type_Struct.h"

// todo: move GlobalObjectLibrary fns to .inl

namespace Gordian
{

template<typename TargetClass, typename std::enable_if<FDefaultTypeResolver::IsReflected<TargetClass>::value, int>::type = 0>
TargetClass* Cast(OObject* Source);

template<typename TargetClass, typename std::enable_if<FDefaultTypeResolver::IsReflected<TargetClass>::value, int>::type = 0>
const TargetClass* Cast(const OObject* Source);


class FGlobalObjectLibrary 
{
public:

	// WIP, intended to initialize objects
	template<typename T, typename std::enable_if<
		std::is_base_of<OObject, T>::value, int>::type = 0>
	static T* CreateObject(OObject* OwningObject,
						   const OType_Struct* ObjectType,
						   const std::string& ObjectName = "");
};

};

#include "GordianEngine/GlobalLibraries/Private/GlobalObjectLibrary.inl"
