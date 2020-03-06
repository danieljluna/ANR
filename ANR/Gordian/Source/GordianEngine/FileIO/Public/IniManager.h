// Gordian by Daniel Luna (2019)

#pragma once

#include <string>

#include "inih/INIReader.h"

#include "StackableIniReader.h"

namespace Gordian
{


// Manages all ini categories and determines where to look for ini files
class IniManager
{
public:

	// Returns the singleton
	static IniManager& Get();

	// Returns the ini reader for a given category.
	// Returns an empty reader if the given category has not been instantiated
	const FStackableIniReader& GetIniCategory(const std::string& IniCategory);

private:

	static FStackableIniReader EmptyReader;

	// This should never be instantiated
	IniManager();
	~IniManager();

	// Loads all ini files relevant to the given category and stores off the data.
	// Returns whether or not all inis loaded without errors.
	const FStackableIniReader* LoadIniCategory(const std::string& IniCategory);

	// Returns the filepath of the given ini category's ini file. Assumes the category
	// has already been properly trimmed
	std::string GetFilePathToIniCategory(const std::string& IniCategory) const;

	// Trims a category string so it is legal
	std::string GetTrimmedCategory(const std::string& InCategory) const;

	// Maps Ini Categories to a StackableIniReader
	std::map<std::string, FStackableIniReader*> IniReadersByCategory;

};


};

