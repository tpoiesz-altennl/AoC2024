#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>

class FileUtil
{
public:
	static std::ifstream ReadInputFile(std::string callingLocation)
	{
		std::ifstream file(MakeFolder(callingLocation) + "/InputFile.txt");
		if (!file)
		{
			std::cout << "Oh no!\n";
		}
		return file;
	}

	static std::ifstream ReadTestFile(std::string callingLocation)
	{
		std::ifstream file(MakeFolder(callingLocation) + "/TestFile.txt");
		if (!file)
		{
			std::cout << "Oh no!\n";
		}
		return file;
	}

	template<typename T, typename... Args>
	static void OutputMatches(std::string callingLocation, const std::vector<T>& testCases, std::function<bool(const T&, Args...)> conditional, 
		bool matchConditional = true, Args... argsForConditional)
	{
		std::ofstream file(MakeFolder(callingLocation) + "/OutputFile.txt");
		if (!file)
		{
			std::cout << "Oh no!\n";
		}

		for (const T& testCase : testCases)
		{
			if (conditional(testCase, argsForConditional...) == matchConditional)
			{
				std::stringstream ss;
				//ss << testCase;
				//file.write(ss.str());
			}
		}
	}

private:
	static std::string MakeFolder(const std::string& filepath)
	{
		size_t lastSep, secLastSep;
		lastSep = filepath.rfind('\\');
		secLastSep = filepath.rfind('\\', lastSep - 1) + 1;
		return filepath.substr(secLastSep, lastSep - secLastSep);
	}
};
