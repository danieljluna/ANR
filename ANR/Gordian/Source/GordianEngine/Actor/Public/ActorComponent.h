// Gordian by Daniel Luna

#pragma once

#include "GordianEngine/Core/Public/Object.h"

namespace Gordian
{

class AActor;

// A component that extends the functionality of an Actor
class OActorComponent : public OObject
{
	REFLECT_CLASS(OObject)

public:

	OActorComponent(const std::string& InName, OObject* InOwningObject);

	/// Called when this component is added to an actor. Should be used
	///	  to set up the component
	virtual void Initialize(AActor* ActorInitializingFrom);
	/// Called when the owning object begins play.
	virtual void OnBeginPlay();

	virtual void Tick(const sf::Time& DeltaTime) {};

	AActor* GetOwningActor() const;

private:
	
	// todo: use _bIsTicking to prevent iterating over non-ticking components
	bool _bIsTicking;

};


};
