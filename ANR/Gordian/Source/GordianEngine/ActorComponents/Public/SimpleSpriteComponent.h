// Gordian by Daniel Luna

#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "GordianEngine/Actor/Public/ActorComponent.h"
#include "GordianEngine/Core/Public/Renderable.h"

namespace Gordian
{

class AActor;


// Defines
class OSimpleSpriteComponent : public OActorComponent
							 , public IRenderable
{
	REFLECT_CLASS(OActorComponent)

public:

	OSimpleSpriteComponent(const std::string& InName, OObject* InOwningObject);

	//-OActorComponent------------------

	virtual void Initialize(AActor* ActorInitializingFrom) override;

	//-End-OActorComponent------------------

	virtual void Render(sf::Time BlendTime, sf::RenderTarget& Target, sf::RenderStates States) const override;

	// Just a garbage test function for input testing.
	void TestFunction();

private:

	sf::Sprite _SpriteToRender;

	sf::Texture _TextureToRender;

	std::string _FileToLoadTextureFrom;
};


};
