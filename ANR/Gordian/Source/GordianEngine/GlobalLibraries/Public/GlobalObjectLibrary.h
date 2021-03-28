// Gordian by Daniel Luna (2019)

#pragma once

#include <type_traits>

#include "GordianEngine/Containers/Public/TPrefixTree.h"
#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Reflection/Public/Type.h"
#include "GordianEngine/Reflection/Public/Type_Struct.h"

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
	template<typename T, typename std::enable_if<std::is_base_of<OObject, T>::value, int>::type = 0>
	static T* CreateObject(OObject* OwningObject,
						   const OType_Struct* ObjectType,
						   const std::string& ObjectName = "");

	// Registers a type by name to be searched for later
	static bool RegisterType(const OType* TypeToRegister);

	static const OType* FetchTypeByName(const std::string& TypeName);

private:

	// Stores all types by the names of the type for easy lookup
	static TPrefixTree<const OType*> _AllTypesByName;
};

}; // namespace Gordian

#include "GordianEngine/GlobalLibraries/Private/GlobalObjectLibrary.inl"
