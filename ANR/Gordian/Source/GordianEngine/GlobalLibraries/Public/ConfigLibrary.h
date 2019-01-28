// Gordian by Daniel Luna (2019)

#pragma once

#include <string>

namespace Gordian
{

class FConfigLibrary
{
public:
	// Converts the name of an ini file to its full file if it exists.
	//
	// Returns the full path if the config was found, returns empty string 
	//	if it could not be found.
	static std::string TryAndFindConfig(const std::string& ConfigName);

private:
	// Called when a config could not be found. Logs warnings.
	static void OnFailToFindConfig(const std::string& ConfigName);
};

};
