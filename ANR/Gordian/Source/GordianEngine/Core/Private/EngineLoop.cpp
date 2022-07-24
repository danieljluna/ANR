// Gordian by Daniel Luna (2019)

#include "GordianEngine/Core/Public/EngineLoop.h"

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/CommandPrompt.h"
#include "GordianEngine/Debug/Public/Logging.h"
#include "GordianEngine/FileIO/Public/IniManager.h"
#include "GordianEngine/FileIO/Public/StackableIniReader.h"
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

sf::Int32 FEngineLoop::Init(int argc, char** argv)
{
	GE_LOG(LogCore, Log, "Initializing Core Loop...");

	sf::Int32 ErrorCode = 0;

	ErrorCode = ParseCommandArgs(argc, argv);
	if (ErrorCode != 0)
	{
		GE_LOG(LogCore, Error, "Failed to parse command line arguments!");
		return ErrorCode;
	}

#if GE_USE_CONSOLE
	// Ensure console formatting has been initialized
	ErrorCode = FConsoleFormatting::InitializeFormatting();
	if (ErrorCode != 0)
	{
		GE_LOG(LogCore, Error, "Failed to initialize console formatting, code: %d", ErrorCode);
		return ErrorCode;
	}
#endif

	char FilenameFromPath[50];
	_splitpath_s(argv[0], NULL, 0, NULL, 0, FilenameFromPath, 50, NULL, 0);

    ErrorCode = InitializeGameWindow(FilenameFromPath);
    if (ErrorCode != 0)
    {
        return ErrorCode;
    }

	// Initialize classes

	InputManager = new FInputManager();
	GameWorld = FGlobalObjectLibrary::CreateObject<OWorld>(nullptr, OWorld::GetStaticType(), "GameWorld");
    bIsRequestingExit = false;
    TickDurationClock.restart();

	GE_LOG(LogCore, Log, "Core Loop Initialized Successfully.");
    return ErrorCode;
}

sf::Int32 FEngineLoop::ParseCommandArgs(int argc, char** argv)
{
	check(argc > 0);
	for (int i = 1; i < argc; ++i)
	{
		// This should become data driven
		return LoadProject(argv[i]);
	}

	return 0;
}

sf::Int32 FEngineLoop::InitializeGameWindow(const char* WindowTitle)
{
	const INIReader& IniReader = IniManager::Get().GetIniCategory("Engine");

	GameWindow = new sf::RenderWindow(sf::VideoMode(IniReader.GetInteger("Graphics", "WindowWidth", 800),
													IniReader.GetInteger("Graphics", "WindowHeight", 600)),
									  WindowTitle,
									  sf::Style::Titlebar | sf::Style::Close);

    if (!GameWindow->isOpen())
    {
		GE_LOG(LogCore, Error, "Failed to open a render window!");
        return 1;
    }

    GameWindow->setVerticalSyncEnabled(IniReader.GetBoolean("Graphics", "VerticalSync", true));
    GameWindow->setMouseCursorGrabbed(IniReader.GetBoolean("Graphics", "LockCursorInWindow", false));
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
			continue;
		}

		if (FCommandPrompt::Get().ParseRawInput(Event))
		{
			// Command prompt has hogged this input, continue...
			continue;
		}
		
		InputManager->HandleWindowEvent(Event);
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

	GameWindow->draw(FCommandPrompt::Get());

    GameWindow->display();
}

void FEngineLoop::Exit(bool bForceImmediate)
{
	GE_LOG(LogCore, Log, "Exiting Core Loop...");

	if (!bIsRequestingExit && bForceImmediate)
	{
		// If we need to request exit, now is the time
		RequestExit();
	}

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

bool FEngineLoop::LoadProject(const char* InProjectPath)
{
	// todo: initialize loading project classes / defaults
	return false;
}

const sf::Vector2u& FEngineLoop::GetWindowSize() const
{
	check(GameWindow != nullptr);
	return GameWindow->getSize();
}

// A node in a sorted binary tree
struct BinaryTreeNode
{
	// Points to the parent of this node in the tree
	BinaryTreeNode* Parent;
	// Points to the left child of this node
	// In ascending sort, this always points to a node with a smaller value
	BinaryTreeNode* Left;
	// Points to the right child of this node
	// In ascending sort, this always points to a node with a larger value
	BinaryTreeNode* Right;

	char Value;
};


// Given a root node, searches for a descendant node with the given value
const BinaryTreeNode* FindValue(const BinaryTreeNode* Root, char InValue)
{
	const BinaryTreeNode* CurrentSearchNode = Root;
	while (CurrentSearchNode != nullptr)
	{
		if (InValue == CurrentSearchNode->Value)
		{
			return CurrentSearchNode;
		}
		else if (InValue < CurrentSearchNode->Value)
		{
			CurrentSearchNode = CurrentSearchNode->Left;
		}
		else // InValue > CurrentSearchNode->Value
		{
			CurrentSearchNode = CurrentSearchNode->Right;
		}
	}

	return nullptr;
}
