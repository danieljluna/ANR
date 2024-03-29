// Gordian by Daniel Luna (2019)

#pragma once

#include <string>
#include <vector>

#include "GordianEngine/Core/Public/Object.h"

namespace Gordian
{


// Abstract Base. Stores all reflected data about a given type.
class OType : public OObject
{
	REFLECT_CLASS(OObject)

	friend class OType_Pointer;
	friend class OType_Struct;
	friend class OType_StdVector;
public:

	OType(const std::string& Name, size_t Size);
	virtual ~OType();

	virtual std::string GetFullName() const;


	// Sets the size in memory this type takes up
	inline void SetSize(size_t NewSize)
	{
		Size = NewSize;
	}
	// Gets the size in memory this type takes up
	inline size_t GetSize() const
	{
		return Size;
	}

	// Cheap helper to determine if this type inherits from OType_Struct
	inline bool IsStruct() const
	{
		return bIsStructType;
	}

	virtual bool IsChildClassOf(const OType_Struct* PossibleParent) const
	{
		return false;
	}

	// Given a pointer to an object of this type, output all reflected data
	void Dump(const void* Data, size_t MaxDumpDepth) const;

protected:

	virtual void Dump_Internal(const void* Data, 
							   size_t MaxDumpDepth, 
							   int IndentationLevel, 
							   bool bShouldPrintName) const = 0;

	bool bIsStructType;
	size_t Size;
	const size_t k_IndentationWidth = 3;
};


///////////////////////////////////////////////////////////////////////
// std::vector specialization
///////////////////////////////////////////////////////////////////////

class OType_StdVector : public OType
{
public:
	inline const OType* GetItemType() const
	{
		return ItemType;
	}

	// Ctor
	template <typename ItemType>
	OType_StdVector(const ItemType*)			//typename is true parameter here
		: OType{ "std::vector", sizeof(std::vector<ItemType>) }
		, ItemType{ FTypeResolver<ItemType>::Get() }
	{
		// Store Accessor fns
		GetSize = [](const void* VectorPtr) -> size_t
		{
			const std::vector<ItemType>& VectorData = *(const std::vector<ItemType>*) VectorPtr;
			return VectorData.size();
		};
		GetItem = [](const void* VectorPtr, size_t Index) -> const void *
		{
			const std::vector<ItemType>& VectorData = *(const std::vector<ItemType>*) VectorPtr;
			return &VectorData.at(Index);
		};
	}

	virtual std::string GetFullName() const override
	{
		return std::string("std::vector<" + ItemType->GetFullName() + ">");
	}


protected:

	virtual void Dump_Internal(const void* Data, 
							   size_t MaxDumpDepth, 
							   int IndentationLevel, 
							   bool bShouldPrintName) const override
	{
		size_t VectorSize = GetSize(Data);
		std::clog << GetFullName() << " {size: " << std::dec << VectorSize << "}";

		if (VectorSize <= 0)
		{
			return;
		}

		std::string Indent(IndentationLevel, ' ');
		std::clog << std::endl << Indent << "{" << std::endl;

		std::string ItemIndent(IndentationLevel + k_IndentationWidth, ' ');
		for (size_t i = 0; i < VectorSize; ++i)
		{
			std::clog << ItemIndent << "[" << i << "]: ";
			const void* ItemLocation = GetItem(Data, i);
			ItemType->Dump_Internal(ItemLocation, MaxDumpDepth, IndentationLevel + k_IndentationWidth, true);
			std::clog << std::endl;

			if (i == k_MaxItemDisplayCount && VectorSize > 1)
			{
				i = VectorSize - 1;
				std::clog << "..." << std::endl;
			}
		}

		std::clog << Indent << "}";
	}

	const OType* ItemType;
	size_t(*GetSize)(const void*);
	const void* (*GetItem)(const void*, size_t);
	const size_t k_MaxItemDisplayCount = 5;

};


// Overrides the type resolver for vectors
template<typename T>
class FTypeResolver<std::vector<T>>
{
public:
	static const OType* Get()
	{
		static OType_StdVector TypeDescriptor{ (T*) nullptr };
		return &TypeDescriptor;
	}
};


///////////////////////////////////////////////////////////////////////
// Pointer Specialization
///////////////////////////////////////////////////////////////////////

class OType_Pointer : public OType
{
public:
	inline const OType* GetItemType() const
	{
		return ItemType;
	}

	template <typename ItemType>
	OType_Pointer(const ItemType*)
		: OType{ "*", sizeof(ItemType*) }
		, ItemType{ FTypeResolver<ItemType>::Get() }
	{
		SetName(FTypeResolver<ItemType>::Get()->GetName() + "*");
	}

	virtual std::string GetFullName() const override
	{
		return std::string(ItemType->GetFullName() + "*");
	}


protected:

	virtual void Dump_Internal(const void* Data, 
							   size_t MaxDumpDepth, 
							   int IndentationLevel, 
							   bool bShouldPrintName) const override;

	const OType* ItemType;

};


template<typename T>
typename std::enable_if<std::is_pointer<T>::value, const OType*>::type GetPrimitiveDescriptor()
{
	static OType_Pointer TypeDescription{ (T) nullptr };
	return &TypeDescription;
}




};	// namespace Gordian
