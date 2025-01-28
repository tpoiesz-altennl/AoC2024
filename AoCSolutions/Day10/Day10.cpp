#include "Day10.h"
#include "../Util.h"

unsigned int FindPath(const Grid& grid, const vec2& currCoord, char searchVal, OrthDirection incomingDir,
	int remainingSteps, std::vector<vec2>& foundNines, bool considerDuplicates = false)
{
	if (!considerDuplicates)
	{
		if (grid.at(currCoord) == '9' && std::find(foundNines.begin(), foundNines.end(), currCoord) == foundNines.end())
		{
			foundNines.push_back(currCoord);
			return 1;
		}
	}
	else
	{
		if (grid.at(currCoord) == '9')
			return 1;
	}
	
	if (remainingSteps < 1)
		return 0;

	int totalPaths = 0;
	OrthDirection nextDir = incomingDir + 1;
	if (incomingDir == OrthDirection::None)
	{
		for (unsigned int i = 0; i < 4; ++i, ++nextDir)
		{
			vec2 nextCoord = NextPos(currCoord, nextDir);
			if (grid.IsValidPos(nextCoord) && grid.at(nextCoord) == searchVal)
			{
				OrthDirection newIncoming = GetOppositeDir(nextDir);
				if (unsigned int numPaths = FindPath(grid, nextCoord, searchVal + 1, newIncoming, 
					remainingSteps - 1, foundNines, considerDuplicates))
				{
					totalPaths += numPaths;
				}
			}
		}
	}
	else
	{
		for (; nextDir != incomingDir; ++nextDir)
		{
			vec2 nextCoord = NextPos(currCoord, nextDir);
			if (grid.IsValidPos(nextCoord) && grid.at(nextCoord) == searchVal)
			{
				OrthDirection newIncoming = GetOppositeDir(nextDir);
				if (unsigned int numPaths = FindPath(grid, nextCoord, searchVal + 1, newIncoming, 
					remainingSteps - 1, foundNines, considerDuplicates))
				{
					totalPaths += numPaths;
				}
			}
		}
	}

	return totalPaths;
}

int Day10::Solution1()
{
	//Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	std::vector<vec2> zeroes = grid.FindAll('0');
	
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
	//Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	std::vector<vec2> zeroes = grid.FindAll('0');

	int totalPaths = 0;
	for (const vec2& zero : zeroes)
	{
		std::vector<vec2> foundNines;
		totalPaths += FindPath(grid, zero, '1', OrthDirection::None, 9, foundNines, true);
	}

	std::cout << totalPaths;

	return 0;
}
