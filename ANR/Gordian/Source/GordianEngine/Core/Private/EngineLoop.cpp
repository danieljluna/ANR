// Gordian by Daniel Luna (2019)

#include "GordianEngine/Core/Public/EngineLoop.h"

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/Logging.h"
#include "GordianEngine/Input/Public/InputManager.h"
#include "GordianEngine/Platform/Public/Platform.h"
#include "GordianEngine/Platform/Public/ConsoleFormatting.h"
#include "GordianEngine/World/Public/World.h"

#include "inih/INIReader.h"

using namespace Gordian;

FEngineLoop::FEngineLoop()
    : GameWindow(nullptr)
	, InputManager(nullptr)
	, GameWorld(nullptr)
    , TickConsumptionStepSize(sf::Time::Zero)
    , TimePendingTickConsumption(sf::Time::Zero)
    , bIsRequestingExit(false)
{
    TickConsumptionStepSize = sf::milliseconds(1000 / 30);
}

FEngineLoop::~FEngineLoop()
{
	// If GameWindow is non-null after main ends, sfml will crash
	check(GameWindow == nullptr && InputManager == nullptr && GameWorld == nullptr);
}

sf::Int32 FEngineLoop::Init()
{
	GE_LOG(LogCore, Log, "Initializing Core Loop...");

	sf::Int32 ErrorCode = 0;

	// Ensure console formatting has been initialized
	ErrorCode = FConsoleFormatting::InitializeFormatting();
	if (ErrorCode != 0)
	{
		GE_LOG(LogCore, Error, "Failed to initialize console formatting, code: %d", ErrorCode);
		return ErrorCode;
	}

    ErrorCode = InitializeGameWindow();
    if (ErrorCode != 0)
    {
        return ErrorCode;
    }

	InputManager = new FInputManager();
	GameWorld = new OWorld("GameWorld", nullptr);
    bIsRequestingExit = false;
    TickDurationClock.restart();

	GE_LOG(LogCore, Log, "Core Loop Initialized Successfully.");
    return ErrorCode;
}

sf::Int32 FEngineLoop::InitializeGameWindow()
{
    // todo: replace this ini proof of concept with more manageable code
	INIReader reader("../Gordian/Config/DefaultEngine.ini");
	check(reader.ParseError() == 0);

	GameWindow = new sf::RenderWindow(sf::VideoMode(reader.GetInteger("Graphics", "WindowWidth", 800), reader.GetInteger("Graphics", "WindowHeight", 600)), "Title");
    if (!GameWindow->isOpen())
    {
		GE_LOG(LogCore, Error, "Failed to open a render window!");
        return 1;
    }

    GameWindow->setVerticalSyncEnabled(true);
    GameWindow->setMouseCursorGrabbed(false);
	GameWindow->setView(GameWindow->getDefaultView());
    return 0;
}

void FEngineLoop::Tick()
{
	check(!bIsRequestingExit);

    ParseInput();

    TimePendingTickConsumption += TickDurationClock.restart();
    while (TimePendingTickConsumption >= TickConsumptionStepSize)
    {
        Tick(TickConsumptionStepSize);

        TimePendingTickConsumption -= TickConsumptionStepSize;
    }

    // In order to smooth motion of objects, we use the leftover time that has
    // not been passed through update to estimate positions of renderable objects.
    const float BlendFactor = TimePendingTickConsumption / TickConsumptionStepSize;
	check(BlendFactor >= 0.f && BlendFactor < 1.f);
    Render(TimePendingTickConsumption);
}

void FEngineLoop::ParseInput()
{
	check(GameWindow != nullptr && InputManager != nullptr);

    sf::Event Event;
    while (GameWindow->pollEvent(Event))
    {
        if (Event.type == sf::Event::Closed)
        {
            RequestExit();
        }
		else
		{
			InputManager->HandleWindowEvent(Event);
		}
    }
}

void FEngineLoop::Tick(const sf::Time& DeltaSeconds)
{
	check(DeltaSeconds > sf::Time::Zero);

	if (GameWorld != nullptr)
	{
		GameWorld->Tick(DeltaSeconds);
	}
}

void FEngineLoop::Render(const sf::Time& BlendTime)
{
	check(GameWindow != nullptr);

	GameWindow->clear();

	if (GameWorld != nullptr)
	{
		GameWorld->Render(BlendTime, *GameWindow, sf::RenderStates::Default);
	}

    GameWindow->display();
}

void FEngineLoop::Exit()
{
	GE_LOG(LogCore, Log, "Exiting Core Loop...");

	check(bIsRequestingExit);

	if (GameWindow != nullptr)
	{
		GameWindow->close();
		delete GameWindow;
		GameWindow = nullptr;
	}

	if (GameWorld != nullptr)
	{
		delete GameWorld;
		GameWorld = nullptr;
	}

	if (InputManager != nullptr)
	{
		delete InputManager;
		InputManager = nullptr;
	}

	GE_LOG(LogCore, Log, "Exited Core Loop.");
}

bool FEngineLoop::IsRequestingExit() const
{
    return bIsRequestingExit;
}

void FEngineLoop::RequestExit()
{
	GE_LOG(LogCore, Log, "A request was made to exit the core loop.");
    bIsRequestingExit = true;
}
