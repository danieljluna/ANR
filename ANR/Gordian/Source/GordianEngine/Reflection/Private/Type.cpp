// Gordian by Daniel Luna (2019)

#include "GordianEngine/Reflection/Public/Type.h"
#include "GordianEngine/Core/Public/Gordian.h"

#include "GordianEngine/Core/Public/Object.h"

using namespace Gordian;

namespace
{
	const int k_IndentationWidth = 3;
}

OType::OType(const std::string& Name, size_t Size) 
	: OObject(Name, nullptr)
	, bIsStructType(false)
	, Size{ Size }
{
}

OType::~OType()
{

}

std::string OType::GetFullName() const
{
	return GetName();
}

void OType::Dump(const void* Data, size_t MaxDumpDepth) const
{
	// Preserve existing format flags
	std::ios_base::fmtflags PreviousFormat(std::clog.flags());

	Dump_Internal(Data, MaxDumpDepth, 0, true);

	// Load back up old formatting
	std::clog.flags(PreviousFormat);
}

void OType_Pointer::Dump_Internal(const void* Data, size_t MaxDumpDepth, int IndentationLevel, bool bShouldPrintName) const
{
	const void * const * PtrAtData = static_cast<const void * const *>(Data);
	if (*PtrAtData == nullptr)
	{
		std::clog << " {nullptr}";
		return;
	}

	const OType* ObjectDumpInterpretation = ItemType;
	if (ItemType->IsChildClassOf(OObject::GetStaticType()))
	{
		// Attempt to dump as actual object type
		const OObject* PtrAtDataAsObject = reinterpret_cast<const OObject*>(*PtrAtData);
		check(PtrAtDataAsObject);
		ObjectDumpInterpretation = PtrAtDataAsObject->GetType();
	}

	std::clog << ObjectDumpInterpretation->GetFullName() << "*";
	std::string Indent(IndentationLevel, ' ');

	if (MaxDumpDepth == 0)
	{
		std::clog << " {" << std::hex << "0x" << *PtrAtData << "}";
	}
	else
	{
		ObjectDumpInterpretation->Dump_Internal(*PtrAtData, MaxDumpDepth - 1, IndentationLevel, false);
	}
}

RCLASS_INITIALIZE_EMPTY(OType)
