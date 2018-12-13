// Gordian by Daniel Luna

#pragma once

#include <SFML/Window/Event.hpp>

namespace Gordian
{


namespace InputKeys
{
	enum class EInputKeyTypes
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
	enum class EGamepadButtons
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

	InputKeys::EGamepadButtons ConvertJoystickKeyCodeToGamepadButton(unsigned int JoystickKeyCode);
};


// A compound enum class that converts to all input key types
class EGenericInputKey
{
public:

	EGenericInputKey();
	EGenericInputKey(const EGenericInputKey& InGenericKey);

	EGenericInputKey(const InputKeys::EKeyboardKeys& InKeyboardKey);
	EGenericInputKey(const InputKeys::EMouseButtons& InMouseKey);
	EGenericInputKey(const InputKeys::EGamepadButtons& InGamepadKey);
	EGenericInputKey(const sf::Event& InInputEvent);

	operator InputKeys::EKeyboardKeys() const;
	operator InputKeys::EMouseButtons() const;
	operator InputKeys::EGamepadButtons() const;

	const EGenericInputKey& operator=(const EGenericInputKey& Other);
	const EGenericInputKey& operator=(const InputKeys::EKeyboardKeys& InKeyboardKey);
	const EGenericInputKey& operator=(const InputKeys::EMouseButtons& InMouseKey);
	const EGenericInputKey& operator=(const InputKeys::EGamepadButtons& InGamepadKey);
	const EGenericInputKey& operator=(const sf::Event& InInputEvent);

	const InputKeys::EInputKeyTypes& GetKeyType() const;

	bool operator==(const EGenericInputKey& Other) const;
	bool operator!=(const EGenericInputKey& Other) const;
	bool operator<(const EGenericInputKey& Other) const;

	bool IsValid() const;

private:
	
	InputKeys::EInputKeyTypes _KeyType;

	union
	{
		InputKeys::EKeyboardKeys _KeyboardKey;
		InputKeys::EMouseButtons _MouseKey;
		unsigned int _GamepadKey;
	};
};


};
