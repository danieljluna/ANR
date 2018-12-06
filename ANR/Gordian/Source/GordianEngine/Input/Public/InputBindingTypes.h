// Gordian by Daniel Luna

#pragma once

#include <SFML/System/Vector2.hpp>

#include "GordianEngine/BitSet/Public/BitSet.h"
#include "GordianEngine/Input/Public/InputKeys.h"

namespace Gordian
{


// All the different ways to bind to a digital input
enum DigitalEventType
{
	Pressed,
	Released,
};


// Manages all settings for a digital binding.
// Digital bindings 
struct FDigitalBinding
{
	FDigitalBinding();

	// Returns true if the key and combo keys for the event matches this binding.
	// Does not check that the event type is appropriate for the key type! It is
	//	expected that the caller can already prove this.
	bool DoesEventMatch(const sf::Event& TestEvent) const;

	// Denotes which key this binding listens for
	EGenericInputKey TriggerKey;

	// List of all possible combo keys
	enum EComboKey
	{
		Alt,
		Control,
		Shift,
		System,
		MAX_VALUE
	};

	// Bitmask of required combo keys.
	// Combo keys are ignored in release events
	TBitSet<EComboKey> RequiredComboKeys;
};

	
};
