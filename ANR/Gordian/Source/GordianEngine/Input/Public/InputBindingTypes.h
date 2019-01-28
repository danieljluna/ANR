// Gordian by Daniel Luna (2019)

#pragma once

#include <SFML/System/Vector2.hpp>

#include "GordianEngine/BitSet/Public/BitSet.h"
#include "GordianEngine/Input/Public/InputKeys.h"

namespace Gordian
{


// List of all possible combo keys
enum class EComboKey : sf::Uint8
{
	Alt,
	Control,
	Shift,
	System,
	MAX_VALUE
};


// All the different ways to bind to a digital input
enum class EDigitalEventType : sf::Uint8
{
	Pressed,
	Released,
	MAX_VALUE
};

using FCommand = std::string;


// Manages all settings for a digital binding.
// Digital bindings pass no values when triggered
struct FDigitalBinding
{
	FDigitalBinding();

	// Denotes which key this binding listens for
	EGenericInputKey TriggerKey;

	// Bitmask of required combo keys.
	// Combo keys are ignored in release events
	TBitSet<EComboKey> RequiredComboKeys;

	// Command this binding should trigger
	FCommand CommandToTrigger;

	bool ShouldInputTriggerBinding(const EGenericInputKey& InputKey, const TBitSet<EComboKey>& ComboKeyState) const;

	bool operator<(const FDigitalBinding& Other) const;
};



};
