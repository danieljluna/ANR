// Gordian by Daniel Luna (2019)

#include "GordianEngine/Core/Public/EntryPoint.h"
#include "GordianEngine/Core/Public/EngineLoop.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>

static Gordian::FEngineLoop Gordian::GEngineLoop;

unsigned int Gordian::EngineInit(int argc, char **argv)
{
	unsigned int ErrorCode = Gordian::GEngineLoop.Init(argc, argv);
	return ErrorCode;
}

void Gordian::EngineTick()
{
	Gordian::GEngineLoop.Tick();
}

void Gordian::EngineExit()
{
	Gordian::GEngineLoop.Exit(true);
}

int Gordian::EngineEntryPoint(int argc, char **argv)
{
	// Enforce EngineExit is called
	struct EngineLoopCleanupGuard
	{
		~EngineLoopCleanupGuard()
		{
			Gordian::EngineExit();
		}
	} CleanupGuard;

	unsigned int ErrorCode = Gordian::EngineInit(argc, argv);

	if (ErrorCode != 0)
	{
		return ErrorCode;
	}

	while (!Gordian::GEngineLoop.IsRequestingExit())
	{
		Gordian::EngineTick();
	}

	return ErrorCode;
}
