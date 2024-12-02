#pragma once

#include <string>
#include <fstream>
#include <iostream>

std::ifstream ReadInputFile(std::string callingLocation)
{
	size_t lastSep, secLastSep;
	lastSep = callingLocation.rfind('\\');
	secLastSep = callingLocation.rfind('\\', lastSep - 1) + 1;
	std::string folder = callingLocation.substr(secLastSep, lastSep - secLastSep);
	std::ifstream file(folder + "/InputFile.txt");
	if (!file)
	{
		std::cout << "Oh no!\n";
	}
	return file;
}

std::ifstream ReadTestFile(std::string callingLocation)
{
	size_t lastSep, secLastSep;
	lastSep = callingLocation.rfind('\\');
	secLastSep = callingLocation.rfind('\\', lastSep - 1) + 1;
	std::string folder = callingLocation.substr(secLastSep, lastSep - secLastSep);
	std::ifstream file(folder + "/TestFile.txt");
	if (!file)
	{
		std::cout << "Oh no!\n";
	}
	return file;
}
