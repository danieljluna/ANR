
template<class X, class Y>
bool Gordian::FInputManager::BindToDigitalCommand(const FCommand& CommandToBind,
												  const EDigitalEventType& EventType,
												  const Y* ObjectToBindTo,
												  void(X::*Function_To_Bind)() const)
{
	const std::map<FCommand, FDigitalBroadcaster>::iterator MatchingCommandPair = _DigitalCommandDelegates.find(CommandToBind);
	if (MatchingCommandPair != _DigitalCommandDelegates.cend())
	{
		switch (EventType)
		{
			case EDigitalEventType::Pressed:
				MatchingCommandPair->second.OnPressed.Bind(ObjectToBindTo, Function_To_Bind);
				return true;
				break;
			case EDigitalEventType::Released:
				MatchingCommandPair->second.OnReleased.Bind(ObjectToBindTo, Function_To_Bind);
				return true;
				break;
			default:
				checkNoEntry();
				break;
		}
	}

	return false;
}

template<class X, class Y>
bool Gordian::FInputManager::BindToDigitalCommand(const FCommand& CommandToBind,
												  const EDigitalEventType& EventType,
												  Y* ObjectToBindTo,
												  void(X::*Function_To_Bind)())
{
	const std::map<FCommand, FDigitalBroadcaster>::iterator MatchingCommandPair = _DigitalCommandDelegates.find(CommandToBind);
	if (MatchingCommandPair != _DigitalCommandDelegates.cend())
	{
		switch (EventType)
		{
			case EDigitalEventType::Pressed:
				MatchingCommandPair->second.OnPressed.Bind(ObjectToBindTo, Function_To_Bind);
				return true;
				break;
			case EDigitalEventType::Released:
				MatchingCommandPair->second.OnReleased.Bind(ObjectToBindTo, Function_To_Bind);
				return true;
				break;
			default:
				checkNoEntry();
				break;
		}
	}

	return false;
}

bool Gordian::FInputManager::BindToDigitalCommand(const FCommand& CommandToBind,
												  const EDigitalEventType& EventType,
												  void(*Function_To_Bind)())
{
	const std::map<FCommand, FDigitalBroadcaster>::iterator MatchingCommandPair = _DigitalCommandDelegates.find(CommandToBind);
	if (MatchingCommandPair != _DigitalCommandDelegates.cend())
	{
		switch (EventType)
		{
			case EDigitalEventType::Pressed:
				MatchingCommandPair->second.OnPressed.Bind(Function_To_Bind);
				return true;
				break;
			case EDigitalEventType::Released:
				MatchingCommandPair->second.OnReleased.Bind(Function_To_Bind);
				return true;
				break;
			default:
				checkNoEntry();
				break;
		}
	}

	return false;
}
