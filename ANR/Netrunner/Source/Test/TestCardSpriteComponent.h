// ANR by Daniel Luna (2019)

#pragma once

#include "GordianEngine/ActorComponents/Public/SimpleSpriteComponent.h"

namespace ANR
{


// This is just a test bed for the engine code that displays a card from
// a sprite sheet. This should not be part of the final code.
class OTestCardSpriteComponent : public Gordian::OSimpleSpriteComponent
{
	REFLECT_CLASS(Gordian::OSimpleSpriteComponent)

public:

	OTestCardSpriteComponent(const std::string& InName, OObject* InOwningObject);

	//-OActorComponent------------------

	virtual void Initialize(Gordian::AActor* ActorInitializingFrom) override;

	//-End-OActorComponent------------------


	// Just a garbage test function for input testing.
	void TestFunction();

};


};
