#include "Day8.h"
#include "../Util.h"

#include <vector>
#include <string>
#include <regex>

std::pair<int, int> operator+(std::pair<int, int> a, std::pair<int, int> b)
{
	return std::pair<int, int>(a.first + b.first, a.second + b.second);
}

std::pair<int, int> operator-(std::pair<int, int> a, std::pair<int, int> b)
{
	return std::pair<int, int>(a.first - b.first, a.second - b.second);
}

char FindNewChar(const std::vector<std::string>& input, std::string& triedChars)
{
	std::string str = "(?![" + triedChars + "])[a-z]|(?![" + triedChars + "])[A-Z]|(?![" + triedChars + "])[0-9]";
	std::regex reg(str);
	for (const std::string& line : input)
	{
		std::smatch m;
		if (std::regex_search(line, m, reg))
		{
			triedChars += m[0].str();
			return m[0].str()[0];
		}
	}
	return NULL;
}

// returns coordinate pairs as (x,y)
std::vector<std::pair<int, int>> GetCharCoordinates(const std::vector<std::string>& input, char val)
{
	std::vector<std::pair<int, int>> ret;
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int j = 0; j < input[0].size(); ++j)
		{
			if (input[i][j] == val)
			{
				ret.push_back(std::pair<int, int>(j, i));
			}
		}
	}
	return ret;
}

int Day8::Solution1()
{
	//std::vector<std::string> grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	std::vector<std::string> grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	std::vector<std::string> antinodeGrid;
	for (unsigned int i = 0; i < grid.size(); ++i)
	{
		std::string dots = "";
		for (unsigned int j = 0; j < grid[i].size(); ++j)
		{
			dots += '.';
		}
		antinodeGrid.push_back(dots);
	}

	std::string triedCharacters = "";
	while (char c = FindNewChar(grid, triedCharacters))
	{
		std::vector<std::pair<int, int>> coordinates = GetCharCoordinates(grid, c);
		for (unsigned int i = 0; i < coordinates.size(); ++i)
		{
			for (unsigned int j = i + 1; j < coordinates.size(); ++j)
			{
				std::pair<int, int> rc = coordinates[j] - coordinates[i];
			}
		}
	}

	return 0;
}

int Day8::Solution2()
{
	return 0;
}
