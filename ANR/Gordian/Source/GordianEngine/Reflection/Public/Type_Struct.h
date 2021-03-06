// Gordian by Daniel Luna (2019)

#pragma once

#include "Type.h"

#include <vector>

#include "StructMember.h"
#include "GordianEngine/Debug/Public/Asserts.h"

namespace Gordian
{


// Type used for structs and classes.
// This type also includes all reflection data for members of the type.
class OType_Struct : public OType
{
	REFLECT_CLASS(OType)

public:

	static FStructMember NullMember;

	OType_Struct();
	OType_Struct(void(*Initialize)(OType_Struct*));

	// Ensures this has been initialized
	// Uses const cast to avoid const issues
	void EnsureInitialization() const;

	// If T is a reflected class, 
	template<typename Parent>
	typename std::enable_if<!std::is_same<Parent, void>::value>::type SetParentClass(const Parent*)
	{
		const OType* ParentType = FTypeResolver<Parent>::Get();
		ParentClass = static_cast<const OType_Struct*>(ParentType);
		if (ParentClass == this)
		{
			ParentClass = nullptr;
			ClassDepth = 0;
		}
		else
		{
			ParentClass->EnsureInitialization();

			check(ParentType->IsStruct());

			if (ParentClass->IsChildClassOf(this))
			{
				// todo: Print some error msg about circular parent classes
				checkNoEntry();
				return;
			}

			ClassDepth = ParentClass->ClassDepth + 1;
			Members.insert(std::end(Members), 
						   std::begin(ParentClass->Members),
						   std::end(ParentClass->Members));
		}
	}

	// If T is void
	template<typename Parent>
	typename std::enable_if<std::is_same<Parent, void>::value>::type SetParentClass(const Parent*)
	{
		ParentClass = nullptr;
		ClassDepth = 0;
	}


	// Adds non-inherited members to class
	inline void DeclareMembers(const std::initializer_list<FStructMember>& InMembers)
	{
		Members.insert(Members.end(), InMembers);
	}
	// Returns all members
	inline const std::vector<FStructMember>& GetMembers() const
	{
		return Members;
	}
	// Returns the first member with the given name
	inline const FStructMember* GetMember(const char* MemberName) const
	{
		for (const FStructMember& Member : Members)
		{
			if (std::strcmp(MemberName, Member.Name) == 0)
			{
				return &Member;
			}
		}

		return nullptr;
	}
	// Returns true if a member with the given name exists
	inline bool DoesMemberExist(const char* MemberName) const
	{
		for (const FStructMember& Member : Members)
		{
			if (std::strcmp(MemberName, Member.Name) == 0)
			{
				return true;
			}
		}

		return false;
	}

	// Returns true if this class is a child of PossibleParent or if they 
	//	are the same class.
	virtual bool IsChildClassOf(const OType_Struct* PossibleParent) const override;

	// Gets the parent type. Prefer to AttemptToGetParentType where possible.
	inline const OType_Struct* GetParentType() const
	{
		return ParentClass;
	}

protected:

	enum class EInitializationState : sf::Uint8
	{
		Uninitialized = 0,
		MidInitialization,
		FullyInitialized,
	};

	// Returns the initialization state of this type.
	//	While Uninitialized, any read action is undefined.
	//	While MidInitialization, it is not guaranteed that all members have been registered.
	//  While FullyInitialized, all actions are safe.
	inline const EInitializationState& GetInitializationState() const
	{
		return _InitializationState;
	}

	virtual void Dump_Internal(const void* Data, 
							   size_t MaxDumpDepth, 
							   int IndentationLevel, 
							   bool bShouldPrintName) const override;

	std::vector<FStructMember> Members;
	const OType_Struct* ParentClass;
	// Describes how far this class is from it's inheritance root.
	unsigned int ClassDepth;

private:

	// Marks whether or not this type has been initialized.
	// Reading this object while uninitialized will result in undefined behavior
	EInitializationState _InitializationState;

	void(*_InitializeFunc)(OType_Struct*);

	// Private Initialization Method
	void _InternalInitialize();
};


};	// namespace Gordian
