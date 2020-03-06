// Gordian by Daniel Luna (2019)

#pragma once

#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"

#include "GordianEngine/World/Public/World.h"

#include "inih/INIReader.h"

namespace sf
{
	class RenderWindow;
}

namespace Gordian
{

class OWorld;
class FInputManager;

/// Logic for the main game loop.
class FEngineLoop : public sf::NonCopyable
{
public:

    FEngineLoop();
	~FEngineLoop();

    /// Initializes the main loop.
	/// @param[in] argc Command line argument count
	/// @param[in] argv Command line argument array
    /// @return An error code where 0 is success and > 0 is a failure.
    sf::Int32 Init(int argc, char** argv);
    /// Advances the main loop once.
    void Tick();
    /// Exits the main loop, clearing any state.
	/// @param[in] bForceImmediate If true, exit is forced immediately
    void Exit(bool bForceImmediate);

    /// If true the loop is currently attempting to terminate
    bool IsRequestingExit() const;
    /// Call to terminate the loop
    void RequestExit();

	bool LoadProject(const char* InProjectPath);

	const sf::Vector2u& GetWindowSize() const;

protected:

	sf::Int32 ParseCommandArgs(int argc, char** argv);

    /// Initializes the game window.
	///	@return Returns an non-zero error codes if relevant.
    sf::Int32 InitializeGameWindow(const char* WindowTitle);

    /// Parse Input received by the local window
    void ParseInput();
	// Dispatch update across objects that care.
	void Tick(const sf::Time& DeltaSeconds);
    // Draws all drawables in the Window then refreshes
    //  Blend Value is passed to drawables which use it to smooth visuals
    //  despite small amounts of TimePendingTickConsumption at render time.
    void Render(const sf::Time& BlendTime);

    // For now, we store the game window here.
    sf::RenderWindow* GameWindow;
	// For now, we store the input manager here.
	// todo: combine the input manager with the render window?
	FInputManager* InputManager;
	// Store a reference to the game world
	OWorld* GameWorld;

    // Tracks time between loop iterations
    sf::Clock TickDurationClock;
    // Timestep to use when updating ticking objects
    sf::Time TickConsumptionStepSize;
    // Tracks time that we have not yet updated with
    sf::Time TimePendingTickConsumption;

    /// If true the loop is currently attempting to terminate
    bool bIsRequestingExit;
};

extern FEngineLoop GEngineLoop;

};
