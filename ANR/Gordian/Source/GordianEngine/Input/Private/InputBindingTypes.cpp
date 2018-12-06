// Gordian by Daniel Luna

#include "../Public/InputBindingTypes.h"

#include <cassert>

using namespace Gordian;

FDigitalBinding::FDigitalBinding()
{
	RequiredComboKeys.reset();
	
}

bool FDigitalBinding::DoesEventMatch(const sf::Event& TestEvent) const
{
	bool bDoesKeyMatch = false;
	switch (KeyType)
	{
		case InputKeys::EInputKeyTypes::Keyboard:
			assert(TestEvent.type == sf::Event::KeyPressed
				   || TestEvent.type == sf::Event::KeyReleased);
			bDoesKeyMatch = TestEvent.key.code == KeyboardKey;
			break;
		case InputKeys::EInputKeyTypes::Mouse:
			assert(TestEvent.type == sf::Event::MouseButtonPressed
				   || TestEvent.type == sf::Event::MouseButtonReleased);
			bDoesKeyMatch = TestEvent.mouseButton.button == MouseButton;
			break;
		case InputKeys::EInputKeyTypes::Gamepad:
			assert(TestEvent.type == sf::Event::JoystickButtonPressed
				   || TestEvent.type == sf::Event::JoystickButtonReleased);
			bDoesKeyMatch = TestEvent.joystickButton.button == GamepadButton;
			break;
		default:
			assert(false);
			break;
	}

	if (!bDoesKeyMatch)
	{
		// Early out if the key does not match
		return false;
	}

	TBitSet<EComboKey> TestEventComboKeys;
	TestEventComboKeys.set(EComboKey::Alt, TestEvent.key.alt);
	TestEventComboKeys.set(EComboKey::Control, TestEvent.key.control);
	TestEventComboKeys.set(EComboKey::Shift, TestEvent.key.shift);
	TestEventComboKeys.set(EComboKey::System, TestEvent.key.system);

	return (TestEventComboKeys & RequiredComboKeys) == RequiredComboKeys;
}

