#include "Day18.h"
#include "../Util.h"

static constexpr unsigned int c_GridW = 71, c_GridH = 71;
static constexpr unsigned int c_TestGridW = 7, c_TestGridH = 7;

unsigned int Heuristic(const vec2& from, const vec2& to)
{
	return std::abs(to.x - from.x) + std::abs(to.y - from.y);
}

std::vector<std::string> BuildGrid(const std::vector<vec2>& blockades, unsigned int numBlockadesToUse, unsigned int offsetIntoBlockades, 
	bool isTest = false)
{
	std::vector<std::string> gridInput;
	for (unsigned int y = 0; y < (isTest ? c_TestGridH : c_GridH); ++y)
	{
		std::string gridLine;
		for (unsigned int x = 0; x < (isTest ? c_TestGridW : c_GridW); ++x)
		{
			gridLine += '.';
		}
		gridInput.push_back(gridLine);
	}

	unsigned int maxIndex = (blockades.size() > offsetIntoBlockades + numBlockadesToUse) ? 
		(offsetIntoBlockades + numBlockadesToUse) : static_cast<unsigned int>(blockades.size() - 1);

	for (unsigned int i = offsetIntoBlockades; i < maxIndex; ++i)
	{
		vec2 blockadePos = blockades[i];
		gridInput[blockadePos.y][blockadePos.x] = '#';
	}

	return gridInput;
}

struct MazeTile
{
	vec2 Pos;
	OrthDirection IncomingDir;
	unsigned int CostToReach;
	unsigned int EstimatedTotalCost;

	MazeTile() : Pos(vec2()), IncomingDir(OrthDirection::None), CostToReach(0), EstimatedTotalCost(0)
	{ }

	MazeTile(vec2 pos, OrthDirection dir, unsigned int cost, unsigned int estimate)
		: Pos(pos), IncomingDir(dir), CostToReach(cost), EstimatedTotalCost(estimate)
	{ }
};

std::vector<MazeTile> Expand(const Grid& grid, vec2 pos, OrthDirection incomingDir)
{
	std::vector<MazeTile> ret;
	for (OrthDirection dir = incomingDir + 1; dir != incomingDir; ++dir)
	{
		vec2 nextTilePos = NextPos(pos, dir);
		if (grid.IsValidPos(nextTilePos) && grid.at(nextTilePos) != '#')
			ret.push_back(MazeTile(nextTilePos, GetOppositeDir(dir), 1, 0));
	}
	return ret;
}

int AStar(const Grid& grid, vec2 startPos, vec2 endPos)
{
	priority_queue<MazeTile> frontierTiles([](const MazeTile& a, const MazeTile& b) { return a.EstimatedTotalCost < b.EstimatedTotalCost; });
	std::vector<MazeTile> foundTiles;

	OrthDirection dir = OrthDirection::Up;
	for (unsigned int i = 0; i < 4; ++i, ++dir)
	{
		vec2 nextTilePos = NextPos(startPos, dir);
		if (grid.IsValidPos(nextTilePos) && grid.at(nextTilePos) != '#')
		{
			MazeTile tile(nextTilePos, GetOppositeDir(dir), 1, 1 + Heuristic(nextTilePos, endPos));
			foundTiles.push_back(tile);
			frontierTiles.insert(tile);
		}
	}

	while (!frontierTiles.empty())
	{
		if (frontierTiles.front().Pos == endPos)
			break;

		MazeTile frontier = frontierTiles.front();
		frontierTiles.pop_front();

		for (MazeTile nextFrontier : Expand(grid, frontier.Pos, frontier.IncomingDir))
		{
			nextFrontier.CostToReach += frontier.CostToReach;
			nextFrontier.EstimatedTotalCost = nextFrontier.CostToReach + Heuristic(nextFrontier.Pos, endPos);
			auto foundIt = std::find_if(foundTiles.begin(), foundTiles.end(), [&nextFrontier](const MazeTile& tile) 
				{ return nextFrontier.Pos == tile.Pos; }
			);

			if (foundIt == foundTiles.end())
			{
				foundTiles.push_back(nextFrontier);
				frontierTiles.insert(nextFrontier);
			}
			else if (nextFrontier.EstimatedTotalCost < foundIt->EstimatedTotalCost)
			{
				*foundIt = nextFrontier;
				frontierTiles.insert(nextFrontier);
			}
		}
	}

	if (!frontierTiles.empty())
		return frontierTiles.front().CostToReach;
	else
		return 0;
}

