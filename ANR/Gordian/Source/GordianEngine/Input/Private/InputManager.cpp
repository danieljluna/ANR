// Gordian by Daniel Luna (2019)

#include "../Public/InputManager.h"

#include "GordianEngine/Debug/Public/Asserts.h"

using namespace Gordian;

FInputManager* FInputManager::Singleton = nullptr;

FInputManager::FInputManager()
{
	FDigitalBinding TempBinding;
	TempBinding.CommandToTrigger = "TestCommand";
	TempBinding.TriggerKey = InputKeys::EKeyboardKeys::T;
	DigitalBindingSet.insert(TempBinding);
	_bHasGeneratedDelegateMap = false;

	GenerateCommandDelegates();

	if (Singleton == nullptr)
	{
		Singleton = this;
	}
}

/*static*/ FInputManager* FInputManager::Get()
{
	check(Singleton != nullptr);
	return Singleton;
}

void FInputManager::HandleWindowEvent(sf::Event& EventData)
{
	if (!_bHasGeneratedDelegateMap)
	{
		GenerateCommandDelegates();
	}

	switch (EventData.type)
	{
		// Handle Digital Input --------------------------------------------------------------
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			// For key changes, we need to update any changes in combo key state
			CheckForComboKeyStateChange(EventData);
			HandleDigitalEvent(EventData);
			break;
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		case sf::Event::JoystickButtonPressed:
		case sf::Event::JoystickButtonReleased:
			HandleDigitalEvent(EventData);
			break;
		case sf::Event::TextEntered:
			// Text is currently unhandled
			break;
		case sf::Event::TouchBegan:
		case sf::Event::TouchEnded:
			// Touch is currently unhandled
			break;

		// Handle all Analog / Axial Input ---------------------------------------------------------
		case sf::Event::MouseMoved:
			break;
		case sf::Event::MouseWheelMoved:
		case sf::Event::MouseWheelScrolled:
			break;
		case sf::Event::JoystickMoved:
			break;
		case sf::Event::TouchMoved:
			break;
		case sf::Event::SensorChanged:
			break;

		// Handle Window State Changes -------------------------------------------------------------
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
			checkNoEntry();
			break;
	}
}

void FInputManager::GenerateCommandDelegates()
{
	check(!_bHasGeneratedDelegateMap);

	_DigitalCommandDelegates.clear();

	for (const FDigitalBinding& DigitalBinding : DigitalBindingSet)
	{
		std::map<FCommand, std::shared_ptr<FDigitalBroadcaster>>::const_iterator ExistingCommandIt;
		ExistingCommandIt = _DigitalCommandDelegates.find(DigitalBinding.CommandToTrigger);
		if (ExistingCommandIt == _DigitalCommandDelegates.cend())
		{
			_DigitalCommandDelegates.emplace(DigitalBinding.CommandToTrigger, std::make_shared<FDigitalBroadcaster>(0));
		}
	}

	_bHasGeneratedDelegateMap = true;
}

void FInputManager::CheckForComboKeyStateChange(const sf::Event& EventData)
{
	check(EventData.type == sf::Event::KeyPressed
		   || EventData.type == sf::Event::KeyReleased);
	
	switch (EventData.key.code)
	{
		case InputKeys::EKeyboardKeys::LShift:
		case InputKeys::EKeyboardKeys::RShift:
			_DigitalComboKeyState.set(EComboKey::Shift, EventData.key.shift);
			break;
		case InputKeys::EKeyboardKeys::LAlt:
		case InputKeys::EKeyboardKeys::RAlt:
			_DigitalComboKeyState.set(EComboKey::Alt, EventData.key.alt);
			break;
		case InputKeys::EKeyboardKeys::LSystem:
		case InputKeys::EKeyboardKeys::RSystem:
			_DigitalComboKeyState.set(EComboKey::System, EventData.key.system);
			break;
		case InputKeys::EKeyboardKeys::LControl:
		case InputKeys::EKeyboardKeys::RControl:
			_DigitalComboKeyState.set(EComboKey::Control, EventData.key.control);
			break;
		default:
			// Key Change was not a combo key, just eject
			return;
	}
}

