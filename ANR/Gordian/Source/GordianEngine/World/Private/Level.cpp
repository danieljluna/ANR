// Gordian by Daniel Luna

#include "GordianEngine/World/Public/Level.h"

#include "GordianEngine/Actor/Public/Actor.h"
#include "GordianEngine/Reflection/Public/Type_Struct.h"

using namespace Gordian;


OLevel::OLevel(const std::string& InName)
	: OObject(InName, nullptr)
{

}

RCLASS_MEMBER_BEGIN(OLevel)
RCLASS_MEMBER_ADD(_ActorList)
RCLASS_MEMBER_END()
