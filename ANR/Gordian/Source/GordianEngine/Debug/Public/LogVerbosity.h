// Gordian by Daniel Luna (2019)

#pragma once

#include <SFML/Config.hpp>

namespace Gordian
{


// Log verbosity is used to determine whether or not the log should be printed
//	based on it's importance.
enum ELogVerbosity : sf::Uint8
{
	// Printed to console and log files. Appears in red. Terminates the program.
	// Cannot be suppressed.
	Fatal		= 0,
	// Printed to console and log files. Appears in red.
	Error,
	// Printed to console and log files. Appears in yellow.
	Warning,
	// Printed to console and log files.
	Display,
	// Printed only to log files.
	Log,
	// Printed only to log files. Saved for detailed logging.
	Verbose,
	// Shorthand for displaying all logs
	All			= Verbose,
	// Utility value for counting enum entries
	Count,
};


};
