#include "Day8.h"
#include "../Util.h"

#include <vector>
#include <string>
#include <regex>

char FindNewChar(const std::vector<std::string>& input, std::string& triedChars)
{
	std::regex reg("(?![" + triedChars + "])[\\w]");
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
std::vector<vec2> Day8::GetCharCoordinates(const std::vector<std::string>& input, char val)
{
	std::vector<vec2> ret;
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		for (unsigned int j = 0; j < input[0].size(); ++j)
		{
			if (input[i][j] == val)
			{
				ret.push_back(vec2(j, i));
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
	int numUniquePositions = 0;
	while (char c = FindNewChar(grid, triedCharacters))
	{
		std::vector<vec2> coordinates = GetCharCoordinates(grid, c);
		for (unsigned int i = 0; i < coordinates.size(); ++i)
		{
			for (unsigned int j = i + 1; j < coordinates.size(); ++j)
			{
				vec2 rc = coordinates[j] - coordinates[i];
				vec2 backFromFirst = coordinates[i] - rc;
				if (backFromFirst.x >= 0 && backFromFirst.x < grid[0].size() && backFromFirst.y >= 0 && backFromFirst.y < grid.size())
				{
					if (antinodeGrid[backFromFirst.y][backFromFirst.x] == '.')
					{
						antinodeGrid[backFromFirst.y][backFromFirst.x] = c;
						++numUniquePositions;
					}
				}
				vec2 forwardFromSecond = coordinates[j] + rc;
				if (forwardFromSecond.x >= 0 && forwardFromSecond.x < grid[0].size() && forwardFromSecond.y >= 0 && forwardFromSecond.y < grid.size())
				{
					if (antinodeGrid[forwardFromSecond.y][forwardFromSecond.x] == '.')
					{
						antinodeGrid[forwardFromSecond.y][forwardFromSecond.x] = c;
						++numUniquePositions;
					}
				}
			}
		}
	}

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputResultGrid(antinodeGrid);

	std::cout << numUniquePositions;

	return 0;
}

vec2 ReduceRC(const vec2& rc)
{
	int lowest = std::min(rc.x, rc.y);
	if (rc.x % lowest == 0 && rc.y % lowest == 0)
		return vec2(rc.x / lowest, rc.y / lowest);
	else
		return rc;
}

int Day8::Solution2()
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
	int numUniquePositions = 0;
	while (char c = FindNewChar(grid, triedCharacters))
	{
		std::vector<vec2> coordinates = GetCharCoordinates(grid, c);
		for (unsigned int i = 0; i < coordinates.size(); ++i)
		{
			for (unsigned int j = i + 1; j < coordinates.size(); ++j)
			{
				vec2 rc = ReduceRC(coordinates[j] - coordinates[i]);
				if (antinodeGrid[coordinates[i].y][coordinates[i].x] == '.')
				{
					antinodeGrid[coordinates[i].y][coordinates[i].x] = c;
					++numUniquePositions;
				}
				vec2 backwards = coordinates[i] - rc;
				int iter = 1;
				while (backwards.x >= 0 && backwards.x < grid[0].size() && backwards.y >= 0 && backwards.y < grid.size())
				{
					if (antinodeGrid[backwards.y][backwards.x] == '.')
					{
						antinodeGrid[backwards.y][backwards.x] = c;
						++numUniquePositions;
					}

					backwards = coordinates[i] - (rc * ++iter);
				}
				vec2 forwards = coordinates[i] + rc;
				iter = 1;
				while (forwards.x >= 0 && forwards.x < grid[0].size() && forwards.y >= 0 && forwards.y < grid.size())
				{
					if (antinodeGrid[forwards.y][forwards.x] == '.')
					{
						antinodeGrid[forwards.y][forwards.x] = c;
						++numUniquePositions;
					}

					forwards = coordinates[i] + (rc * ++iter);
				}
			}
		}
	}

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputResultGrid(antinodeGrid);

	std::cout << numUniquePositions;

	return 0;
}
