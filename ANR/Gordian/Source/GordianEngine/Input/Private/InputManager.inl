
template <class T, void(T::*TMethod)() const>
bool Gordian::FInputManager::BindToDigitalCommand(const FCommand& CommandToBind,
												  const EDigitalEventType& EventType,
												  const T* ObjectToBindTo)
{
	const std::map<FCommand, FDigitalBroadcaster>::iterator MatchingCommandPair = _DigitalCommandDelegates.find(CommandToBind);
	if (MatchingCommandPair != _DigitalCommandDelegates.cend())
	{
		delegate<void()> ConstDelegate = delegate<void()>::create<T, TMethod>(ObjectToBindTo);

		switch (EventType)
		{
			case EDigitalEventType::Pressed:
				MatchingCommandPair->second.OnPressed += ConstDelegate;
				return true;
				break;
			case EDigitalEventType::Released:
				MatchingCommandPair->second.OnReleased += ConstDelegate;
				return true;
				break;
			default:
				checkNoEntry();
				break;
		}
	}

	return false;
}

template <class T, void(T::*TMethod)()>
bool Gordian::FInputManager::BindToDigitalCommand(const FCommand& CommandToBind,
												  const EDigitalEventType& EventType,
												  T* ObjectToBindTo)
{
	const std::map<FCommand, std::shared_ptr<FDigitalBroadcaster>>::iterator MatchingCommandPair = _DigitalCommandDelegates.find(CommandToBind);
	if (MatchingCommandPair != _DigitalCommandDelegates.cend())
	{
		delegate<void()> Delegate = delegate<void()>::create<T, TMethod>(ObjectToBindTo);
		check(MatchingCommandPair->second != nullptr);
		switch (EventType)
		{
			case EDigitalEventType::Pressed:
				MatchingCommandPair->second->OnPressed += Delegate;
				return true;
				break;
			case EDigitalEventType::Released:
				MatchingCommandPair->second->OnReleased += Delegate;
				return true;
				break;
			default:
				checkNoEntry();
				break;
		}
	}

	return false;
}

template <void(*TMethod)()>
bool Gordian::FInputManager::BindToDigitalCommand(const FCommand& CommandToBind,
												  const EDigitalEventType& EventType)
{
	const std::map<FCommand, FDigitalBroadcaster>::iterator MatchingCommandPair = _DigitalCommandDelegates.find(CommandToBind);
	if (MatchingCommandPair != _DigitalCommandDelegates.cend())
	{
		delegate<void()> StaticDelegate = delegate<void()>::create<TMethod>();

		switch (EventType)
		{
			case EDigitalEventType::Pressed:
				MatchingCommandPair->second.OnPressed += StaticDelegate;
				return true;
				break;
			case EDigitalEventType::Released:
				MatchingCommandPair->second.OnReleased += StaticDelegate;
				return true;
				break;
			default:
				checkNoEntry();
				break;
		}
	}

	return false;
}
