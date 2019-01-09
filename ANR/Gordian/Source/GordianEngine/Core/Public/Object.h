// Gordian by Daniel Luna

#pragma once

#include <string>
#include <vector>

#include "SFML/Config.hpp"
#include "SFML/System/Time.hpp"

#include "GordianEngine/BitSet/Public/BitSet.h"
#include "GordianEngine/Reflection/Public/ReflectionMacros.h"
#include "GordianEngine/Reflection/Public/TypeResolver.h"

namespace Gordian
{

class FGlobalObjectLibrary;
class OType;
class OType_Struct;
class OWorld;

enum class EObjectFlags : uint16_t
{
    NoFlags = 0,
	ClassDefaultObject,
	HasInitiatedBeginPlay,
	HasCompleteBeginPlay,
    MAX_VALUE
};

/// Lightweight base class of all engine relevant classes.
/// 
///	Supports casting
/// Long term goal is to support function delegates.
/// All child classes should be reflected.
class OObject
{
	REFLECT_CLASS(void)

	friend FGlobalObjectLibrary;

public:

	OObject() = delete;
	// Generic object constructor. Should not be called directly.
    OObject(const std::string& InName, OObject* InOwningObject);
    virtual ~OObject();

	// This will get called immediately after spawning a new object.
	// Manual object creation should be sure to call this.
	virtual void Initialize();

    bool IsObjectFlagSet(EObjectFlags FlagToCheck) const;
    bool AreObjectFlagsSet(const TBitSet<EObjectFlags>& FlagsToCheck) const;

	OObject* GetOwningObject() const;
	virtual const OWorld* GetWorld() const;

	bool IsSubclassOf(const OType_Struct* ClassToCheckAgainst) const;
	inline const OType_Struct* GetType() const { return _PrivateType; }

	inline const std::string& GetName() const { return _Name; }
	inline void SetName(const std::string& NewName) { _Name = NewName; }

protected:

	// This function sets the given flag, returning true only if the flag was not yet set
	bool SetFlagIfNotSet(EObjectFlags Flag);

	// Tracks the objects state to avoid improper object lifecycles
    TBitSet<EObjectFlags> ObjectFlags;

private:

	// The human-readable name of the object
	std::string _Name;

	// Usually the object that created this one.
	OObject* _OwningObject;

	// The class of this object
	const OType_Struct* _PrivateType;

};

};	// namespace Gordian
