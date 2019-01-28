// Gordian by Daniel Luna (2019)

#include "GordianEngine/World/Public/Level.h"
#include "GordianEngine/Core/Public/Gordian.h"

#include "GordianEngine/Actor/Public/Actor.h"

using namespace Gordian;


OLevel::OLevel(const std::string& InName)
	: OObject(InName, nullptr)
{

}

RCLASS_MEMBER_BEGIN(OLevel)
RCLASS_MEMBER_ADD(_ActorList)
RCLASS_MEMBER_END()
