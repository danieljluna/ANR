// Gordian by Daniel Luna (2019)

#include "GordianEngine/Reflection/Public/Type_Struct.h"

#include <iostream>
#include <string>

#include "GordianEngine/Core/Public/Object.h"
#include "GordianEngine/Debug/Public/Asserts.h"

using namespace Gordian;

FStructMember OType_Struct::NullMember{};

OType_Struct::OType_Struct()
	: OType{ "", 0 }
	, Members{}
	, ParentClass(nullptr)
	, ClassDepth(0)
	, _InitializationState(EInitializationState::Uninitialized)
	, _InitializeFunc(nullptr)
{
	bIsStructType = true;
}

OType_Struct::OType_Struct(void(*Initialize)(OType_Struct*))
	: OType_Struct()
{
	bIsStructType = true;

	check(Initialize != nullptr);

	_InitializeFunc = Initialize;
}

void OType_Struct::EnsureInitialization() const
{
	if (_InitializeFunc != nullptr)
	{
		check(_InitializationState == OType_Struct::EInitializationState::Uninitialized);

		// This is ugly, but makes for cleaner code in other areas
		OType_Struct* NonConstThis = const_cast<OType_Struct*>(this);
		NonConstThis->_InternalInitialize();
	}
}

void OType_Struct::_InternalInitialize()
{
	_InitializationState = EInitializationState::MidInitialization;

	_InitializeFunc(this);

	_InitializationState = EInitializationState::FullyInitialized;

	_InitializeFunc = nullptr;
}

bool OType_Struct::IsChildClassOf(const OType_Struct* PossibleParent) const
{
	const OType_Struct* CurrentClass = this;
	unsigned int PossibleParentDepth = PossibleParent->ClassDepth;

	while (CurrentClass != nullptr
		   && CurrentClass->ClassDepth >= PossibleParentDepth)
	{
		if (CurrentClass == PossibleParent)
		{
			return true;
		}

		CurrentClass = CurrentClass->ParentClass;
	}

	return false;
}

void OType_Struct::Dump_Internal(const void* Data, 
								 size_t MaxDumpDepth, 
								 int IndentationLevel, 
								 bool bShouldPrintName) const
{
	std::string Indent(IndentationLevel, ' ');

	if (bShouldPrintName)
	{
		std::clog << Indent << GetName();
		if (ParentClass != nullptr)
		{
			std::clog << " : " << ParentClass->GetName();
		}
	}

	std::clog << " {" << std::hex << "0x" << Data << "}";

	std::clog << std::endl;
	std::clog << Indent << "{" << std::endl;

	std::string MemberIndent(IndentationLevel + k_IndentationWidth, ' ');

	for (const FStructMember& MemberInfo : Members)
	{
		std::clog << MemberIndent << MemberInfo.Name << " = ";
		void* MemberData = (char*)(Data) + MemberInfo.Offset;
		MemberInfo.Type->Dump_Internal(MemberData, MaxDumpDepth, IndentationLevel + k_IndentationWidth, true);
		std::clog << std::endl;
	}

	std::clog << Indent << "}";
}

RCLASS_INITIALIZE(OType_Struct)
RCLASS_BEGIN_MEMBER_LIST()
RCLASS_MEMBER_ADD(ParentClass)
RCLASS_END_INIT()
