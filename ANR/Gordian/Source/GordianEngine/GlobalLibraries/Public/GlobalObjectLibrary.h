// Gordian by Daniel Luna

#pragma once

#include <cassert>
#include <type_traits>

#include "GordianEngine/Reflection/Public/Type.h"
#include "GordianEngine/Reflection/Public/Type_Struct.h"

// todo: move GlobalObjectLibrary fns to .inl

namespace Gordian
{

template<typename TargetClass, typename std::enable_if<FDefaultTypeResolver::IsReflected<TargetClass>::value, int>::type = 0>
TargetClass* Cast(OObject* Source)
{
	assert(Source->GetType() != nullptr);
	if (Source->GetType()->IsChildClassOf(TargetClass::GetStaticType()))
	{
		return static_cast<TargetClass*>(Source);
	}

	return nullptr;
}

template<typename TargetClass, typename std::enable_if<FDefaultTypeResolver::IsReflected<TargetClass>::value, int>::type = 0>
const TargetClass* Cast(const OObject* Source)
{
	assert(Source->GetType() != nullptr);
	if (Source->GetType()->IsChildClassOf(TargetClass::GetStaticType()))
	{
		return static_cast<const TargetClass*>(Source);
	}

	return nullptr;
}


class FGlobalObjectLibrary 
{
public:

	// WIP, intended to initialize objects
	template<typename T, typename std::enable_if<
										std::is_base_of<OObject, T>::value, int>::type = 0>
	static T* CreateObject(OObject* OwningObject, 
						   const OType_Struct* ObjectType, 
						   const std::string& ObjectName = "")
	{
		// todo: this should eventually use pooled objects where possible
		OObject* NewObject = new T(ObjectName != "" ? ObjectName : ObjectType->GetName()
								  , OwningObject);

		NewObject->_PrivateType = ObjectType;

		return static_cast<T*>(NewObject);
	}
};

};
