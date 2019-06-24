// Gordian by Daniel Luna (2019)

#include "../Public/CommandPrompt.h"

#include "SFML/Graphics/RenderTarget.hpp"

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/Logging.h"
#include "GordianEngine/Core/Public/EngineLoop.h"
#include "GordianEngine/Input/Public/InputKeys.h"

using namespace Gordian;

namespace
{
	// Console settings
	static const char* k_PreviousCommandsFilepath = "Saved/PreviousCommands.txt";
	static const char* k_CommandPromptFontFilepath = "Resources/Default/CommandPrompt.ttf";
	static const unsigned int k_CommandPromptFontSize = 16;
	static const sf::Uint16 k_ConsoleBackgroundAlpha = 127;

	// Important keycodes
	static const sf::Uint16 k_BackspaceUnicode = 8;
	static const sf::Uint16 k_EnterUnicode = 14;
	static const sf::Uint16 k_SpaceUnicode = 32;
	static const sf::Uint16 k_GraveAccentUnicode = 96;
	static const sf::Uint16 k_TildeUnicode = 126;
}

FCommandPrompt::FCommandPrompt()
{
	TestTrie.Reserve(15);
	CurrentInputString = "";
	errno_t ErrorCode = 0;
	ErrorCode = fopen_s(&PreviousCommandsFile, k_PreviousCommandsFilepath, "r");
	if (ErrorCode != 0)
	{
		GE_LOG(LogFileIO, Log, "PreviousCommands file could not be found or does not exist. (Code: %d)", ErrorCode);
		PreviousCommandsFile = nullptr;
	}

	if (!PromptFont.loadFromFile(k_CommandPromptFontFilepath))
	{
		GE_LOG(LogFileIO, Fatal, "CommandPrompt could not find it's font %s to load!", k_CommandPromptFontFilepath)
	}

	
	// Temporarily use a full sized character so we can better estimate the size
	CurrentInputText.setString("Q");
	CurrentInputText.setCharacterSize(k_CommandPromptFontSize);
	CurrentInputText.setFont(PromptFont);

	// Set up the Text
	const sf::FloatRect& TextBounds = CurrentInputText.getGlobalBounds();
	StringHeight = TextBounds.top + TextBounds.height;
	DividerSpacing = TextBounds.top;
	BorderSpacing = 0.5f * TextBounds.top;
	CurrentInputText.setPosition(BorderSpacing, -(StringHeight + BorderSpacing));
	CurrentInputText.setFillColor(sf::Color::White);
	bIsPromptOpen = false;

	// Set up the Background
	BackgroundShape.setFillColor(sf::Color(0, 0, 0, k_ConsoleBackgroundAlpha));
	BackgroundShape.setOutlineColor(sf::Color::White);
	BackgroundShape.setOutlineThickness(1.f);
	BackgroundShape.setPosition(-DividerSpacing, -(StringHeight + BorderSpacing + DividerSpacing));
	// Note the width is 1: it should be scaled when drawn so it is as wide as the window.
	BackgroundShape.setSize(sf::Vector2f(10, (StringHeight + BorderSpacing + 2*DividerSpacing)));

	SetCurrentInputString("");
}

/*static*/ FCommandPrompt& FCommandPrompt::Get()
{
	static FCommandPrompt Singleton;

	return Singleton;
}

bool FCommandPrompt::IsOpen() const
{
	return bIsPromptOpen;
}

void FCommandPrompt::Initialize()
{

}

bool FCommandPrompt::ParseRawInput(const sf::Event& RawInput)
{
	if (RawInput.type == sf::Event::KeyPressed)
		//&& RawInput.key.code == sf::Keyboard::Tilde)
	{
		const sf::Keyboard::Key& InputKey = RawInput.key.code;
		if (InputKey == InputKeys::EKeyboardKeys::Tilde)
		{
			Toggle();
			return true;
		}
		else if (InputKey == InputKeys::EKeyboardKeys::Enter)
		{
			DigestCommand();
			return true;
		}
	}

	if (!bIsPromptOpen)
	{
		return false;
	}

	if (RawInput.type == sf::Event::TextEntered)
	{
		AppendCurrentInputString(RawInput.text.unicode);
	}

	return true;
}

void FCommandPrompt::Toggle()
{
	bIsPromptOpen ? Close() : Open();
}

void FCommandPrompt::Open()
{
	bIsPromptOpen = true;
}

void FCommandPrompt::Close()
{
	bIsPromptOpen = false;
}

void FCommandPrompt::DigestCommand()
{
	TestTrie.Insert(CurrentInputString, CurrentInputString.getSize());

	SetCurrentInputString("");
}

void FCommandPrompt::SetCurrentInputString(const sf::String& NewInputString)
{
	CurrentInputString = NewInputString;

	// Update Visualization
	CurrentInputText.setString(CurrentInputString + "_");
}

void FCommandPrompt::AppendCurrentInputString(const sf::Uint32& UnicodeValue)
{
	if (UnicodeValue == k_BackspaceUnicode)
	{
		const std::size_t StringSize = CurrentInputString.getSize();
		if (StringSize > 0)
		{
			CurrentInputString.erase(StringSize - 1);
		}
	}
	else if (UnicodeValue >= k_SpaceUnicode
			 && UnicodeValue < k_TildeUnicode
			 && UnicodeValue != k_GraveAccentUnicode)
	{
		CurrentInputString += UnicodeValue;
	}
	else
	{
		return;
	}

	// Update Visualization
	CurrentInputText.setString(CurrentInputString + "_");
}

void FCommandPrompt::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!bIsPromptOpen)
	{
		return;
	}

	const float RenderTargetWidth = float(target.getSize().x);
	const float RenderTargetHeight = float(target.getSize().y);
	states.transform.translate(0.0, RenderTargetHeight);

	// Draw background rectangle
	{
		sf::RenderStates BackgroundRenderStates = states;
		BackgroundRenderStates.transform.scale(RenderTargetWidth, 1.0f);
		target.draw(BackgroundShape, BackgroundRenderStates);
	}

	// Draw text input
	const sf::FloatRect& TextBounds = CurrentInputText.getGlobalBounds();
	// If text is wider than the target width
	if (TextBounds.left + TextBounds.width > RenderTargetWidth - 2 * BorderSpacing)
	{
		// Transform text so the last input is always on screen.
		sf::RenderStates InputTextRenderStates = states;
		InputTextRenderStates.transform.translate(RenderTargetWidth - 2 * BorderSpacing - TextBounds.width - TextBounds.left, 0);
		target.draw(CurrentInputText, InputTextRenderStates);
	}
	else
	{
		target.draw(CurrentInputText, states);
	}
}
