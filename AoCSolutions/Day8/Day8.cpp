#include "Day8.h"
#include "../Util.h"

#include <vector>
#include <string>
#include <regex>

char FindNewChar(const Grid& input, std::string& triedChars)
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

vec2 ReduceRC(const vec2& rc)
{
	int lowest = std::min(rc.x, rc.y);
	if (rc.x % lowest == 0 && rc.y % lowest == 0)
		return vec2(rc.x / lowest, rc.y / lowest);
	else
		return rc;
}

int Day8::Solution1()
{
	//Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	Grid antinodeGrid;
	{
		std::vector<std::string> temp;
		for (unsigned int y = 0; y < grid.height(); ++y)
		{
			std::string dots = "";
			for (unsigned int j = 0; j < grid.width(); ++j)
			{
				dots += '.';
			}
			temp.push_back(dots);
		}
		antinodeGrid = Grid(temp);
	}

	std::string triedCharacters = "";
	int numUniquePositions = 0;
	while (char c = FindNewChar(grid, triedCharacters))
	{
		std::vector<vec2> coordinates = grid.FindAll(c);
		for (unsigned int i = 0; i < coordinates.size(); ++i)
		{
			for (unsigned int j = i + 1; j < coordinates.size(); ++j)
			{
				vec2 rc = coordinates[j] - coordinates[i];
				vec2 backFromFirst = coordinates[i] - rc;
				if (grid.IsValidPos(backFromFirst) && antinodeGrid[backFromFirst] == '.')
				{
					antinodeGrid[backFromFirst] = c;
					++numUniquePositions;
				}
				vec2 forwardFromSecond = coordinates[j] + rc;
				if (grid.IsValidPos(forwardFromSecond) && antinodeGrid[forwardFromSecond] == '.')
				{
					antinodeGrid[forwardFromSecond] = c;
					++numUniquePositions;
				}
			}
		}
	}

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputGrid(antinodeGrid);

	std::cout << numUniquePositions;

	return 0;
}

int Day8::Solution2()
{
	//Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	Grid antinodeGrid;
	{
		std::vector<std::string> temp;
		for (unsigned int y = 0; y < grid.height(); ++y)
		{
			std::string dots = "";
			for (unsigned int j = 0; j < grid.width(); ++j)
			{
				dots += '.';
			}
			temp.push_back(dots);
		}
		antinodeGrid = Grid(temp);
	}

	std::string triedCharacters = "";
	int numUniquePositions = 0;
	while (char c = FindNewChar(grid, triedCharacters))
	{
		std::vector<vec2> coordinates = grid.FindAll(c);
		for (unsigned int i = 0; i < coordinates.size(); ++i)
		{
			for (unsigned int j = i + 1; j < coordinates.size(); ++j)
			{
				vec2 rc = ReduceRC(coordinates[j] - coordinates[i]);
				if (antinodeGrid[coordinates[i]] == '.')
				{
					antinodeGrid[coordinates[i]] = c;
					++numUniquePositions;
				}
				vec2 backwards = coordinates[i] - rc;
				int iter = 1;
				while (grid.IsValidPos(backwards))
				{
					if (antinodeGrid[backwards] == '.')
					{
						antinodeGrid[backwards] = c;
						++numUniquePositions;
					}

					backwards = coordinates[i] - (rc * ++iter);
				}
				vec2 forwards = coordinates[i] + rc;
				iter = 1;
				while (grid.IsValidPos(forwards))
				{
					if (antinodeGrid[forwards] == '.')
					{
						antinodeGrid[forwards] = c;
						++numUniquePositions;
					}

					forwards = coordinates[i] + (rc * ++iter);
				}
			}
		}
	}

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputGrid(antinodeGrid);

	std::cout << numUniquePositions;

	return 0;
}
