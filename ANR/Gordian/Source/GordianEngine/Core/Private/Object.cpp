// Gordian by Daniel Luna (2019)

#include "GordianEngine/Core/Public/Object.h"
#include "GordianEngine/Core/Public/Gordian.h"

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/World/Public/World.h"

using namespace Gordian;

OObject::OObject(const std::string& InName, OObject* InOwningObject)
	: _Name(InName)
	, _OwningObject(InOwningObject)
{
	ObjectFlags.reset();
}

OObject::~OObject()
{

}

void OObject::Initialize()
{

}

bool OObject::IsObjectFlagSet(EObjectFlags FlagToCheck) const
{
    return ObjectFlags[FlagToCheck];
}

bool OObject::AreObjectFlagsSet(const TBitSet<EObjectFlags>& FlagsToCheck) const
{
    return (bool)(ObjectFlags & FlagsToCheck);
}

bool OObject::SetFlagIfNotSet(EObjectFlags Flag)
{
	if (ObjectFlags[Flag])
	{
		return false;
	}

	ObjectFlags.set(Flag);

	return true;
}

OObject* OObject::GetOwningObject() const
{
	return _OwningObject;
}

const OWorld* OObject::GetWorld() const
{
	if (_OwningObject != nullptr)
	{
		return _OwningObject->GetWorld();
	}

	return nullptr;
}

bool OObject::IsSubclassOf(const OType_Struct* ClassToCheckAgainst) const
{
	check(_PrivateType != nullptr);
	return _PrivateType->IsChildClassOf(ClassToCheckAgainst);
}

RCLASS_MEMBER_BEGIN(OObject)
RCLASS_MEMBER_ADD(_OwningObject)
RCLASS_MEMBER_ADD(_Name)
RCLASS_MEMBER_END()

