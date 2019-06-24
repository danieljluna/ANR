// Gordian by Daniel Luna (2019)

#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Window/Event.hpp"

#include "GordianEngine/Containers/Public/PrefixTree.h"

namespace Gordian
{


// Allows the user to input debug commands via string.
// This is intentionally not using the UI functionality to keep its functionality independent 
//	from as many Gordian systems as possible.
class FCommandPrompt : public sf::NonCopyable
					 , public sf::Drawable
{
public:

	static FCommandPrompt& Get();

	// Binds to input and sets up visuals
	void Initialize();

	// Handles input straight from the engine.
	// Returns true if the input should be consumed.
	bool ParseRawInput(const sf::Event& RawInput);

	// Returns whether or not the prompt is open
	bool IsOpen() const;

	// Draw override for the prompt.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:

	// Toggles the command prompt between open and closed
	void Toggle();

	// Open the command prompt to user input and make it visible
	void Open();

	// Close the command prompt to user input and hide it.
	void Close();

	// Digests the CurrentInputString into actual function commands
	void DigestCommand();

	// Setter for current input string
	void SetCurrentInputString(const sf::String& NewInputString);
	// Appends a character to the current input string
	void AppendCurrentInputString(const sf::Uint32& UnicodeValue);

private:

	FCommandPrompt();

	// Font used for the prompt display
	sf::Font PromptFont;
	// The drawable text representing the user's input
	sf::Text CurrentInputText;
	// The background rectangle for the entire console
	sf::RectangleShape BackgroundShape;

	TPrefixTree<int> TestTrie;

	// Will eventually be used to read / cache previous commands so 
	FILE* PreviousCommandsFile;
	// Current user input string 
	sf::String CurrentInputString;
	// Stores the height of a line of text
	float StringHeight;
	// Stores the distance between the divider line and the text
	float DividerSpacing;
	// Spacing between text and the edge of the window
	float BorderSpacing;
	// True if the prompt is currently open
	bool bIsPromptOpen;

};	// class FCommandPrompt


};	// namespace Gordian
