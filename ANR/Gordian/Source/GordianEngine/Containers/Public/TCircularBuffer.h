// Gordian by Daniel Luna (2020)

#pragma once

namespace Gordian
{


// Denotes a circular FIFO buffer of fixed size.
// If data is ever pushed when the buffer is full, it stomps the oldest data
template<typename T>
class TCircularBuffer
{
public:
	TCircularBuffer(size_t InBufferSize = 0);
	TCircularBuffer(const TCircularBuffer<T>& Other);
	TCircularBuffer(TCircularBuffer<T>&& Other);
	~TCircularBuffer();

	// Fetches element at Index. 
	// Doesn't check if entry has been set.
	inline const T& operator[](size_t Index) const;

	// Fetches element at Index. 
	// Doesn't check if entry has been set.
	inline T& operator[](size_t Index);

	// Fetches element at Index if it has been set.
	const T& At(size_t Index) const;

	// Fetches element at Index if it has been set.
	T& At(size_t Index);

	// Max number of items the buffer can hold
	inline size_t Capacity() const;

	// Number of items in the buffer
	inline size_t Num() const;

	inline bool IsEmpty() const;
	inline bool IsFull() const;

	// Resizes the buffer, clearing all contents.
	// This is not cheap, and generally should be avoided.
	bool Resize(size_t InSize);

	// Removes all entries in the buffer
	inline void Empty();

	// Pushes a value to the end of the buffer
	// If the buffer is full, the oldest value is overridden.
	// Returns the entity count after the op
	size_t Enqueue(const T& ValueToCopy);

	// Removes the value at the front of the buffer.
	// Returns the entity count after the op
	size_t Dequeue();

	// Returns the oldest value still in the buffer.
	// Does not modify the buffer.
	inline const T* Front() const;

	// Returns the newest value still in the buffer.
	// Does not modify the buffer.
	inline const T* Back() const;

private:
	// Converts an index to its raw position in the array
	inline size_t TransformIndexToPosition(size_t Index) const;

	// Offset of circular first item from the start of buffer
	size_t BufferOffset;
	// Number of items in the buffer
	size_t EntityCount;
	// Max capacity of the buffer
	size_t BufferSize;
	// Buffer allocation
	T* Buffer;
};


};

#include "../Private/TCircularBuffer.inl"