int Day18::Solution1()
{
	//bool isTest = true;
	bool isTest = false;
	unsigned int simulationSteps = (isTest ? 12 : 1024);
	std::vector<vec2> fallingBytes = FileUtil::ReadInputIntoVec<vec2>(__FILE__, isTest);

	Grid grid(BuildGrid(fallingBytes, simulationSteps, 0, isTest));
	vec2 startPos(0, 0);
	vec2 endPos((isTest ? c_TestGridW - 1 : c_GridW - 1), (isTest ? c_TestGridH - 1 : c_GridH - 1));

	std::cout << AStar(grid, startPos, endPos);
	return 0;
}

std::vector<vec2> AStarWithPathTracking(const Grid& grid, vec2 startPos, vec2 endPos)
{
	priority_queue<MazeTile> frontierTiles([](const MazeTile& a, const MazeTile& b) { return a.EstimatedTotalCost < b.EstimatedTotalCost; });
	std::vector<MazeTile> foundTiles({ MazeTile(startPos, OrthDirection::None, 0, Heuristic(startPos, endPos)) });

	OrthDirection dir = OrthDirection::Up;
	for (unsigned int i = 0; i < 4; ++i, ++dir)
	{
		vec2 nextTilePos = NextPos(startPos, dir);
		if (grid.IsValidPos(nextTilePos) && grid.at(nextTilePos) != '#')
		{
			MazeTile tile(nextTilePos, GetOppositeDir(dir), 1, 1 + Heuristic(nextTilePos, endPos));
			foundTiles.push_back(tile);
			frontierTiles.insert(tile);
		}
	}

	while (!frontierTiles.empty())
	{
		if (frontierTiles.front().Pos == endPos)
			break;

		MazeTile frontier = frontierTiles.front();
		frontierTiles.pop_front();

		for (MazeTile nextFrontier : Expand(grid, frontier.Pos, frontier.IncomingDir))
		{
			nextFrontier.CostToReach += frontier.CostToReach;
			nextFrontier.EstimatedTotalCost = nextFrontier.CostToReach + Heuristic(nextFrontier.Pos, endPos);
			auto foundIt = std::find_if(foundTiles.begin(), foundTiles.end(), [&nextFrontier](const MazeTile& tile)
				{ return nextFrontier.Pos == tile.Pos; }
			);

			if (foundIt == foundTiles.end())
			{
				foundTiles.push_back(nextFrontier);
				frontierTiles.insert(nextFrontier);
			}
			else if (nextFrontier.EstimatedTotalCost < foundIt->EstimatedTotalCost)
			{
				*foundIt = nextFrontier;
				frontierTiles.insert(nextFrontier);
			}
		}
	}

	if (!frontierTiles.empty())
	{
		MazeTile tile = frontierTiles.front();
		std::vector<vec2> ret({ tile.Pos });
		while (tile.IncomingDir != OrthDirection::None)
		{
			vec2 prevPos = NextPos(tile.Pos, tile.IncomingDir);
			tile = *std::find_if(foundTiles.cbegin(), foundTiles.cend(), [&prevPos](const MazeTile& t) { return t.Pos == prevPos; });
			ret.push_back(tile.Pos);
		}
		return ret;
	}
	else
		return std::vector<vec2>();
}

int Day18::Solution2()
{
	//bool isTest = true;
	bool isTest = false;
	unsigned int simulationSteps = (isTest ? 12 : 1024);
	std::vector<vec2> fallingBytes = FileUtil::ReadInputIntoVec<vec2>(__FILE__, isTest);

	Grid grid(BuildGrid(fallingBytes, simulationSteps, 0, isTest));
	vec2 startPos(0, 0);
	vec2 endPos((isTest ? c_TestGridW - 1 : c_GridW - 1), (isTest ? c_TestGridH - 1 : c_GridH - 1));

	std::vector<vec2> path = AStarWithPathTracking(grid, startPos, endPos);
	for (unsigned int i = simulationSteps; i < fallingBytes.size(); ++i)
	{
		vec2 nextByte = fallingBytes[i];
		grid[nextByte] = '#';

		if (std::find(path.begin(), path.end(), nextByte) != path.end())
		{
			path = AStarWithPathTracking(grid, startPos, endPos);
			if (path.empty())
			{
				std::cout << i << std::endl;
				std::cout << nextByte;
				return 0;
			}
		}
	}

	std::cout << "NaN,NaN";
	return 0;
}
