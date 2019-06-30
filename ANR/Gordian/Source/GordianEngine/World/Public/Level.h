// Gordian by Daniel Luna (2019)

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

	bool IsValid() const { return _ErrorCode == 0; }

	bool LoadLevel(const std::string& InFilePath);

private:

	errno_t _ErrorCode;

	// This should eventually be some kind of struct with initialization overrides
	std::vector<OType_Struct*> _Specifications;


};


};
