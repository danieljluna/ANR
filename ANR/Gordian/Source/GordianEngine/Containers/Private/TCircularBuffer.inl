// Gordian by Daniel Luna (2020)

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/Logging.h"

namespace
{
	DECLARE_LOG_CATEGORY_STATIC(LogCircularBuffer, Log, Verbose)
};

template<typename T>
Gordian::TCircularBuffer<T>::TCircularBuffer(size_t InBufferSize)
{
	EntityCount = 0;
	BufferOffset = 0;
	BufferSize = InBufferSize;
	if (BufferSize > 0)
	{
		Buffer = new T[BufferSize];
	}
}

template<typename T>
Gordian::TCircularBuffer<T>::TCircularBuffer(const TCircularBuffer<T>& Other)
{
	EntityCount = Other.EntityCount;
	BufferOffset = Other.BufferOffset;
	BufferSize = Other.BufferSize;
	
	if (Other.Buffer == nullptr)
	{
		Buffer = nullptr;
	}
	else
	{
		Buffer = new T[BufferSize];
		for (size_t i = 0; i < BufferSize; ++i)
		{
			Buffer[i] = Other.Buffer[i];
		}
	}
}

template<typename T>
Gordian::TCircularBuffer<T>::TCircularBuffer(TCircularBuffer<T>&& Other)
{
	EntityCount = Other.EntityCount;
	BufferOffset = Other.BufferOffset;
	BufferSize = Other.BufferSize;
	Buffer = Other.Buffer;

	Other.EntityCount = 0;
	Other.BufferOffset = 0;
	BufferSize = 0;
	Other.Buffer = nullptr;
}

template<typename T>
Gordian::TCircularBuffer<T>::~TCircularBuffer()
{
	delete[] Buffer;
}

template<typename T>
size_t Gordian::TCircularBuffer<T>::TransformIndexToPosition(size_t Index) const
{
	size_t ReturnValue = (Index + BufferOffset) % BufferSize;
	return ReturnValue;
}

template<typename T>
const T& Gordian::TCircularBuffer<T>::operator[](size_t Index) const
{
	const size_t FinalPosition = TransformIndexToPosition(Index);
	return Buffer[FinalPosition];
}

template<typename T>
T& Gordian::TCircularBuffer<T>::operator[](size_t Index)
{
	const size_t FinalPosition = TransformIndexToPosition(Index);
	return Buffer[FinalPosition];
}

template<typename T>
const T& Gordian::TCircularBuffer<T>::At(size_t Index) const
{
	if (Index < 0)
	{
		GE_LOG(LogCircularBuffer, Fatal, "Index (%d) must be non-negative.");
	}

	if (Index >= EntityCount)
	{
		GE_LOG(LogCircularBuffer, Fatal, "Attempted to access item %d, but there are only %d", Index, EntityCount);
	}

	const size_t FinalPosition = TransformIndexToPosition(Index);
	return Buffer[FinalPosition];
}

template<typename T>
T& Gordian::TCircularBuffer<T>::At(size_t Index)
{
	if (Index < 0)
	{
		GE_LOG(LogCircularBuffer, Fatal, "Index (%d) must be non-negative.");
	}

	if (Index >= EntityCount)
	{
		GE_LOG(LogCircularBuffer, Fatal, "Attempted to access item %d, but there are only %d", Index, EntityCount);
	}

	const size_t FinalPosition = TransformIndexToPosition(Index);
	return Buffer[FinalPosition];
}

template<typename T>
size_t Gordian::TCircularBuffer<T>::Num() const
{
	return EntityCount;
}

template<typename T>
bool Gordian::TCircularBuffer<T>::IsEmpty() const
{
	return EntityCount == 0;
}

template<typename T>
bool Gordian::TCircularBuffer<T>::IsFull() const
{
	return EntityCount == BufferSize;
}

template<typename T>
bool Gordian::TCircularBuffer<T>::Resize(size_t InSize)
{
	EntityCount = 0;
	BufferOffset = 0;
	BufferSize = InSize;

	delete[] Buffer;
	Buffer = new T[BufferSize];
	return true;
}

template<typename T>
void Gordian::TCircularBuffer<T>::Empty()
{
	EntityCount = 0;
	BufferOffset = 0;
}

template<typename T>
size_t Gordian::TCircularBuffer<T>::Enqueue(const T& ValueToCopy)
{
	const size_t FinalPosition = (BufferOffset + EntityCount) % BufferSize;

	Buffer[FinalPosition] = ValueToCopy;
	if (EntityCount < BufferSize)
	{
		++EntityCount;
	}
	else
	{
		// Move the start back one since we overwrote the start
		BufferOffset = (BufferOffset + 1) % BufferSize;
	}

	return EntityCount;
}

template<typename T>
size_t Gordian::TCircularBuffer<T>::Dequeue()
{
	if (EntityCount == 0)
	{
		GE_LOG(LogCircularBuffer, Verbose, "Called Dequeue on an empty Circular Buffer.");
		return 0;
	}

	BufferOffset = (BufferOffset + 1) % BufferSize;
	--EntityCount;

	return EntityCount;
}

template<typename T>
const T* Gordian::TCircularBuffer<T>::Front() const
{
	if (EntityCount == 0)
	{
		return nullptr;
	}

	return &Buffer[BufferOffset];
}

template<typename T>
const T* Gordian::TCircularBuffer<T>::Back() const
{
	if (EntityCount == 0)
	{
		return nullptr;
	}

	const size_t FinalPosition = (BufferOffset + EntityCount) % BufferSize;
	return &Buffer[FinalPosition];
}
