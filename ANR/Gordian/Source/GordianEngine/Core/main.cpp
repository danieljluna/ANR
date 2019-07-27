// Gordian by Daniel Luna (2019)

#include "GordianEngine/Core/Public/EngineLoop.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>

Gordian::FEngineLoop GEngineLoop;

unsigned int EngineInit(int argc, char **argv)
{
    unsigned int ErrorCode = GEngineLoop.Init(argc, argv);
    return ErrorCode;
}

void EngineTick()
{
    GEngineLoop.Tick();
}

void EngineExit()
{
    GEngineLoop.RequestExit();

    GEngineLoop.Exit();
}


int main(int argc, char **argv)
{
    // Enforce EngineExit is called
    struct EngineLoopCleanupGuard
    {
        ~EngineLoopCleanupGuard()
        {
            EngineExit();
        }
    } CleanupGuard;

    unsigned int ErrorCode = EngineInit(argc, argv);

    if (ErrorCode != 0)
    {
        return ErrorCode;
    }

    while (!GEngineLoop.IsRequestingExit())
    {
        EngineTick();
    }

    return ErrorCode;
}
