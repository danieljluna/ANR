// ANR by Daniel Luna (2019)

#include "TestCardActor.h"
#include "Core/Public/ANR.h"

#include "TestCardSpriteComponent.h"

using namespace Gordian;
using namespace ANR;

ATestCardActor::ATestCardActor(const std::string& InName, OObject* InOwningObject)
	: Parent(InName, InOwningObject)
{
	_SpriteComponent = FGlobalObjectLibrary::CreateObject<ANR::OTestCardSpriteComponent>(this,
																						 OTestCardSpriteComponent::GetStaticType(),
																						 "SimpleSprite");
	check(_SpriteComponent != nullptr);
	AddComponent(_SpriteComponent);
}

