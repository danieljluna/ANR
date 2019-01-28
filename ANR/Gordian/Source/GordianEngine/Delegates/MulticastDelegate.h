// Gordian by Daniel Luna (2019)

#pragma once

#include "FastDelegate.h"

#include <set>

namespace Gordian
{


// A Broadcaster allows multiple functions from all walks of life with matching parameter types 
//	to bind to this object and then get called whenever the Broadcaster fires.

// Zero-Parameter Broadcaster.
template<class RetType = fastdelegate::detail::DefaultVoid>
class MulticastDelegate0
{
private:

	typedef typename fastdelegate::detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
	using DelegateType = fastdelegate::FastDelegate0<RetType>;

public:

	// Binds a non-const member function to this delegate for the given object
	// Be sure to unbind when the object is destroyed!
	template<class X, class Y>
	inline void Bind(Y *object_to_bind, DesiredRetType(X::* function_to_bind)())
	{
		_DelegateSet.emplace(object_to_bind, function_to_bind);
	}

	// Binds a const member function to this delegate for the given object
	// Be sure to unbind when the object is destroyed!
	template < class X, class Y >
	inline void Bind(const Y *object_to_bind, DesiredRetType(X::* function_to_bind)() const)
	{
		_DelegateSet.emplace(object_to_bind, function_to_bind);
	}

	// Binds a static function to this delegate
	inline void Bind(DesiredRetType(*function_to_bind)())
	{
		_DelegateSet.emplace(function_to_bind);
	}

	// Removes the given object's binding of a non-const member function to this delegate (if it exists)
	template<class X, class Y>
	inline void Unbind(Y *object_to_unbind, DesiredRetType(X::* function_to_unbind)())
	{
		_DelegateSet.erase(DelegateType(object_to_unbind, function_to_unbind));
	}

	// Removes the given object's binding of a const member function to this delegate (if it exists)
	template < class X, class Y >
	inline void Unbind(const Y *object_to_unbind, DesiredRetType(X::* function_to_unbind)() const)
	{
		_DelegateSet.erase(DelegateType(object_to_unbind, function_to_unbind));
	}

	// Removes the binding of the given static function to this delegate (if it exists)
	inline void Unbind(DesiredRetType(*function_to_unbind)())
	{
		_DelegateSet.erase(DelegateType(function_to_unbind));
	}

	// Checks if the given object has bound the given non-const member function to this delegate
	template<class X, class Y>
	inline bool IsBound(Y *object_to_check, DesiredRetType(X::* function_to_check)()) const
	{
		return _DelegateSet.contains(DelegateType(object_to_check, function_to_check));
	}

	// Checks if the given object has bound the given const-member function to this delegate
	template < class X, class Y >
	inline bool IsBound(const Y *object_to_check, DesiredRetType(X::* function_to_check)() const) const
	{
		return _DelegateSet.contains(DelegateType(object_to_check, function_to_check));
	}

	// Checks if a static function is bound
	inline bool IsBound(DesiredRetType(*function_to_check)()) const
	{
		return _DelegateSet.contains(DelegateType(function_to_check));
	}

	// Broadcasts to all bound functions
	inline void Broadcast()
	{
		using ReverseIteratorType = std::set<DelegateType>::reverse_iterator;
		for (ReverseIteratorType it = _DelegateSet.rbegin(); it != _DelegateSet.rend();)
		{
			const DelegateType& DelegateAtIndex = *it;
			if (DelegateAtIndex.empty())
			{
				_DelegateSet.erase(std::next(it).base());
			}
			else
			{
				// For now we just ignore return values
				DelegateAtIndex();
				++it;
			}
		}
	}

private:

	std::set<DelegateType> _DelegateSet;
};


// One-Parameter Broadcaster
template<class Param1, class RetType = fastdelegate::detail::DefaultVoid>
class MulticastDelegate1
{
private:

	typedef typename fastdelegate::detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
	using DelegateType = fastdelegate::FastDelegate1<Param1, RetType>;

public:

	// Binds a non-const member function to this delegate for the given object
	// Be sure to unbind when the object is destroyed!
	template<class X, class Y>
	inline void Bind(Y *object_to_bind, DesiredRetType(X::* function_to_bind)())
	{
		_DelegateSet.emplace(object_to_bind, function_to_bind);
	}

	// Binds a const member function to this delegate for the given object
	// Be sure to unbind when the object is destroyed!
	template < class X, class Y >
	inline void Bind(const Y *object_to_bind, DesiredRetType(X::* function_to_bind)() const)
	{
		_DelegateSet.emplace(object_to_bind, function_to_bind);
	}

	// Binds a static function to this delegate
	inline void Bind(DesiredRetType(*function_to_bind)())
	{
		_DelegateSet.emplace(function_to_bind);
	}

	// Removes the given object's binding of a non-const member function to this delegate (if it exists)
	template<class X, class Y>
	inline void Unbind(Y *object_to_unbind, DesiredRetType(X::* function_to_unbind)())
	{
		_DelegateSet.erase(DelegateType(object_to_unbind, function_to_unbind));
	}

	// Removes the given object's binding of a const member function to this delegate (if it exists)
	template < class X, class Y >
	inline void Unbind(const Y *object_to_unbind, DesiredRetType(X::* function_to_unbind)() const)
	{
		_DelegateSet.erase(DelegateType(object_to_unbind, function_to_unbind));
	}

	// Removes the binding of the given static function to this delegate (if it exists)
	inline void Unbind(DesiredRetType(*function_to_unbind)())
	{
		_DelegateSet.erase(DelegateType(function_to_unbind));
	}

	// Checks if the given object has bound the given non-const member function to this delegate
	template<class X, class Y>
	inline bool IsBound(Y *object_to_check, DesiredRetType(X::* function_to_check)()) const
	{
		return _DelegateSet.contains(DelegateType(object_to_check, function_to_check));
	}

	// Checks if the given object has bound the given const-member function to this delegate
	template < class X, class Y >
	inline bool IsBound(const Y *object_to_check, DesiredRetType(X::* function_to_check)() const) const
	{
		return _DelegateSet.contains(DelegateType(object_to_check, function_to_check));
	}

	// Checks if a static function is bound
	inline bool IsBound(DesiredRetType(*function_to_check)()) const
	{
		return _DelegateSet.contains(DelegateType(function_to_check));
	}

	// Broadcasts to all bound functions
	inline void Broadcast(Param1 Param1_Value) const
	{
		for (auto it = _DelegateSet.rbegin(); it != _DelegateSet.rend();)
		{
			DelegateType& DelegateAtIndex = *it;
			if (DelegateAtIndex.empty())
			{
				const auto DelegateToRemove = it;
				++it;
				_DelegateSet.erase(DelegateToRemove);
			}
			else
			{
				// For now we just ignore return values
				DelegateAtIndex(Param1_Value);
				++it;
			}
		}
	}

private:

	std::set<DelegateType> _DelegateSet;
};



};
