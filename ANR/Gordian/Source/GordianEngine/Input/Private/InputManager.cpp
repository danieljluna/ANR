// Gordian by Daniel Luna

#include "../Public/InputManager.h"

#include <cassert>

using namespace Gordian;

FInputManager::FInputManager()
{

}

void FInputManager::HandleWindowEvent(sf::Event& EventData)
{
	switch (EventData.type)
	{
		// Handle all Digital Input
		case sf::Event::KeyPressed:
			break;
		case sf::Event::KeyReleased:
			break;
		case sf::Event::TextEntered:
			break;
		case sf::Event::MouseButtonPressed:
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::JoystickButtonPressed:
			break;
		case sf::Event::JoystickButtonReleased:
			break;
		case sf::Event::TouchBegan:
			break;
		case sf::Event::TouchEnded:
			break;

		// Handle all Analog / Axial Input
		case sf::Event::MouseMoved:
			break;
		case sf::Event::MouseWheelScrolled:
			break;
		case sf::Event::JoystickMoved:
			break;
		case sf::Event::TouchMoved:
			break;
		case sf::Event::SensorChanged:
			break;

		// Handle Window State Changes
		case sf::Event::LostFocus:
			break;
		case sf::Event::GainedFocus:
			break;
		case sf::Event::MouseEntered:
			break;
		case sf::Event::MouseLeft:
			break;
		case sf::Event::JoystickConnected:
			break;
		case sf::Event::JoystickDisconnected:
			break;

		default:
			assert(false);
			break;
	}
}
