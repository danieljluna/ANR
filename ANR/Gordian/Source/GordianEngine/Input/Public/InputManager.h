// Gordian by Daniel Luna (2019)

#pragma once

#include <cassert>
#include <list>
#include <map>
#include <string>

#include <SFML/Window/Event.hpp>

#include "InputBindingTypes.h"
#include "GordianEngine/Core/Public/Object.h"
#include "GordianEngine/Delegates/MulticastDelegate.h"
#include "GordianEngine/Debug/Public/Asserts.h"

namespace Gordian
{


using DigitalDelegate = MulticastDelegate0<void>;

// Translates raw SFML input events to Commands that are then broadcasted via delegates
class FInputManager
{
public:
	FInputManager();

	static FInputManager* Singleton;

	void HandleWindowEvent(sf::Event& EventData);

	// Bind to a digital command using a non-const member function
	template<class X, class Y>
	inline bool BindToDigitalCommand(const FCommand& CommandToBind,
									 const EDigitalEventType& EventType,
									 const Y* ObjectToBindTo,
									 void (X::*FunctionToBind)() const);

	// Bind to a digital command using a non-const member function
	template<class X, class Y>
	inline bool BindToDigitalCommand(const FCommand& CommandToBind,
									 const EDigitalEventType& EventType,
									 Y* ObjectToBindTo,
									 void (X::*FunctionToBind)());

	// Bind to a digital command using a non-const member function
	inline bool BindToDigitalCommand(const FCommand& CommandToBind,
									 const EDigitalEventType& EventType,
									 void (*Function_To_Bind)());

private:

	void ReloadFromConfig();

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
		DigitalDelegate OnPressed;
		DigitalDelegate OnReleased;
	};

	// Maps commands to the delegates they fire.
	std::map<FCommand, FDigitalBroadcaster> _DigitalCommandDelegates;

	bool _bHasGeneratedDelegateMap;
};


};


#include "../Private/InputManager.inl"
