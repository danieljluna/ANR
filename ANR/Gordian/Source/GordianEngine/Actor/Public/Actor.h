// Gordian by Daniel Luna (2019)

#pragma once

#include "GordianEngine/Core/Public/Object.h"
#include "GordianEngine/Core/Public/Renderable.h"

namespace Gordian
{

class OActorComponent;

/// Component based entity that exists in the world
///
/// Long term goal to support networking
class AActor : public OObject
			 , public IRenderable
{
	REFLECT_CLASS(OObject)
public:

	AActor(const std::string& InName, OObject* InOwningObject);
	virtual ~AActor() override;

	// Initializes the Actor, including its components. 
	// This happens immediately after creation, and before this actor begins play.
	virtual void Initialize() override;
	// Starts up this actor.
	// This is not called until this actor
	virtual void BeginPlay();
	// Tick this actor.
	virtual void Tick(const sf::Time& DeltaTime) {};
	// Destroys this actor.
	void Destroy() {};

	// Adds the passed component to this actor, removing
	bool AddComponent(OActorComponent* ComponentToAdd);

	// Temp ease of use to render
	virtual void Render(sf::Time BlendTime, sf::RenderTarget& Target, sf::RenderStates States) const override;

protected:

	// Called before initializing components
	virtual void PreInitializeComponents() {};
	// Call this after allocation and before BeginPlay to initialize components properly
	void InitializeComponents();
	// Called after initializing components
	virtual void PostInitializeComponents() {};

	// Returns all components of this actor
	const std::vector<OActorComponent*>& GetComponents();

private:

	// Eventually we should use this to prevent iterating over non-ticking actors
	bool _bIsTicking;

	std::vector<OActorComponent*> _ActorComponents;
};


};	// namespace Gordian
