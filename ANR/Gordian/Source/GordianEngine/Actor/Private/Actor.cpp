// Gordian by Daniel Luna

#include "GordianEngine/Actor/Public/Actor.h"
#include "GordianEngine/Core/Public/Gordian.h"


#include "GordianEngine/Debug/Public/Asserts.h"

#include "GordianEngine/Actor/Public/ActorComponent.h"
#include "GordianEngine/GlobalLibraries/Public/GlobalObjectLibrary.h"

#include "GordianEngine/ActorComponents/Public/SimpleSpriteComponent.h"

using namespace Gordian;

AActor::AActor(const std::string& InName, OObject* InOwningObject)
	: Parent(InName, InOwningObject)
	, _ActorComponents{}
	, _bIsTicking(false)
{
}

AActor::~AActor()
{

}

void AActor::Initialize()
{
	PreInitializeComponents();
	InitializeComponents();
	PostInitializeComponents();
}

void AActor::InitializeComponents()
{
	for (OActorComponent* ActorComponent : _ActorComponents)
	{
		ActorComponent->Initialize(this);
	}
}

void AActor::BeginPlay()
{
	check(SetFlagIfNotSet(EObjectFlags::HasInitiatedBeginPlay));

	for (OActorComponent* ActorComponent : _ActorComponents)
	{
		ActorComponent->OnBeginPlay();
	}

	check(SetFlagIfNotSet(EObjectFlags::HasCompleteBeginPlay));
}

bool AActor::AddComponent(OActorComponent* ComponentToAdd)
{
	_ActorComponents.push_back(ComponentToAdd);

	ComponentToAdd->Initialize(this);

	return true;
}

void AActor::Render(sf::Time BlendTime, sf::RenderTarget& Target, sf::RenderStates States) const
{
	for (const OActorComponent* ActorComponent : _ActorComponents)
	{
		const OSimpleSpriteComponent* RenderComponent = Cast<OSimpleSpriteComponent>(ActorComponent);
		if (RenderComponent != nullptr)
		{
			RenderComponent->Render(BlendTime, Target, States);
		}
	}
}


RCLASS_MEMBER_BEGIN(AActor)
RCLASS_MEMBER_ADD(_bIsTicking)
RCLASS_MEMBER_ADD(_ActorComponents)
RCLASS_MEMBER_END()
