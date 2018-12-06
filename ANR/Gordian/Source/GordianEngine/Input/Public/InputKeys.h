// Gordian by Daniel Luna

#pragma once

#include <SFML/Window/Event.hpp>

namespace Gordian
{


namespace InputKeys
{
	enum EInputKeyTypes
	{
		Keyboard,
		Mouse,
		Gamepad,
		Unknown
	};

	using EKeyboardKeys = sf::Keyboard::Key;
	using EMouseButtons = sf::Mouse::Button;
	// Typical Joystick Buttons
	// todo: split into XBOX and generic
	enum EGamepadButtons
	{
		DPad_Up,
		DPad_Right,
		DPad_Down,
		DPad_Left,
		FaceButton_Start,
		FaceButton_Back,
		FaceButton_Top,
		FaceButton_Right,
		FaceButton_Bottom,
		FaceButton_Left,
		Trigger_Left,
		Trigger_Right,
		Bumper_Left,
		Bumper_Right,
		ButtonCount,
		// Due to sfml limits, a joystick can only have this many registered buttons
		MaxButtons = sf::Joystick::ButtonCount
	};
};


// A compound enum class that converts to all input key types
class EGenericInputKey
{
public:

	EGenericInputKey();
	EGenericInputKey(const EGenericInputKey& InGenericKey);

	EGenericInputKey(InputKeys::EKeyboardKeys InKeyboardKey);
	EGenericInputKey(InputKeys::EMouseButtons InMouseKey);
	EGenericInputKey(InputKeys::EGamepadButtons InGamepadKey);

	operator InputKeys::EKeyboardKeys() const;
	operator InputKeys::EMouseButtons() const;
	operator InputKeys::EGamepadButtons() const;

	const EGenericInputKey& operator=(const EGenericInputKey& Other);
	const EGenericInputKey& operator=(const InputKeys::EKeyboardKeys& Other);
	const EGenericInputKey& operator=(const InputKeys::EMouseButtons& Other);
	const EGenericInputKey& operator=(const InputKeys::EGamepadButtons& Other);

	bool operator==(const EGenericInputKey& Other) const;

	bool IsValid() const;

private:
	
	InputKeys::EInputKeyTypes _KeyType;

	union
	{
		InputKeys::EKeyboardKeys _KeyboardKey;
		InputKeys::EMouseButtons _MouseKey;
		InputKeys::EGamepadButtons _GamepadKey;
	};
};


};
