// Gordian by Daniel Luna (2019)

#include "GordianEngine/World/Public/Level.h"
#include "GordianEngine/Core/Public/Gordian.h"

#include <cstdio>

#include "GordianEngine/Actor/Public/Actor.h"

using namespace Gordian;


OLevel::OLevel(const std::string& InName)
	: OObject(InName, nullptr)
	, _ErrorCode(-1)
	, _Specifications{}
{
	
}

bool OLevel::LoadLevel(const std::string& InFilePath)
{
	std::FILE* LevelFile = nullptr;
	_ErrorCode = fopen_s(&LevelFile, InFilePath.c_str(), "r");
	if (_ErrorCode != 0)
	{
		GE_LOG(LogFileIO, Warning, "LogOutput file could not be set up! Error code: %d!", _ErrorCode);
		return false;
	}

	

	return true;
}

RCLASS_MEMBER_BEGIN(OLevel)
RCLASS_MEMBER_ADD(_Specifications)
RCLASS_MEMBER_END()
