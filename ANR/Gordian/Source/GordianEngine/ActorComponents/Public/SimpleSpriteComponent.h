// Gordian by Daniel Luna (2019)

#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "GordianEngine/Actor/Public/ActorComponent.h"
#include "GordianEngine/Core/Public/Renderable.h"

namespace Gordian
{

class AActor;


// Proof of concept for a renderable component that draws a sfml sprite
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

protected:

	sf::Sprite _SpriteToRender;

	sf::Texture _TextureToRender;

	std::string _FileToLoadTextureFrom;
};


};
