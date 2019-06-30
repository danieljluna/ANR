// ANR by Daniel Luna (2019)

#pragma once

#include "GordianEngine/Actor/Public/Actor.h"

namespace ANR
{

class OTestCardSpriteComponent;


// Test actor that displays cards
class ATestCardActor : public Gordian::AActor
{
	REFLECT_CLASS(Gordian::AActor);

public:

	ATestCardActor(const std::string& InName, OObject* InOwningObject);

private:

	OTestCardSpriteComponent* _SpriteComponent;

};


}; // namespace ANR
