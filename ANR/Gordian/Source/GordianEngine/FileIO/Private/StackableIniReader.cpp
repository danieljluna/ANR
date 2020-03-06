// Gordian by Daniel Luna (2019)

#include "GordianEngine/FileIO/Public/StackableIniReader.h"

#include "inih/ini.h"

using namespace Gordian;
using std::string;

FStackableIniReader::FStackableIniReader(const string& RootIni)
	: INIReader("")	// Ignore the INIReader ctor
{
	_error = 0;	// Clear the error from the INIReader ctor
	OverwriteWithIni(RootIni);
}

int FStackableIniReader::OverwriteWithIni(const std::string& IniToOverwriteWith)
{
	_error = ini_parse(IniToOverwriteWith.c_str(), FStackableIniReader::ValueHandler, this);
	return _error;
}

int FStackableIniReader::ValueHandler(void* user, 
									 const char* section, 
									 const char* name,
									 const char* value)
{
	FStackableIniReader* reader = static_cast<FStackableIniReader*>(user);
	string key = MakeKey(section, name);
	reader->_values[key] = value;
	return 1;
}