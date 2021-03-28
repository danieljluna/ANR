// Gordian by Daniel Luna (2019)

#pragma once

#include <cassert>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>

#include <SFML/Window/Event.hpp>

#include "InputBindingTypes.h"
#include "GordianEngine/Core/Public/Object.h"
#include "GordianEngine/Delegates/MulticastDelegate.h"
#include "GordianEngine/Debug/Public/Asserts.h"

namespace Gordian
{


using DigitalDelegate = multicast_delegate<void(void)>;

// Translates raw SFML input events to Commands that are then broadcasted via delegates
class FInputManager
{
public:
	FInputManager();

	// Temp static fn for finding an input handler.
	static FInputManager* Get();

	void HandleWindowEvent(sf::Event& EventData);

	// Bind to a digital command using a const member function
	template <class T, void(T::*TMethod)() const>
	inline bool BindToDigitalCommand(const FCommand& CommandToBind,
									 const EDigitalEventType& EventType,
									 const T* ObjectToBindTo);

	// Bind to a digital command using a non-const member function
	template <class T, void(T::*TMethod)()>
	inline bool BindToDigitalCommand(const FCommand& CommandToBind,
									 const EDigitalEventType& EventType,
									 T* ObjectToBindTo);

	// Bind to a digital command using a non-member function
	template <void(*TMethod)()>
	inline bool BindToDigitalCommand(const FCommand& CommandToBind,
									 const EDigitalEventType& EventType);

private:

	static FInputManager* Singleton;

	// Given a generic key, returns commands to trigger.
	// Loaded from an ini file
	std::set<FDigitalBinding> DigitalBindingSet;

	// Runtime state of all combo keys
	TBitSet<EComboKey> _DigitalComboKeyState;

	void GenerateCommandDelegates();

	// Call when key state might have changed. Do not pass in non-key sf::events.
	void CheckForComboKeyStateChange(const sf::Event& EventData);

	void HandleDigitalEvent(const sf::Event& EventData);

	bool IsDigitalEvent(const sf::Event& EventData) const;
	EDigitalEventType GetDigitalEventType(const sf::Event& EventData) const;

	void TriggerDigitalCommand(const FCommand& CommandToTrigger, EDigitalEventType EventType);

	struct FDigitalBroadcaster
	{
		// Defined so an empty initializer list can be used to emplace this into maps
		FDigitalBroadcaster(int) {}

		DigitalDelegate OnPressed;
		DigitalDelegate OnReleased;
	};

	// Maps commands to the delegates they fire.
	std::map<FCommand, std::shared_ptr<FDigitalBroadcaster>> _DigitalCommandDelegates;
	std::map<FCommand, std::shared_ptr<int>> _A;

	bool _bHasGeneratedDelegateMap;
};


};


#include "../Private/InputManager.inl"
