// Gordian by Daniel Luna (2019)

#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Window/Event.hpp"

#include "GordianEngine/Containers/Public/TPrefixTree.h"
#include "GordianEngine/Containers/Public/TCircularBuffer.h"

namespace Gordian
{


// Allows the user to input debug commands via string.
// This is intentionally not using the UI functionality to keep its functionality independent 
//	from as many Gordian systems as possible.
class FCommandPrompt : public sf::NonCopyable
					 , public sf::Drawable
{
public:

	virtual ~FCommandPrompt();

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

	// Used to navigate recent commands.
	// Will replace existing text with the recent command selected. The index
	// of the current recent command is reset when a command is digested.
	void TraverseRecentCommands(int DirectionOfTraversal);

private:

	FCommandPrompt();

	// Font used for the prompt display
	sf::Font PromptFont;
	// The drawable text representing the user's input
	sf::Text CurrentInputText;
	// The background rectangle for the entire console
	sf::RectangleShape BackgroundShape;

	// A union of all possible things that could be mapped to console input
	struct FConsoleStringMapping
	{
		
	};

	TPrefixTree<int> TestTrie;

	// List of Recent Digested Commands
	TCircularBuffer<sf::String> PreviousCommands;

	// Current user input string 
	sf::String CurrentInputString;

	// Stores the height of a line of text
	float StringHeight;
	// Stores the distance between the divider line and the text
	float DividerSpacing;
	// Spacing between text and the edge of the window
	float BorderSpacing;

	// Current index when navigating recent commands
	int RecentCommandsIndex;

	// True if the prompt is currently open
	bool bIsPromptOpen;

};	// class FCommandPrompt


};	// namespace Gordian
