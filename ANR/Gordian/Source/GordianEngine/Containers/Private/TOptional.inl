// Gordian by Daniel Luna (2019)

#include "GordianEngine/Debug/Public/Asserts.h"

template<typename T>
Gordian::TOptional<T>::TOptional()
{
	_OptionalValue = nullptr;
}

template<typename T>
Gordian::TOptional<T>::TOptional(const T& Value)
	: TOptional<T>()
{
	Set(Value);
}

template<typename T>
Gordian::TOptional<T>::TOptional(const Gordian::TOptional<T>& CopySource)
	: TOptional<T>()
{
	if (CopySource.IsSet())
	{
		Set(CopySource.Get());
	}
}

template<typename T>
Gordian::TOptional<T>::TOptional(Gordian::TOptional<T>&& MoveSource)
	: TOptional<T>()
{
	_OptionalValue = MoveSource._OptionalValue;
	MoveSource._OptionalValue = nullptr;
}

template<typename T>
Gordian::TOptional<T>::~TOptional()
{
	Clear();
}

template<typename T>
Gordian::TOptional<T>& Gordian::TOptional<T>::operator=(const T& Other)
{
	Set(Other);

	return *this;
}

template<typename T>
Gordian::TOptional<T>& Gordian::TOptional<T>::operator=(const Gordian::TOptional<T>& Other)
{
	if (Other.IsSet())
	{
		Set(Other.Get());
	}
	else
	{
		Clear();
	}

	return *this;
}

template<typename T>
bool Gordian::TOptional<T>::operator==(const T& Other) const
{
	if (!IsSet())
	{
		return false;
	}

	if (Get() != Other)
	{
		return false;
	}
	
	return true;
}

template<typename T>
bool Gordian::TOptional<T>::operator==(const Gordian::TOptional<T>& Other) const
{
	if (IsSet() != Other.IsSet())
	{
		return false;
	}

	if (IsSet())
	{
		if (Get() != Other.Get())
		{
			return false;
		}
	}

	return true;
}

template<typename T>
T* Gordian::TOptional<T>::operator->()
{
	return _OptionalValue;
}

template<typename T>
const T* Gordian::TOptional<T>::operator->() const
{
	return _OptionalValue;
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

