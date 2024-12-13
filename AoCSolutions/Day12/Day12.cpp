#include "Day12.h"
#include "../Util.h"

#include <vector>
#include <string>
#include <algorithm>

bool ScanPatch(const Grid& grid, char toSearch, const vec2& pos, int& patchSize, int& numConnections, std::vector<vec2>& foundPositions)
{
	if (grid.at(pos) != toSearch)
		return false;

	else if (std::find(foundPositions.begin(), foundPositions.end(), pos) != foundPositions.end())
		return true;

	foundPositions.push_back(pos);
	patchSize += 1;

	auto getNextPos = [](const vec2& currPos, OrthDirection dir) -> vec2 {
		switch (dir)
		{
		case OrthDirection::Up:
			return vec2(currPos.x, currPos.y - 1);
		case OrthDirection::Right:
			return vec2(currPos.x + 1, currPos.y);
		case OrthDirection::Down:
			return vec2(currPos.x, currPos.y + 1);
		case OrthDirection::Left:
			return vec2(currPos.x - 1, currPos.y);
		default:
			return vec2(-1, -1);
		}
		};

	OrthDirection dir = OrthDirection::Up;
	for (unsigned int i = 0; i < 4; ++i, ++dir)
	{
		vec2 nextPos = getNextPos(pos, dir);
		if (nextPos.x < 0 || nextPos.x >= grid.at(0).size() || nextPos.y < 0 || nextPos.y >= grid.size())
			continue;

		if (ScanPatch(grid, toSearch, nextPos, patchSize, numConnections, foundPositions))
			numConnections += 1;
	}

	return true;
}

int Day12::Solution1ver1()
{
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));

	int totalCost = 0;
	for (unsigned int y = 0; y < grid.size(); ++y)
	{
		for (unsigned int x = 0; x < grid.at(0).size(); ++x)
		{
			if (char c = grid.at(x, y); c != '.')
			{
				int patchSize = 0, numConnections = 0;
				std::vector<vec2> foundPositions;
				ScanPatch(grid, c, vec2(x, y), patchSize, numConnections, foundPositions);
				for (const vec2& pos : foundPositions)
				{
					grid[pos] = '.';
				}
				int perimeter = patchSize * 4 - numConnections;
				totalCost += patchSize * perimeter;
			}
		}
	}

	//std::cout << totalCost << std::endl;

	return 0;
}

bool ScanPatch2(const Grid& grid, char toSearch, const vec2& pos, int& patchSize, int& numConnections, 
	std::vector<vec2>& foundPositions, OrthDirection prevDir = OrthDirection::None)
{
	if (grid.at(pos) != toSearch)
		return false;

	else if (std::find(foundPositions.begin(), foundPositions.end(), pos) != foundPositions.end())
		return true;

	foundPositions.push_back(pos);
	patchSize += 1;

	auto getNextPos = [](const vec2& currPos, OrthDirection dir, OrthDirection& cameFromDir) -> vec2 {
		switch (dir)
		{
		case OrthDirection::Up:
			cameFromDir = OrthDirection::Down;
			return vec2(currPos.x, currPos.y - 1);
		case OrthDirection::Right:
			cameFromDir = OrthDirection::Left;
			return vec2(currPos.x + 1, currPos.y);
		case OrthDirection::Down:
			cameFromDir = OrthDirection::Up;
			return vec2(currPos.x, currPos.y + 1);
		case OrthDirection::Left:
			cameFromDir = OrthDirection::Right;
			return vec2(currPos.x - 1, currPos.y);
		default:
			return vec2(-1, -1);
		}
		};

	OrthDirection nextDir = prevDir;
	++nextDir;
	OrthDirection nextIncoming;
	if (prevDir == OrthDirection::None)
	{
		for (unsigned int i = 0; i < 4; ++i, ++nextDir)
		{
			
			vec2 nextPos = getNextPos(pos, nextDir, nextIncoming);
			if (nextPos.x < 0 || nextPos.x >= grid.at(0).size() || nextPos.y < 0 || nextPos.y >= grid.size())
				continue;

			if (ScanPatch2(grid, toSearch, nextPos, patchSize, numConnections, foundPositions, nextIncoming))
				numConnections += 1;
		}
	}
	else
	{
		numConnections += 1;
		for (; nextDir != prevDir; ++nextDir)
		{
			vec2 nextPos = getNextPos(pos, nextDir, nextIncoming);
			if (nextPos.x < 0 || nextPos.x >= grid.at(0).size() || nextPos.y < 0 || nextPos.y >= grid.size())
				continue;

			if (ScanPatch2(grid, toSearch, nextPos, patchSize, numConnections, foundPositions, nextIncoming))
				numConnections += 1;
		}
	}

	return true;
}

int Day12::Solution1ver2()
{
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));

	int totalCost = 0;
	for (unsigned int y = 0; y < grid.size(); ++y)
	{
		for (unsigned int x = 0; x < grid.at(0).size(); ++x)
		{
			if (char c = grid.at(x, y); c != '.')
			{
				int patchSize = 0, numConnections = 0;
				std::vector<vec2> foundPositions;
				ScanPatch2(grid, c, vec2(x, y), patchSize, numConnections, foundPositions);
				for (const vec2& pos : foundPositions)
				{
					grid[pos] = '.';
				}
				int perimeter = patchSize * 4 - numConnections;
				totalCost += patchSize * perimeter;
			}
		}
	}

	//std::cout << totalCost << std::endl;

	return 0;
}

