// Gordian by Daniel Luna

#pragma once

#include <string>
#include <map>
#include <list>

#include <SFML/Window/Event.hpp>

#include "InputBindingTypes.h"
#include "GordianEngine/Delegates/MulticastDelegate.h"

namespace Gordian
{


using KeyboardDelegate = MulticastDelegate0<void>;

// Defines a Keyboard binding


// Translates raw SFML input events to Commands that are then broadcasted via delegates
class FInputManager
{
public:
	FInputManager();

	void HandleWindowEvent(sf::Event& EventData);

private:
	// A mapping of commands (which are merely identified as strings for now) to digital bindings.
	// This is the way the user will construct the input bindings, but this is now how they will be used in real time.
	std::map<std::string, std::list<FDigitalBinding>> _DigitalCommandMappings;

	TBitSet<FDigitalBinding::EComboKey> _DigitalComboKeyState;
};


};
