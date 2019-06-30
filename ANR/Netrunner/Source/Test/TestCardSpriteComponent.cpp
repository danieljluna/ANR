// ANR by Daniel Luna (2019)

#include "TestCardSpriteComponent.h"
#include "Core/Public/ANR.h"

#include "SFML/Graphics/Rect.hpp"

#include "GordianEngine/Input/Public/InputManager.h"

using namespace Gordian;
using namespace ANR;


OTestCardSpriteComponent::OTestCardSpriteComponent(const std::string& InName, OObject* InOwningObject)
	: Parent(InName, InOwningObject)
{

}


void OTestCardSpriteComponent::Initialize(AActor* ActorInitializingFrom)
{
	_FileToLoadTextureFrom = "../NDB_Scraper/Output/06041.png";
	_TextureToRender.loadFromFile(_FileToLoadTextureFrom);
	_SpriteToRender.setTexture(_TextureToRender);
	sf::IntRect DefaultCardRect(0, 0, 300, 419);
	_SpriteToRender.setTextureRect(DefaultCardRect);
	FInputManager::Get()->BindToDigitalCommand("TestCommand", EDigitalEventType::Pressed, this, &OTestCardSpriteComponent::TestFunction);
}

void OTestCardSpriteComponent::TestFunction()
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

RCLASS_MEMBER_BEGIN(OTestCardSpriteComponent)
RCLASS_MEMBER_END()