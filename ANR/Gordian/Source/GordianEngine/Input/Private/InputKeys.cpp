// Gordian by Daniel Luna

#include "../Public/InputKeys.h"

#include <cassert>

using namespace Gordian;

EGenericInputKey::EGenericInputKey()
{
	_KeyType = InputKeys::EInputKeyTypes::Unknown;
	_KeyboardKey = InputKeys::EKeyboardKeys::KeyCount;
}

EGenericInputKey::EGenericInputKey(const EGenericInputKey& InGenericKey)
{
	*this = InGenericKey;
}

EGenericInputKey::EGenericInputKey(const InputKeys::EKeyboardKeys& InKeyboardKey)
{
	*this = InKeyboardKey;
}

EGenericInputKey::EGenericInputKey(const InputKeys::EMouseButtons& InMouseKey)
{
	*this = InMouseKey;
}

EGenericInputKey::EGenericInputKey(const InputKeys::EGamepadButtons& InGamepadKey)
{
	*this = InGamepadKey;
}

EGenericInputKey::EGenericInputKey(const sf::Event& InInputEvent)
{
	*this = InInputEvent;
}

EGenericInputKey::operator InputKeys::EKeyboardKeys() const
{
	if (_KeyType == InputKeys::EInputKeyTypes::Keyboard)
	{
		return _KeyboardKey;
	}
	else
	{
		return InputKeys::EKeyboardKeys::KeyCount;
	}
}

EGenericInputKey::operator InputKeys::EMouseButtons() const
{
	if (_KeyType == InputKeys::EInputKeyTypes::Mouse)
	{
		return _MouseKey;
	}
	else
	{
		return InputKeys::EMouseButtons::ButtonCount;
	}
}

EGenericInputKey::operator InputKeys::EGamepadButtons() const
{
	if (_KeyType == InputKeys::EInputKeyTypes::Gamepad)
	{
		return InputKeys::ConvertJoystickKeyCodeToGamepadButton(_GamepadKey);
	}
	else
	{
		return InputKeys::EGamepadButtons::ButtonCount;
	}
}

const EGenericInputKey& EGenericInputKey::operator=(const EGenericInputKey& Other)
{
	_KeyType = Other._KeyType;
	switch (_KeyType)
	{
		case InputKeys::EInputKeyTypes::Keyboard:
			_KeyboardKey = Other._KeyboardKey;
			break;
		case InputKeys::EInputKeyTypes::Mouse:
			_MouseKey = Other._MouseKey;
			break;
		case InputKeys::EInputKeyTypes::Gamepad:
			_GamepadKey = Other._GamepadKey;
			break;
		case InputKeys::EInputKeyTypes::Unknown:
			_KeyboardKey = InputKeys::EKeyboardKeys::Unknown;
			break;
		default:
			assert(false);
			break;
	}

	return *this;
}

const EGenericInputKey& EGenericInputKey::operator=(const InputKeys::EKeyboardKeys& InKeyboardKey)
{
	_KeyType = InputKeys::EInputKeyTypes::Keyboard;
	_KeyboardKey = InKeyboardKey;
	return *this;
}

const EGenericInputKey& EGenericInputKey::operator=(const InputKeys::EMouseButtons& InMouseKey)
{
	_KeyType = InputKeys::EInputKeyTypes::Mouse;
	_MouseKey = InMouseKey;
	return *this;
}

const EGenericInputKey& EGenericInputKey::operator=(const InputKeys::EGamepadButtons& InGamepadKey)
{
	_KeyType = InputKeys::EInputKeyTypes::Gamepad;
	_GamepadKey = static_cast<unsigned int>(InGamepadKey);
	return *this;
}

const EGenericInputKey& EGenericInputKey::operator=(const sf::Event& InInputEvent)
{
	switch (InInputEvent.type)
	{
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			_KeyType = InputKeys::EInputKeyTypes::Keyboard;
			_KeyboardKey = InInputEvent.key.code;
			break;
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
			_KeyType = InputKeys::EInputKeyTypes::Mouse;
			_MouseKey = InInputEvent.mouseButton.button;
			break;
		case sf::Event::JoystickButtonPressed:
		case sf::Event::JoystickButtonReleased:
			_KeyType = InputKeys::EInputKeyTypes::Gamepad;
			_GamepadKey = InInputEvent.joystickButton.button;
			break;
		default:
			_KeyType = InputKeys::EInputKeyTypes::Unknown;
			break;
	}

	return *this;
}

const InputKeys::EInputKeyTypes& EGenericInputKey::GetKeyType() const
{
	return _KeyType;
}

bool EGenericInputKey::operator==(const EGenericInputKey& Other) const
{
	bool bDoesKeyMatch = false;

	if (_KeyType == Other._KeyType)
	{
		switch (_KeyType)
		{
			case InputKeys::EInputKeyTypes::Keyboard:
				bDoesKeyMatch = _KeyboardKey == Other._KeyboardKey;
				break;
			case InputKeys::EInputKeyTypes::Mouse:
				bDoesKeyMatch = _MouseKey == Other._MouseKey;
				break;
			case InputKeys::EInputKeyTypes::Gamepad:
				bDoesKeyMatch = _GamepadKey == Other._GamepadKey;
				break;
			case InputKeys::EInputKeyTypes::Unknown:
				bDoesKeyMatch = true;
				break;
			default:
				assert(false);
				break;
		}
	}

	return bDoesKeyMatch;
}

bool EGenericInputKey::operator!=(const EGenericInputKey& Other) const
{
	return !(*this == Other);
}

bool EGenericInputKey::operator<(const EGenericInputKey& Other) const
{
	if (_KeyType != Other._KeyType)
	{
		return _KeyType < Other._KeyType;
	}
	else
	{
		switch (_KeyType)
		{
			case InputKeys::EInputKeyTypes::Keyboard:
				return _KeyboardKey < Other._KeyboardKey;
				break;
			case InputKeys::EInputKeyTypes::Mouse:
				return _MouseKey < Other._MouseKey;
				break;
			case InputKeys::EInputKeyTypes::Gamepad:
				return _GamepadKey < Other._GamepadKey;
				break;
			case InputKeys::EInputKeyTypes::Unknown:
				return false;
				break;
			default:
				assert(false);
				break;
		}
	}

	return false;
}

bool EGenericInputKey::IsValid() const
{
	bool bIsValid = false;

	switch (_KeyType)
	{
		case InputKeys::EInputKeyTypes::Keyboard:
			bIsValid = _GamepadKey != InputKeys::EKeyboardKeys::KeyCount;
			break;
		case InputKeys::EInputKeyTypes::Mouse:
			bIsValid = _GamepadKey != InputKeys::EMouseButtons::ButtonCount;
			break;
		case InputKeys::EInputKeyTypes::Gamepad:
			bIsValid = _GamepadKey != static_cast<unsigned int>(InputKeys::EGamepadButtons::ButtonCount);
			break;
		case InputKeys::EInputKeyTypes::Unknown:
			bIsValid = false;
			break;
		default:
			assert(false);
			break;
	}

	return bIsValid;
}

InputKeys::EGamepadButtons InputKeys::ConvertJoystickKeyCodeToGamepadButton(unsigned int JoystickKeyCode)
{
	if (JoystickKeyCode >= static_cast<unsigned int>(InputKeys::EGamepadButtons::ButtonCount))
	{
		return InputKeys::EGamepadButtons::ButtonCount;
	}

	return static_cast<InputKeys::EGamepadButtons>(JoystickKeyCode);
}
