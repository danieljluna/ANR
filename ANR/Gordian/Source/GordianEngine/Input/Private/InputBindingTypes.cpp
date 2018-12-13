// Gordian by Daniel Luna

#include "../Public/InputBindingTypes.h"

#include <cassert>

using namespace Gordian;

FDigitalBinding::FDigitalBinding()
{
	RequiredComboKeys.reset();
	CommandToTrigger = "";
}

bool FDigitalBinding::ShouldInputTriggerBinding(const EGenericInputKey& InputKey, const TBitSet<EComboKey>& ComboKeyState) const
{
	const TBitSet<EComboKey>& MatchingComboKeys = ~RequiredComboKeys | ComboKeyState;

	return (InputKey == TriggerKey) && MatchingComboKeys.all();
}

bool FDigitalBinding::operator<(const FDigitalBinding& Other) const
{
	if (TriggerKey != Other.TriggerKey)
	{
		return TriggerKey < Other.TriggerKey;
	}
	else if (RequiredComboKeys.count() != Other.RequiredComboKeys.count())
	{
		return RequiredComboKeys.count() < Other.RequiredComboKeys.count();
	}
	else if (RequiredComboKeys.to_ulong() != Other.RequiredComboKeys.to_ulong())
	{
		return RequiredComboKeys.to_ulong() != Other.RequiredComboKeys.to_ulong();
	}
	else
	{
		return CommandToTrigger < Other.CommandToTrigger;
	}
}

