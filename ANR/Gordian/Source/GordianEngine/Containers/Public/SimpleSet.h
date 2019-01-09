// Gordian by Daniel Luna

#pragma once

namespace Gordian;

// A simple set focused on speed and efficiency in both size and iteration.
// Some functions will not maintain order and pre-allocation will still construct new items.
// Should only be used for primitive types and pointers, avoid using larger classes
// like OObjects.
template<class ItemType>
class FSimpleSet
{
public:

	FSimpleSet(size_t InitialCapacity = 0)
	{
		_ArrayHead = new FSimpleSet[InitialCapacity];
		_ItemCount = 0;
		_ArrayCapacity = InitialCapacity;
	}

	FSimpleSet(const FSimpleSet& Other)
	{
		_ArrayHead = new FSimpleSet[Other._ItemCount];
		_ItemCount = Other._ItemCount;
		_ArrayCapacity = Other._ItemCount;

		for (size_t Index = 0; Index < _ItemCount; ++Index)
		{
			_ArrayHead[Index] = ItemType(Other._ArrayHead[Index]);
		}
	}

	~FSimpleSet()
	{
		delete[] _ArrayHead;
		_ArrayHead = nullptr;
	}

	const FSimpleSet& operator=(const FSimpleSet& Other);

	inline ItemType& operator[](const size_t index)
	{
		check(index < _ItemCount);
		return _ArrayHead[index];
	}

	inline const ItemType& operator[](const size_t index) const
	{
		check(index < _ItemCount);
		return _ArrayHead[index];
	}

	inline size_t GetItemCount() const
	{
		return _ItemCount;
	}

	// Sets the capacity manually, returning success status.
	// Excess capacity is still initialized data, so be wary of setting it too high.
	// If bAllowTrimming is false, attempting to set the capacity to a value
	// smaller than the current number of items will always fail. If it is true,
	// items outside of the new capacity will be lost.
	inline bool SetCapacity(const size_t NewCapacity, const bool bAllowTrimming)
	{
		if (_ArrayCapacity == NewCapacity)
		{
			// Nothing needs to change, just early exit
			return true;
		}

		if (NewCapacity < _ItemCount && !bAllowTrimming)
		{
			return false;
		}

		ItemType* NewHead = new ItemType[NewCapacity];
		if (NewHead == nullptr)
		{
			return false;
		}

		const size_t NewItemCount = NewCapacity < _ItemCount ? NewCapacity : _ItemCount;
		for (size_t Index = 0; Index < NewItemCount; ++Index)
		{
			NewHead[Index] = _ArrayHead[Index];
		}

		delete[] _ArrayHead;
		_ArrayHead = NewHead;
		_ItemCount = NewItemCount;
		_ArrayCapacity = NewCapacity;

		return true;
	}

	// Ensures there is enough capacity to support the given number of items.
	// Will do nothing if the current capacity already exceeds this value.
	// Returns whether or not we can ensure space for the given NewCapacity.
	inline bool Reserve(const size_t NewCapacity)
	{
		if (_ArrayCapacity >= NewCapacity)
		{
			return true;
		}

		return SetCapacity(NewCapacity, false);
	}
	
	// Returns the capacity of the array.
	// Unused capacity is still constructed data, it's just ignored.
	inline size_t GetCapacity() const
	{
		return _ArrayCapacity;
	}

	inline bool Append(const ItemType& NewItem)
	{
		if (!Reserve(_ItemCount + 1))
		{
			return false;
		}

		_ArrayHead[_ItemCount] = NewItem;
		++_ItemCount;
		return true;
	}

	inline void PopBack()
	{
		if (_ItemCount > 0)
		{
			// Note we simply ignore the old data
			--_ItemCount;
		}
	}

	// Does not conserve item order, but does conserve iteration consistency
	inline void RemoveSloppy(size_t IndexToRemove)
	{
		check(IndexToRemove < _ItemCount);

		const size_t LastItem = _ItemCount - 1;
		if (IndexToRemove != LastItem)
		{
			std::swap<ItemType>(_ArrayHead[IndexToRemove], _ArrayHead[LastItem]);
		}

		PopBack();
	}

private:

	ItemType* _ArrayHead;
	size_t _ItemCount;
	size_t _ArrayCapacity;

};

template<class T>
const FSimpleSet<T>& FSimpleSet<T>::operator=(const FSimpleSet<T>& Other)
{
	if (this == &Other)
	{
		return *this;
	}

	Reserve(Other._ItemCount);
	_ItemCount = Other._ItemCount;

	for (size_t Index = 0; Index < _ItemCount; ++Index)
	{
		_ArrayHead[Index] = Other._ArrayHead[Index];
	}

	return *this;
}

