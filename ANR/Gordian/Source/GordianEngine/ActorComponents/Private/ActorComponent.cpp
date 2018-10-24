// Gordian by Daniel Luna

#include "GordianEngine/Reflection/Public/Type_Struct.h"

#include "GordianEngine/Actor/Public/ActorComponent.h"
#include "GordianEngine/Actor/Public/Actor.h"

#include "GordianEngine/GlobalLibraries/Public/GlobalObjectLibrary.h"

using namespace Gordian;

OActorComponent::OActorComponent(const std::string& InName, OObject* InOwningObject)
	: Parent(InName, InOwningObject)
	, _bIsTicking(false)
{

}

void OActorComponent::Initialize(AActor* ActorInitializingFrom)
{
	// Sanity check to avoid bad owning trees
	assert(ActorInitializingFrom == GetOwningActor());
}

void OActorComponent::OnBeginPlay()
{
	assert(SetFlagIfNotSet(EObjectFlags::HasInitiatedBeginPlay));

	assert(SetFlagIfNotSet(EObjectFlags::HasCompleteBeginPlay));
}

AActor* OActorComponent::GetOwningActor() const
{
	OObject* Owner = GetOwningObject();
	while (Owner != nullptr)
	{
		AActor* PossibleOwningActor = Cast<AActor>(Owner);
		if (PossibleOwningActor != nullptr)
		{
			return PossibleOwningActor;
		}
	}

	return nullptr;
}

RCLASS_MEMBER_BEGIN(OActorComponent)
RCLASS_MEMBER_END()