std::vector<std::vector<vec2>> BuildShape(std::vector<vec2> positions, bool groupByY)
{
	std::sort(positions.begin(), positions.end(), [=](const vec2& a, const vec2& b) -> bool {
		if (groupByY)
		{
			if (a.y != b.y)
				return a.y < b.y;
			else
				return a.x < b.x;
		}
		else
		{
			if (a.x != b.x)
				return a.x < b.x;
			else
				return a.y < b.y;
		}
		});
	std::vector<std::vector<vec2>> shape;
	int prevXY = -1;
	unsigned int currXYIdx = -1;
	for (const vec2& pos : positions)
	{
		if (prevXY != (groupByY ? pos.y : pos.x))
		{
			shape.push_back(std::vector<vec2>({ pos }));
			++currXYIdx;
			prevXY = groupByY ? pos.y : pos.x;
		}
		else
		{
			shape[currXYIdx].push_back({ pos });
		}
	}
	return shape;
}

int DetermineNumEdges(std::vector<vec2>& positions, Testing::DebugFile* dbg = nullptr)
{
	auto isTopEdge = [](const std::vector<std::vector<vec2>>& shape, const unsigned int row, const vec2& pos, bool checkY) -> bool {
		vec2 posToCheck = checkY ? vec2(pos.x, pos.y - 1) : vec2(pos.x - 1, pos.y);
		return row == 0 || std::find(shape[row - 1].begin(), shape[row - 1].end(), posToCheck) == shape[row - 1].end();
		};

	auto isBottomEdge = [](const std::vector<std::vector<vec2>>& shape, const unsigned int row, const vec2& pos, bool checkY) -> bool {
		vec2 posToCheck = checkY ? vec2(pos.x, pos.y + 1) : vec2(pos.x + 1, pos.y);
		return row == shape.size() - 1 || std::find(shape[row + 1].begin(), shape[row + 1].end(), posToCheck) == shape[row + 1].end();
		};

	int numEdges = 0;
	// Determine vertical edges, then mirror shape along diagonal axis and determine horizontal edges
	for (unsigned int i = 0; i < 2; ++i)
	{
		std::vector<std::vector<vec2>> shape = BuildShape(positions, i);
		for (unsigned int row = 0; row < shape.size(); ++row)
		{
			vec2 pos = shape[row][0];
			// topEdge == we are at the top row or there is no "tile" right above the current one
			bool topEdge = isTopEdge(shape, row, pos, i);
			bool bottomEdge = isBottomEdge(shape, row, pos, i);
			auto it = shape[row].begin(), next = it + 1;
			while (next != shape[row].end())
			{
				if ((i ? (it->x + 1 == next->x) : (it->y + 1 == next->y)))
				{
					bool nextTopEdge = isTopEdge(shape, row, *next, i);
					if (topEdge && !nextTopEdge)
						numEdges += 1;
					topEdge = nextTopEdge;
					bool nextBottomEdge = isBottomEdge(shape, row, *next, i);
					if (bottomEdge && !nextBottomEdge)
						numEdges += 1;
					bottomEdge = nextBottomEdge;
				}
				else // a discontinuity means we could have an extra edge
				{
					if (topEdge)
						numEdges += 1;
					topEdge = isTopEdge(shape, row, *next, i);
					if (bottomEdge)
						numEdges += 1;
					bottomEdge = isBottomEdge(shape, row, *next, i);
				}
				++it;
				++next;
			}
			if (topEdge)
				numEdges += 1;
			if (bottomEdge)
				numEdges += 1;
		}
		if (dbg)
		{
			std::vector<std::string> debugShape;
			if (i)
			{
				int minX = std::min_element(positions.begin(), positions.end(), [](const vec2& a, const vec2& b) { return a.x < b.x;} )->x;
				int maxX = std::max_element(positions.begin(), positions.end(), [](const vec2& a, const vec2& b) { return a.x < b.x;} )->x;
				int minY = shape[0][0].y;
				int maxY = shape[shape.size() - 1][0].y;

				for (int i = 0; i <= maxY - minY; ++i)
				{
					std::string row = "";
					for (int j = minX; j <= maxX; ++j)
					{
						if (std::find(shape[i].begin(), shape[i].end(), vec2(j, minY + i)) != shape[i].end())
							row += 'O';
						else
							row += '.';
					}
					debugShape.push_back(row);
				}
			}
			else
			{
				int minX = shape[0][0].x;
				int maxX = shape[shape.size() - 1][0].x;
				int minY = std::min_element(positions.begin(), positions.end(), [](const vec2& a, const vec2& b) { return a.y < b.y;})->y;
				int maxY = std::max_element(positions.begin(), positions.end(), [](const vec2& a, const vec2& b) { return a.y < b.y;})->y;

				for (int i = 0; i <= maxX - minX; ++i)
				{
					std::string row = "";
					for (int j = minY; j <= maxY; ++j)
					{
						if (std::find(shape[i].begin(), shape[i].end(), vec2(minX + i, j)) != shape[i].end())
							row += 'O';
						else
							row += '.';
					}
					debugShape.push_back(row);
				}
			}
			dbg->OutputResultGrid<std::string>(debugShape);
			__debugbreak();
		}
	}

	return numEdges;
}

int Day12::Solution2()
{
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));

	int totalCost = 0;
	Testing::DebugFile dbg(__FILE__);
	for (unsigned int y = 0; y < grid.size(); ++y)
	{
		for (unsigned int x = 0; x < grid.at(0).size(); ++x)
		{
			if (char c = grid.at(x, y); c != '.')
			{
				int patchSize = 0, dummy = 0;
				std::vector<vec2> foundPositions;
				ScanPatch(grid, c, vec2(x, y), patchSize, dummy, foundPositions);
				int numEdges = DetermineNumEdges(foundPositions/*, &dbg*/);
				for (const vec2& pos : foundPositions)
				{
					grid[pos] = '.';
				}
				totalCost += patchSize * numEdges;
			}
		}
	}

	std::cout << totalCost;

	return 0;
}
