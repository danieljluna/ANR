// Gordian by Daniel Luna (2019)

#include "GordianEngine/ActorComponents/Public/SimpleSpriteComponent.h"
#include "GordianEngine/Core/Public/Gordian.h"

#include "SFML/Graphics/Rect.hpp"

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
}

void OSimpleSpriteComponent::Render(sf::Time BlendTime, sf::RenderTarget& Target, sf::RenderStates States) const
{
	// We should get the position first
	// Then offset it by the movement component's offset for the given blend time

	Target.draw(_SpriteToRender, States);
}

RCLASS_MEMBER_BEGIN(OSimpleSpriteComponent)
RCLASS_MEMBER_END()
