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

EGenericInputKey::EGenericInputKey(InputKeys::EKeyboardKeys InKeyboardKey)
{
	*this = InKeyboardKey;
}

EGenericInputKey::EGenericInputKey(InputKeys::EMouseButtons InMouseKey)
{
	*this = InMouseKey;
}

EGenericInputKey::EGenericInputKey(InputKeys::EGamepadButtons InGamepadKey)
{
	*this = InGamepadKey;
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
		return _GamepadKey;
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

const EGenericInputKey& EGenericInputKey::operator=(const InputKeys::EKeyboardKeys& Other)
{
	_KeyType = InputKeys::EInputKeyTypes::Keyboard;
	_KeyboardKey = Other;
	return *this;
}

const EGenericInputKey& EGenericInputKey::operator=(const InputKeys::EMouseButtons& Other)
{
	_KeyType = InputKeys::EInputKeyTypes::Mouse;
	_MouseKey = Other;
	return *this;
}

const EGenericInputKey& EGenericInputKey::operator=(const InputKeys::EGamepadButtons& Other)
{
	_KeyType = InputKeys::EInputKeyTypes::Gamepad;
	_GamepadKey = Other;
	return *this;
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
				bDoesKeyMatch = _KeyboardKey == InputKeys::EKeyboardKeys::KeyCount;
				break;
			default:
				assert(false);
				break;
		}
	}

	return bDoesKeyMatch;
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
			bIsValid = _GamepadKey != InputKeys::EGamepadButtons::ButtonCount;
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
