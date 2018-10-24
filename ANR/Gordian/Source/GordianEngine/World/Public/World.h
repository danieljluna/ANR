// Gordian by Daniel Luna

#pragma once

#include <vector>

#include "SFML/Graphics/View.hpp"
#include "SFML/System/Time.hpp"

#include "GordianEngine/Actor/Public/Actor.h"
#include "GordianEngine/GlobalLibraries/Public/GlobalObjectLibrary.h"
#include "GordianEngine/Core/Public/Object.h"
#include "GordianEngine/Core/Public/Renderable.h"

namespace Gordian
{

class OLevel;

class OWorld : public OObject
			 , public IRenderable
{
public:

	REFLECT_CLASS(OObject)

	OWorld();

	virtual const OWorld* GetWorld() const;

	void BeginPlay();
	void Tick(const sf::Time& DeltaSeconds);
	virtual void Render(sf::Time BlendTime, sf::RenderTarget& Target, sf::RenderStates States) const;


	// Level Management -------------------------------

	// Loads a level into this world, returning true if loading was successful.
	bool LoadLevel(const OLevel* LevelToLoad);

	// ------------------------------------------------



	// Spawns an actor in this world
	// todo: inline OWorld::SpawnActor
	template<typename T, typename std::enable_if<std::is_base_of<AActor, T>::value, int>::type = 0>
	T* SpawnActor(const OType_Struct* ActorType,
				  const std::string& ActorName = "")
	{
		// todo: this should eventually use pooled objects where possible
		T* NewActor = FGlobalObjectLibrary::CreateObject<T>(this, ActorType, ActorName);

		if (NewActor != nullptr)
		{
			NewActor->Initialize();
			RegisterActorWithWorld(NewActor);
		}

		return NewActor;
	}

	// Returns whether the actor was successfully registered
	bool RegisterActorWithWorld(AActor* ActorToRegister);

private:

	// A list of all actors managed directly by this world.
	std::vector<AActor*> _Actors;

	const OLevel* _CurrentlyLoadedLevel;

};

}


