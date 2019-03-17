// Gordian by Daniel Luna (2019)

#include "GordianEngine/ActorComponents/Public/SimpleSpriteComponent.h"
#include "GordianEngine/Core/Public/Gordian.h"

#include "SFML/Graphics/Rect.hpp"

#include "GordianEngine/Input/Public/InputManager.h"

using namespace Gordian;

OSimpleSpriteComponent::OSimpleSpriteComponent(const std::string& InName, OObject* InOwningObject)
	: Parent(InName, InOwningObject)
	, _SpriteToRender()
	, _TextureToRender()
	, _FileToLoadTextureFrom("BAD_TEXT")
{

}

void OSimpleSpriteComponent::Initialize(AActor* ActorInitializingFrom)
{
	Parent::Initialize(ActorInitializingFrom);

	_FileToLoadTextureFrom = "F:/Programming/GitHub/ANR/ANR/NDB_Scraper/Output/06041.png";
	_TextureToRender.loadFromFile(_FileToLoadTextureFrom);
	_SpriteToRender.setTexture(_TextureToRender);
	sf::IntRect DefaultCardRect(0, 0, 300, 419);
	_SpriteToRender.setTextureRect(DefaultCardRect);
	FInputManager::Get()->BindToDigitalCommand("TestCommand", EDigitalEventType::Pressed, this, &OSimpleSpriteComponent::TestFunction);
}

void OSimpleSpriteComponent::Render(sf::Time BlendTime, sf::RenderTarget& Target, sf::RenderStates States) const
{
	// We should get the position first
	// Then offset it by the movement component's offset for the given blend time

	Target.draw(_SpriteToRender, States);
}

void OSimpleSpriteComponent::TestFunction()
{
	const sf::Texture* SpriteTexture = _SpriteToRender.getTexture();
	if (SpriteTexture != nullptr)
	{
		sf::IntRect Position = _SpriteToRender.getTextureRect();
		Position.left += 300;

		if (Position.left >= (int)(SpriteTexture->getSize().x))
		{
			Position.left = 0;
			Position.top += 419;
			if (Position.top >= (int)(SpriteTexture->getSize().y))
			{
				Position.top = 0;
			}

		}

		_SpriteToRender.setTextureRect(Position);
	}
}

RCLASS_MEMBER_BEGIN(OSimpleSpriteComponent)
RCLASS_MEMBER_END()
