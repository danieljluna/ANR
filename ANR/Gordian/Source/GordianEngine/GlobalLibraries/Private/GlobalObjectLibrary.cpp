// Gordian by Daniel Luna (2019)

#include "../Public/GlobalObjectLibrary.h"
#include "GordianEngine/Debug/Public/Asserts.h"

using namespace Gordian;

TPrefixTree<const OType*> FGlobalObjectLibrary::_AllTypesByName;

// Registers a type by name to be searched for later
/*static*/ bool FGlobalObjectLibrary::RegisterType(const OType* TypeToRegister)
{
	check(TypeToRegister != nullptr);

	return _AllTypesByName.Insert(TypeToRegister->GetFullName(), TypeToRegister);
}

/*static*/ const OType* FGlobalObjectLibrary::FetchTypeByName(const std::string& TypeName)
{
	const OType* const * TypeMatchedToKey = _AllTypesByName.Find(TypeName);
	if (TypeMatchedToKey != nullptr)
	{
		return *TypeMatchedToKey;
	}

	return nullptr;
}