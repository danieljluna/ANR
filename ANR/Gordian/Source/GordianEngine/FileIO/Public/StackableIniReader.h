// Gordian by Daniel Luna (2019)

#pragma once

#include <string>

#include "inih/INIReader.h"

namespace Gordian
{


// Reads ini files, mapping string keys to the last declared value found
// in the read ini. Can read multiple files, stacking any settings on top
// of values found in previous inis.
class FStackableIniReader : public INIReader
{
public:

	// Initializes this Reader with the given file.
	explicit FStackableIniReader(const std::string& RootIni);

	// This will read the passed file as an ini and stomp any matching keys
	// that already exist in this reader. This will also update the error value.
	int OverwriteWithIni(const std::string& IniToOverwriteWith);

private:

	// Called to parse new ini values
	static int ValueHandler(void* user, 
							const char* section, 
							const char* name,
							const char* value);
};


};

