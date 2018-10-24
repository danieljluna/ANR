// Gordian by Daniel Luna

#pragma once

#include "GordianEngine/Core/Public/Object.h"

#include <vector>

namespace Gordian
{


class AActor;

// Stores the actors that define a level.
// Will eventually support serialization to a file 
class OLevel : public OObject
{
	REFLECT_CLASS(OObject)

	OLevel(const std::string& InName);

private:

	std::vector<AActor*> _ActorList;
};


};
