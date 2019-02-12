// Goridan by Daniel Luna (2019)

#pragma once

namespace Gordian
{

// A structure that can optionally contain a value of type T
//
// At it's core this is just a wrapper around a T* that treats nullptr as 'None'
template <typename T>
class TOptional
{
public:

	// Default Constructor
	TOptional();
	// Value Constructor
	TOptional(const T& Value);
	// Copy Constructor
	TOptional(const TOptional<T>& CopySource);
	// Move Constructor
	TOptional(TOptional<T>&& MoveSource);

	// Destructor
	~TOptional();

	// Returns true iff the value has been set.
	bool IsSet() const;

	// Returns the value as const
	const T& Get() const;

	// Returns the value as non-const
	T& Get();

	// Sets the value.
	void Set(const T& NewValue);

	// Clears any value. Will no longer be 'set'.
	void Clear();

private:

	T* _OptionalValue;
};


};	// namespace Gordian

#include "../Private/TOptional.inl"