void FInputManager::HandleDigitalEvent(const sf::Event& EventData)
{
	check(IsDigitalEvent(EventData));

	// Translate event to a generic key
	EGenericInputKey KeyTranslation(EventData);

	FDigitalBinding SearchBinding;
	SearchBinding.TriggerKey = KeyTranslation;

	using BindingSetItType = std::set<FDigitalBinding>::const_iterator;

	// We start the search at the first element with the same trigger key
	BindingSetItType FirstBindingByKey = DigitalBindingSet.lower_bound(SearchBinding);
	if (FirstBindingByKey != DigitalBindingSet.cend() && FirstBindingByKey->TriggerKey == KeyTranslation)
	{
		// This may not be the only binding that can trigger though. We need to iterate across all possible bindings.
		// Since bindings are sorted by key, then by combo state, we can find the binding with the most 'complicated' 
		// yet possibly triggerable state and start the search there, working back to the simple case.
		SearchBinding.RequiredComboKeys = _DigitalComboKeyState;
		SearchBinding.CommandToTrigger = char(127);
		BindingSetItType LastApplicableBinding = DigitalBindingSet.upper_bound(SearchBinding);

		// We will search across these its in reverse fasion (reverse its seem to be crashing here, so we just do it by hand)
		const BindingSetItType SearchLocationStart = --LastApplicableBinding;
		const BindingSetItType SearchLocationEnd = --FirstBindingByKey;

		// Prefer triggerable bindings with more complex combo key state over those with simpler ones.
		// Eventually we should expand upon input and create Controllers which decide whether or not they consume input. Here we assume all input is consumed
		for (BindingSetItType It = SearchLocationStart; It != SearchLocationEnd; --It)
		{
			if (It->ShouldInputTriggerBinding(KeyTranslation, _DigitalComboKeyState))
			{
				TriggerDigitalCommand(It->CommandToTrigger, GetDigitalEventType(EventData));
				return;
			}
		}
	}

}

bool FInputManager::IsDigitalEvent(const sf::Event& EventData) const
{
	switch (EventData.type)
	{
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		case sf::Event::JoystickButtonPressed:
		case sf::Event::JoystickButtonReleased:
		case sf::Event::TouchBegan:
		case sf::Event::TouchEnded:
			return true;
			break;
		default:
			return false;
			break;
	}
}

EDigitalEventType FInputManager::GetDigitalEventType(const sf::Event& EventData) const
{
	check(IsDigitalEvent(EventData));

	switch (EventData.type)
	{
		case sf::Event::KeyPressed:
		case sf::Event::MouseButtonPressed:
		case sf::Event::JoystickButtonPressed:
		case sf::Event::TouchBegan:
			return EDigitalEventType::Pressed;
			break;
		case sf::Event::KeyReleased:
		case sf::Event::MouseButtonReleased:
		case sf::Event::JoystickButtonReleased:
		case sf::Event::TouchEnded:
			return EDigitalEventType::Released;
			break;
		default:
			return EDigitalEventType::MAX_VALUE;
			break;
	}
}

void FInputManager::TriggerDigitalCommand(const FCommand& CommandToTrigger, EDigitalEventType EventType)
{
	using CommandMapItType = std::map<FCommand, std::shared_ptr<FDigitalBroadcaster>>::iterator;
	CommandMapItType CommandDelegatePair = _DigitalCommandDelegates.find(CommandToTrigger);
	if (CommandDelegatePair != _DigitalCommandDelegates.end())
	{
		check(CommandDelegatePair->second != nullptr);
		switch (EventType)
		{
			case EDigitalEventType::Pressed:
				CommandDelegatePair->second->OnPressed();
				break;
			case EDigitalEventType::Released:
				CommandDelegatePair->second->OnReleased();
				break;
			default:
				checkNoEntry();
				break;
		}
	}
}
