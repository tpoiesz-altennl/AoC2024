#include "Day10.h"
#include "../Util.h"

// returns coordinate pairs as (x,y)
std::vector<vec2> Day10::GetCharCoordinates(const std::vector<std::string>& input, char val)
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

int FindPath(const std::vector<std::string>& grid, const vec2& currCoord, char searchVal, OrthDirection incomingDir,
	int remainingSteps, std::vector<vec2>& foundNines, bool considerDuplicates = false, Testing::DebugFile* dbg = nullptr)
{
	if (!considerDuplicates)
	{
		if (grid[currCoord.y][currCoord.x] == '9' && std::find(foundNines.begin(), foundNines.end(), currCoord) == foundNines.end())
		{
			foundNines.push_back(currCoord);
			return 1;
		}
	}
	else
	{
		if (grid[currCoord.y][currCoord.x] == '9')
			return 1;
	}
	
	if (remainingSteps < 1)
		return 0;

	auto getNextCoord = [&](OrthDirection dir, vec2& nextCoord, OrthDirection& newIncoming) -> bool {
		switch (dir)
		{
		case OrthDirection::Up:
		{
			newIncoming = OrthDirection::Down;
			nextCoord.y -= 1;
			if (nextCoord.y < 0)
				return false;
			else
				return true;
		}
		case OrthDirection::Right:
		{
			newIncoming = OrthDirection::Left;
			nextCoord.x += 1;
			if (nextCoord.x >= static_cast<int>(grid[0].size()))
				return false;
			else
				return true;
		}
		case OrthDirection::Down:
		{
			newIncoming = OrthDirection::Up;
			nextCoord.y += 1;
			if (nextCoord.y >= static_cast<int>(grid.size()))
				return false;
			else
				return true;
		}
		case OrthDirection::Left:
		{
			newIncoming = OrthDirection::Right;
			nextCoord.x -= 1;
			if (nextCoord.x < 0)
				return false;
			else
				return true;
		}
		default:
			return false;
		}
		};

	int totalPaths = 0;
	OrthDirection nextDir = incomingDir;
	++nextDir;
	if (incomingDir == OrthDirection::None)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			vec2 nextCoord = currCoord;
			OrthDirection newIncoming = OrthDirection::None;
			if (getNextCoord(nextDir, nextCoord, newIncoming))
			{
				if (grid[nextCoord.y][nextCoord.x] == searchVal)
				{
					if (int numPaths = FindPath(grid, nextCoord, searchVal + 1, newIncoming, 
						remainingSteps - 1, foundNines, considerDuplicates, dbg))
					{
						/*if (dbg)
							; // Do some cool output magic*/
						totalPaths += numPaths;
					}
				}
			}
			++nextDir;
		}
	}
	else
	{
		for (; nextDir != incomingDir; ++nextDir)
		{
			vec2 nextCoord = currCoord;
			OrthDirection newIncoming = OrthDirection::None;
			if (getNextCoord(nextDir, nextCoord, newIncoming))
			{
				if (grid[nextCoord.y][nextCoord.x] == searchVal)
				{
					if (int numPaths = FindPath(grid, nextCoord, searchVal + 1, newIncoming, 
						remainingSteps - 1, foundNines, considerDuplicates, dbg))
					{
						/*if (dbg)
							; // Do some cool output magic*/
						totalPaths += numPaths;
					}
				}
			}
		}
	}

	return totalPaths;
}

int Day10::Solution1()
{
	//std::vector<std::string> grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	std::vector<std::string> grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	std::vector<std::string> debugOutput;
	for (unsigned int i = 0; i < grid.size(); ++i)
	{
		std::string dots = "";
		for (unsigned int j = 0; j < grid[i].size(); ++j)
		{
			dots += '.';
		}
		debugOutput.push_back(dots);
	}

	std::vector<vec2> zeroes = GetCharCoordinates(grid, '0');
	
	int totalPaths = 0;
	for (const vec2& zero : zeroes)
	{
		std::vector<vec2> foundNines;
		totalPaths += FindPath(grid, zero, '1', OrthDirection::None, 9, foundNines);
	}

	std::cout << totalPaths;

	return 0;
}

int Day10::Solution2()
{
	//std::vector<std::string> grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	std::vector<std::string> grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	std::vector<std::string> debugOutput;
	for (unsigned int i = 0; i < grid.size(); ++i)
	{
		std::string dots = "";
		for (unsigned int j = 0; j < grid[i].size(); ++j)
		{
			dots += '.';
		}
		debugOutput.push_back(dots);
	}

	std::vector<vec2> zeroes = GetCharCoordinates(grid, '0');

	int totalPaths = 0;
	for (const vec2& zero : zeroes)
	{
		std::vector<vec2> foundNines;
		totalPaths += FindPath(grid, zero, '1', OrthDirection::None, 9, foundNines, true);
	}

	std::cout << totalPaths;

	return 0;
}
