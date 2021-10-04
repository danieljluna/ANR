// Gordian by Daniel Luna (2019)
#pragma once

namespace Gordian
{


unsigned int EngineInit(int argc, char **argv);

void EngineTick();

void EngineExit();

int EngineEntryPoint(int argc, char **argv);


};

#define USING_GORDIAN_ENTRY_POINT int main(int argc, char **argv) { return Gordian::EngineEntryPoint(argc, argv); }

