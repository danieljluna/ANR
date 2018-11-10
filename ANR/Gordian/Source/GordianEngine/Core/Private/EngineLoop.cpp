// Gordian by Daniel Luna

#include "GordianEngine/Core/Public/EngineLoop.h"

#include <assert.h>

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "GordianEngine/World/Public/World.h"

using namespace Gordian;

FEngineLoop::FEngineLoop()
    : GameWindow(nullptr)
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
	assert(GameWindow == nullptr && GameWorld == nullptr);
}

sf::Int32 FEngineLoop::Init()
{
    sf::Int32 ErrorCode = 0;
    ErrorCode = InitializeGameWindow();
    if (ErrorCode != 0)
    {
        return ErrorCode;
    }

	GameWorld = new OWorld();
    bIsRequestingExit = false;
    TickDurationClock.restart();
    return 0;
}

sf::Int32 FEngineLoop::InitializeGameWindow()
{
    // todo: create inis for this
    GameWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Title");
    if (!GameWindow->isOpen())
    {
        return 1;
    }

    GameWindow->setVerticalSyncEnabled(true);
    GameWindow->setMouseCursorGrabbed(false);
	GameWindow->setView(GameWindow->getDefaultView());
    return 0;
}

void FEngineLoop::Tick()
{
    assert(!bIsRequestingExit);

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
    assert(BlendFactor >= 0.f && BlendFactor < 1.f);
    Render(TimePendingTickConsumption);
}

void FEngineLoop::ParseInput()
{
	assert(GameWindow != nullptr);

    sf::Event Event;
    while (GameWindow->pollEvent(Event))
    {
        if (Event.type == sf::Event::Closed)
        {
            RequestExit();
        }
    }
}

void FEngineLoop::Tick(const sf::Time& DeltaSeconds)
{
	assert(DeltaSeconds > sf::Time::Zero);

	if (GameWorld != nullptr)
	{
		GameWorld->Tick(DeltaSeconds);
	}
}

void FEngineLoop::Render(const sf::Time& BlendTime)
{
	assert(GameWindow != nullptr);

	if (GameWorld != nullptr)
	{
		GameWorld->Render(BlendTime, *GameWindow, sf::RenderStates::Default);
	}
    GameWindow->display();
}

void FEngineLoop::Exit()
{
	assert(bIsRequestingExit);

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
}

bool FEngineLoop::IsRequestingExit() const
{
    return bIsRequestingExit;
}

void FEngineLoop::RequestExit()
{
    bIsRequestingExit = true;
}
