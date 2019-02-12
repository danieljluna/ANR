// Gordian by Daniel Luna (2019)

#include "GordianEngine/World/Public/World.h"
#include "GordianEngine/Core/Public/Gordian.h"

#include "GordianEngine/Actor/Public/Actor.h"
#include "GordianEngine/ActorComponents/Public/SimpleSpriteComponent.h"

#include "GordianEngine/Debug/Public/Asserts.h"

using namespace Gordian;

OWorld::OWorld(const std::string& InName, OObject* InOwningObject)
	: Parent(InName, InOwningObject)
	, _Actors{}
	, _CurrentlyLoadedLevel(nullptr)
{
}

const OWorld* OWorld::GetWorld() const
{
	return this;
}

void OWorld::BeginPlay()
{
	check(SetFlagIfNotSet(EObjectFlags::HasInitiatedBeginPlay));
	check(IsObjectFlagSet(EObjectFlags::HasInitiatedBeginPlay));


	//-Temp-Initialization-Logic----------------------------------------

	AActor* TestActor = SpawnActor<AActor>(AActor::GetStaticType(), "TestActor_0");
	OSimpleSpriteComponent* SimpleSpriteComponent = FGlobalObjectLibrary::CreateObject<OSimpleSpriteComponent>(TestActor, OSimpleSpriteComponent::GetStaticType(), "SimpleSprite");
	check(TestActor != nullptr && SimpleSpriteComponent != nullptr);
	TestActor->AddComponent(SimpleSpriteComponent);

	//-End-Temp-Initialization-Logic------------------------------------


	for (AActor* Actor : _Actors)
	{
		Actor->BeginPlay();
	}

	OWorld::GetStaticType()->Dump(this);
	check(SetFlagIfNotSet(EObjectFlags::HasCompleteBeginPlay));
}

void OWorld::Tick(const sf::Time& DeltaSeconds)
{
	if (!IsObjectFlagSet(EObjectFlags::HasCompleteBeginPlay))
	{
		BeginPlay();
	}

	for (AActor* Actor : _Actors)
	{
		Actor->Tick(DeltaSeconds);
	}
}

void OWorld::Render(sf::Time BlendTime, sf::RenderTarget& Target, sf::RenderStates States) const
{
	for (AActor* Actor : _Actors)
	{
		Actor->Render(BlendTime, Target, States);
	}
}

bool OWorld::LoadLevel(const OLevel* LevelToLoad)
{
	return _CurrentlyLoadedLevel == LevelToLoad;
}

bool OWorld::RegisterActorWithWorld(AActor* ActorToRegister)
{
	check(ActorToRegister != nullptr);

	// todo: add checks to ensure object is already properly initialized before registering successfully
	_Actors.push_back(ActorToRegister);

	if (IsObjectFlagSet(EObjectFlags::HasCompleteBeginPlay) 
		&& !ActorToRegister->IsObjectFlagSet(EObjectFlags::HasInitiatedBeginPlay))
	{
		ActorToRegister->BeginPlay();
	}

	return true;
}

RCLASS_MEMBER_BEGIN(OWorld)
RCLASS_MEMBER_ADD(_Actors)
RCLASS_MEMBER_END()
