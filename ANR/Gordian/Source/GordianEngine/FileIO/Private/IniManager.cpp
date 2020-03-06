// Gordian by Daniel Luna (2019)

#include "GordianEngine/FileIO/Public/IniManager.h"

#include <cstdio>
#include <cmath>

#include "GordianEngine/Debug/Public/Asserts.h"

using namespace Gordian;

namespace
{
	const std::string k_IniPath = "../Gordian/Config/";
	const std::string k_IniFileSuffix = ".ini";
}

FStackableIniReader IniManager::EmptyReader("");

IniManager::IniManager()
{
	
}

IniManager::~IniManager()
{
	for (auto& it : IniReadersByCategory)
	{
		if (it.second != nullptr)
		{
			delete it.second;
		}
	}
}

IniManager& IniManager::Get()
{
	static IniManager Singleton;

	return Singleton;
}

const FStackableIniReader& IniManager::GetIniCategory(const std::string& IniCategory)
{
	const std::string FilePathToIniAsString = GetFilePathToIniCategory(IniCategory);
	const char* FilePathToIni = FilePathToIniAsString.c_str();
	const FStackableIniReader* CategoryReader = nullptr;

	decltype(IniReadersByCategory)::const_iterator it = IniReadersByCategory.find(IniCategory);
	if (IniReadersByCategory.find(IniCategory) == IniReadersByCategory.cend())
	{
		CategoryReader = LoadIniCategory(IniCategory);
	}
	else
	{
		CategoryReader = it->second;
	}

	return CategoryReader == nullptr ? EmptyReader : *CategoryReader;
}

const FStackableIniReader* IniManager::LoadIniCategory(const std::string& IniCategory)
{
	check(IniReadersByCategory.find(IniCategory) == IniReadersByCategory.cend());

	const std::string TrimmedCategory = GetTrimmedCategory(IniCategory);
	const std::string PathToIniAsString = GetFilePathToIniCategory(TrimmedCategory);
	const char* PathToIni = PathToIniAsString.c_str();

	FILE* IniFile;
	fopen_s(&IniFile, PathToIni, "r");
	if (IniFile == NULL)
	{
		// If the ini doesn't exist then fail to load
		return nullptr;
	}
	fclose(IniFile);	// we can close the file - it will be reopened on the ctor

	FStackableIniReader* CategoryIniReader = new FStackableIniReader(PathToIni);
	IniReadersByCategory.emplace(TrimmedCategory, CategoryIniReader);

	return CategoryIniReader;
}

std::string IniManager::GetFilePathToIniCategory(const std::string& IniCategory) const
{
	std::string ReturnValue = k_IniPath + IniCategory + k_IniFileSuffix;
	return ReturnValue;
}

std::string IniManager::GetTrimmedCategory(const std::string& InCategory) const
{
	const size_t BeginIndex = InCategory.find_first_not_of(" \n\t/\\.");
	std::string TrimmedCategory = InCategory.substr(BeginIndex);
	const size_t EndIndex = InCategory.find_first_of(" \n\t/\\.");
	TrimmedCategory = TrimmedCategory.substr(0, EndIndex);

	return TrimmedCategory;
}

