// Gordian by Daniel Luna

#include "GordianEngine/Reflection/Public/Type_Struct.h"

#include <iostream>
#include <string>
#include <cassert>

#include "GordianEngine/Core/Public/Object.h"

using namespace Gordian;

FStructMember OType_Struct::NullMember{};

OType_Struct::OType_Struct()
	: OType{ "", 0 }
	, Members{}
	, ParentClass(nullptr)
	, ClassDepth(0)
	, _InitializationState(EInitializationState::Uninitialized)
{
	bIsStructType = true;
}

void OType_Struct::Initialize(void(*Initialize)(OType_Struct*))
{
	assert(Initialize != nullptr);
	if (_InitializationState == OType_Struct::Uninitialized)
	{
		_InitializationState = OType_Struct::MidInitialization;

		Initialize(this);

		_InitializationState = OType_Struct::FullyInitialized;
	}
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

void OType_Struct::Dump_Internal(const void* Data, int IndentationLevel) const
{
	const OObject* DataAsObject = static_cast<const OObject*>(Data);

	std::string Indent(IndentationLevel, ' ');
	std::clog << Indent << GetName();

	if (ParentClass != nullptr)
	{
		std::clog << " : " << ParentClass->GetName();
	}

	std::clog << " {" << std::hex << "0x" << Data << "}";

	std::clog << std::endl;
	std::clog << Indent << "{" << std::endl;

	std::string MemberIndent(IndentationLevel + k_IndentationWidth, ' ');

	for (const FStructMember& MemberInfo : Members)
	{
		std::clog << MemberIndent << MemberInfo.Name << " = ";
		void* MemberData = (char*)(Data) + MemberInfo.Offset;
		MemberInfo.Type->Dump_Internal(MemberData, IndentationLevel + k_IndentationWidth);
		std::clog << std::endl;
	}

	std::clog << Indent << "}"<< std::endl;
}

RCLASS_MEMBER_BEGIN(OType_Struct)
RCLASS_MEMBER_ADD(ParentClass)
RCLASS_MEMBER_END()
