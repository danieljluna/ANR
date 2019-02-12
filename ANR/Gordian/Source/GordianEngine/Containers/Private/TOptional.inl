// Gordian by Daniel Luna (2019)

#include "GordianEngine/Debug/Public/Asserts.h"

template<typename T>
Gordian::TOptional<T>::TOptional()
{
	_OptionalValue = nullptr;
}

template<typename T>
Gordian::TOptional<T>::TOptional(const T& Value)
{
	Set(Value);
}

template<typename T>
Gordian::TOptional<T>::TOptional(const Gordian::TOptional<T>& CopySource)
{
	Clear();

	if (CopySource.IsSet())
	{
		Set(CopySource._OptionalValue);
	}
}

template<typename T>
Gordian::TOptional<T>::TOptional(Gordian::TOptional<T>&& MoveSource)
{
	Clear();

	_OptionalValue = MoveSource._OptionalValue;
	MoveSource._OptionalValue = nullptr;
}

template<typename T>
Gordian::TOptional<T>::~TOptional()
{
	Clear();
}

template<typename T>
bool Gordian::TOptional<T>::IsSet() const
{
	return _OptionalValue != nullptr;
}

template<typename T>
const T& Gordian::TOptional<T>::Get() const
{
	check(IsSet());

	return *_OptionalValue;
}

template<typename T>
T& Gordian::TOptional<T>::Get()
{
	check(IsSet());

	return *_OptionalValue;
}

template<typename T>
void Gordian::TOptional<T>::Set(const T& NewValue)
{
	Clear();

	_OptionalValue = new T(NewValue);
}

template<typename T>
void Gordian::TOptional<T>::Clear()
{
	if (IsSet())
	{
		delete _OptionalValue;
		_OptionalValue = nullptr;
	}
}

