#include "Day20.h"
#include "../Util.h"
#include <map>

// Returns number of steps that each given tile is away from the start of the path
std::map<vec2, unsigned int> TraversePath(const Grid& grid)
{
	vec2 startPos = grid.FindChar('S');
	vec2 endPos = grid.FindChar('E');
	vec2 currPos = startPos;
	OrthDirection currDir = OrthDirection::Up;
	// Find initial direction
	for (; ; ++currDir)
	{
		if (grid.at(NextPos(startPos, currDir)) != '#')
			break;
	}

	std::map<vec2, unsigned int> ret;
	unsigned int currSteps = 0;
	while (currPos != endPos)
	{
		ret.emplace(std::make_pair(currPos, currSteps));
		if (grid.at(NextPos(currPos, currDir)) != '#')
		{
			currPos = NextPos(currPos, currDir);
			currSteps += 1;
		}
		else
		{
			for (OrthDirection checkDir = currDir - 1; checkDir != GetOppositeDir(currDir); checkDir += 2)
			{
				if (grid.at(NextPos(currPos, checkDir)) != '#')
				{
					currDir = checkDir;
					currPos = NextPos(currPos, currDir);
					currSteps += 1;
					break;
				}
			}
		}
	}
	// Also add endPos
	ret.emplace(std::make_pair(currPos, currSteps));
	return ret;
}

unsigned int CountHighValueCheats(const std::map<vec2, unsigned int>& pathLookup, unsigned int valueThreshold)
{
	unsigned int numValuableCheats = 0;
	for (const auto& pair : pathLookup)
	{
		vec2 skipPos(pair.first.x + 2, pair.first.y);
		auto it = pathLookup.find(skipPos);
		if (it != pathLookup.end())
		{
			unsigned int skipValue = (it->second > pair.second ? it->second - pair.second : pair.second - it->second) - 2;
			if (skipValue >= valueThreshold)
				++numValuableCheats;
		}

		skipPos = vec2(pair.first.x, pair.first.y + 2);
		it = pathLookup.find(skipPos);
		if (it != pathLookup.end())
		{
			unsigned int skipValue = (it->second > pair.second ? it->second - pair.second : pair.second - it->second) - 2;
			if (skipValue >= valueThreshold)
				++numValuableCheats;
		}
	}
	return numValuableCheats;
}

int Day20::Solution1()
{
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));
	//Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true));
	std::map<vec2, unsigned int> pathLookup = TraversePath(grid);

	std::cout << CountHighValueCheats(pathLookup, 100);
	//std::cout << CountHighValueCheats(pathLookup, 38);
	return 0;
}

unsigned int CountHighValueCheats2(const std::map<vec2, unsigned int>& pathLookup, int gridW, int gridH, int cheatRange, unsigned int valueThreshold)
{
	unsigned int numValuableCheats = 0;
	for (const auto& pair : pathLookup)
	{
		vec2 centre = pair.first;
		int range = 0;
		bool increaseRange = true;
		for (int y = centre.y - cheatRange; y <= centre.y + cheatRange; ++y)
		{
			// Ignore grid edges, they only consist of walls
			if (y < gridH - 1)
			{
				if (y >= 1)
				{
					for (int x = centre.x - range; x <= centre.x + range; ++x)
					{
						// Ignore grid edges, they only consist of walls
						if (x < gridW - 1)
						{
							if (x >= 1)
							{
								vec2 skipPos(x, y);
								auto it = pathLookup.find(skipPos);
								if (it != pathLookup.end())
								{
									unsigned int cheatDist = std::abs(centre.y - y) + std::abs(centre.x - x);
									if (cheatDist > 1)
									{
										unsigned int skipValue = 
											(it->second > pair.second ? it->second - pair.second : pair.second - it->second) - cheatDist;
										if (skipValue >= valueThreshold)
											++numValuableCheats;
									}
								}
							}
						}
						else // Stop checking if there are only positions outside of grid bounds left
							break;
					}
				}
			}
			else // Stop checking if there are only positions outside of grid bounds left
				break;

			if (increaseRange)
				++range;
			else
				--range;
			if (range == cheatRange)
				increaseRange = false;
		}
	}
	// Every cheat is counted twice (once from a to b and once from b to a), so divide by 2
	return numValuableCheats / 2;
}

int Day20::Solution2()
{
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));
	//Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true));
	std::map<vec2, unsigned int> pathLookup = TraversePath(grid);

	std::cout << CountHighValueCheats2(pathLookup, static_cast<int>(grid[0].size()), static_cast<int>(grid.size()), 20, 100);
	//std::cout << CountHighValueCheats2(pathLookup, static_cast<int>(grid[0].size()), static_cast<int>(grid.size()), 20, 50);
	return 0;
}
