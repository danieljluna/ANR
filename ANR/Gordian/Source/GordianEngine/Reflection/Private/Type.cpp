// Gordian by Daniel Luna

#include "GordianEngine/Reflection/Public/Type.h"
#include "GordianEngine/Core/Public/Gordian.h"


using namespace Gordian;

namespace
{
	const int k_IndentationWidth = 3;
}

OType::OType(const std::string& Name, size_t Size) 
	: OObject(Name, nullptr)
	, Size{ Size }
{
	bIsStructType = false;
}

OType::~OType()
{

}

std::string OType::GetFullName() const
{
	return GetName();
}

void OType::Dump(const void* Data) const
{
	// Preserve existing format flags
	std::ios_base::fmtflags PreviousFormat(std::clog.flags());

	Dump_Internal(Data, 0);

	// Load back up old formatting
	std::clog.flags(PreviousFormat);
}

RCLASS_MEMBER_BEGIN(OType)
RCLASS_MEMBER_END()
