// Gordian by Daniel Luna (2019)

#include "GordianEngine/Actor/Public/ActorComponent.h"
#include "GordianEngine/Core/Public/Gordian.h"

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
	check(ActorInitializingFrom == GetOwningActor());
}

void OActorComponent::OnBeginPlay()
{
	check(SetFlagIfNotSet(EObjectFlags::HasInitiatedBeginPlay));

	check(SetFlagIfNotSet(EObjectFlags::HasCompleteBeginPlay));
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

RCLASS_INITIALIZE_EMPTY(OActorComponent)
