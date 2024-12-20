#include "Day16.h"
#include "../Util.h"

#include <thread>
#include <deque>
#include <algorithm>
#include <set>

void BruteForce(Grid& grid, vec2 currPos, OrthDirection incomingDir,
	u64& lowestPathScore, u64& currentPathScore)
{
	if (grid.at(currPos) == 'E')
	{
		currentPathScore += 1;
		if (currentPathScore < lowestPathScore)
			lowestPathScore = currentPathScore;
		currentPathScore -= 1;
		return;
	}
	else if (grid.at(currPos) != '.')
	{
		return;
	}
	else if (currentPathScore >= lowestPathScore)
	{
		return;
	}

	// Mark position as visited
	grid[currPos] = 'X';
	currentPathScore += 1;
	OrthDirection nextDir = GetOppositeDir(incomingDir);
	++nextDir;
	for (; nextDir != GetOppositeDir(incomingDir); ++nextDir)
	{
		if (nextDir != incomingDir)
			currentPathScore += 1000;
		vec2 nextPos = NextPos(currPos, nextDir);
		if (grid.IsValidPos(currPos))
		{
			BruteForce(grid, nextPos, nextDir, lowestPathScore, currentPathScore);
		}
		if (nextDir != incomingDir)
			currentPathScore -= 1000;
		//currPos = NextPos(currPos, GetOppositeDir(nextDir));
	}
	grid[currPos] = '.';
	currentPathScore -= 1;
}

int Day16::Solution1ver1()
{
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));
	//Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true));
	vec2 startPos = grid.FindChar('S');
	if (!grid.IsValidPos(startPos))
		return 1;

	std::vector<std::thread> threads;
	OrthDirection dir = OrthDirection::Right;
	std::vector<u64> currentScores(4, 0);
	std::vector<u64> lowestScores(4, -1);
	//u64 lowestScore = -1;
	for (unsigned int i = 0; i < 4; ++i, ++dir)
	{
		vec2 nextPos = NextPos(startPos, dir);
		//u64 score = 0;
		if (dir != OrthDirection::Right)
		{
			if (dir == OrthDirection::Left)
				currentScores[i] += 2000;
			else
				currentScores[i] += 1000;
		}
		//MoveReindeer(grid, nextPos, dir, lowestScore, score);
		threads.push_back(std::thread(&BruteForce, std::ref(grid), nextPos, dir, std::ref(lowestScores[i]), std::ref(currentScores[i])));
	}

	for (auto& thread : threads)
		thread.join();

	auto it = std::min_element(lowestScores.begin(), lowestScores.end());

	std::cout << *it;
	return 0;
}

struct MazeNode
{
	vec2 Pos;
	OrthDirection IncomingDir;
	unsigned int CostToReach;

	MazeNode() : Pos(vec2()), IncomingDir(OrthDirection::None), CostToReach(-1)
	{ }

	MazeNode(vec2 pos, OrthDirection dir, unsigned int cost)
		: Pos(pos), IncomingDir(dir), CostToReach(cost)
	{ }
};

bool PathFindToNode(const Grid& grid, const MazeNode& start, MazeNode& found)
{
	if (grid.at(start.Pos) == 'E')
	{
		found.Pos = start.Pos;
		found.IncomingDir = start.IncomingDir;
		found.CostToReach += 1;
		return true;
	}
	else if (grid.at(start.Pos) == '#' || grid.at(start.Pos) == 'S')
	{
		return false;
	}

	MazeNode next(vec2(), OrthDirection::None, 0);
	unsigned int numBranches = 0;
	for (OrthDirection checkDir = start.IncomingDir + 1; checkDir != start.IncomingDir; ++checkDir)
	{
		vec2 checkPos = NextPos(start.Pos, checkDir);
		if (grid.IsValidPos(checkPos) && (grid.at(checkPos) == '.' || grid.at(checkPos) == 'X' || grid.at(checkPos) == 'E'))
		{
			numBranches += 1;
			// This will be relevant if there's only one path onward
			next.Pos = checkPos;
			next.IncomingDir = GetOppositeDir(checkDir);
		}
	}
	if (numBranches > 1) // Found a crossroads node
	{
		found.Pos = start.Pos;
		found.IncomingDir = start.IncomingDir;
		found.CostToReach += 1;
		return true;
	}
	else if (numBranches < 1) // Dead end
	{
		return false;
	}
	else // Continue pathfinding
	{
		found.CostToReach += 1;
		if (next.IncomingDir != start.IncomingDir)
			found.CostToReach += 1000;
		return PathFindToNode(grid, next, found);
	}
}

std::vector<MazeNode> FindNextNodes(const Grid& grid, const MazeNode& startingNode)
{
	std::vector<MazeNode> ret;

	for (OrthDirection dir = startingNode.IncomingDir + 1; dir != startingNode.IncomingDir; ++dir)
	{
		vec2 nextPos = NextPos(startingNode.Pos, dir);
		OrthDirection incomingDir = GetOppositeDir(dir);
		MazeNode foundNode(vec2(), OrthDirection::None, 0);
		if (grid.IsValidPos(nextPos) && PathFindToNode(grid, MazeNode(nextPos, incomingDir, 0), foundNode))
		{
			if (incomingDir != startingNode.IncomingDir)
				foundNode.CostToReach += 1000;
			ret.push_back(std::move(foundNode));
		}
	}

	return ret;
}

void ForwardExpansion(const Grid& grid, std::deque<MazeNode>& frontierNodes, 
	std::vector<MazeNode>& nodes, const vec2& endPos)
{
	MazeNode frontierNode = frontierNodes.front();
	if (frontierNode.Pos == endPos)
	{
		// Don't expand forward from the end position
		frontierNodes.pop_front();
		return;
	}

	std::vector<MazeNode> nextNodes = FindNextNodes(grid, frontierNode);
	for (auto& node : nextNodes)
	{
		node.CostToReach += frontierNode.CostToReach;
		auto it = std::find_if(nodes.begin(), nodes.end(), [&node](const MazeNode& n) {
			return node.Pos == n.Pos;
			});

		if (it == nodes.end())
		{
			frontierNodes.push_back(node);
			nodes.push_back(std::move(node));
		}
		else if (it->CostToReach > node.CostToReach)
		{
			it->CostToReach = node.CostToReach;
			it->IncomingDir = node.IncomingDir;
			auto it = std::find_if(frontierNodes.begin(), frontierNodes.end(), [&node](const MazeNode& n) {
				return node.Pos == n.Pos;
				});
			if (it != frontierNodes.end())
				*it = node;
			else
				frontierNodes.push_back(node);
		}
	}
	frontierNodes.pop_front();
}

int Day16::Solution1ver2()
{
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));
	//Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true));
	vec2 startPos = grid.FindChar('S');
	vec2 endPos = grid.FindChar('E');
	if (!(grid.IsValidPos(startPos) && grid.IsValidPos(endPos)))
		return 1;
	std::vector<MazeNode> nodes({ MazeNode(startPos, OrthDirection::Left, 0) });
	std::deque<MazeNode> frontierNodes({ nodes.back() });

	while (!frontierNodes.empty())
	{
		ForwardExpansion(grid, frontierNodes, nodes, endPos);
	}

	std::cout << std::find_if(nodes.begin(), nodes.end(), [&endPos](const MazeNode& node) {
		return node.Pos == endPos;
		})->CostToReach;
	return 0;
}

struct MazeNode2 : public MazeNode
{
	unsigned int EstimatedTotalCost;

	MazeNode2() : MazeNode(vec2(), OrthDirection::None, -1), EstimatedTotalCost(-1)
	{ }

	MazeNode2(vec2 pos, OrthDirection dir, unsigned int cost, unsigned int costPlusHeuristic)
		: MazeNode(pos, dir, cost), EstimatedTotalCost(costPlusHeuristic)
	{ }

	MazeNode2(MazeNode base, unsigned int costPlusHeuristic)
		: MazeNode(base), EstimatedTotalCost(costPlusHeuristic)
	{ }
};

unsigned int Heuristic(const vec2& from, OrthDirection incomingDir, const vec2& to)
{
	unsigned int xDiff = std::abs(to.x - from.x), yDiff = std::abs(to.y - from.y);
	unsigned int cost = xDiff + yDiff;
	if (!cost) // At destination
		return cost;
	else if (xDiff == 0) // Vertically aligned positions
	{
		OrthDirection dirToDestination = to.y - from.y > 0 ? OrthDirection::Down : OrthDirection::Up;
		if (dirToDestination == incomingDir)
			cost += 2000; // Would need to do a 180 degree turn
		else if (dirToDestination == OrthDirection::Left || dirToDestination == OrthDirection::Right)
			cost += 1000; // Path involves 90 degree turn
	}
	else if (yDiff == 0) // Horizonally aligned positions
	{
		OrthDirection dirToDestination = to.x - from.x > 0 ? OrthDirection::Right : OrthDirection::Left;
		if (dirToDestination == incomingDir)
			cost += 2000; // Would need to do a 180 degree turn
		else if (dirToDestination == OrthDirection::Up || dirToDestination == OrthDirection::Down)
			cost += 1000; // Path involves 90 degree turn
	}
	else
	{
		OrthDirection verticalDirToDestination = to.y - from.y > 0 ? OrthDirection::Down : OrthDirection::Up;
		OrthDirection horizontalDirToDestination = to.x - from.x > 0 ? OrthDirection::Right : OrthDirection::Left;
		if (verticalDirToDestination == incomingDir || horizontalDirToDestination == incomingDir)
			cost += 2000;
		else
			cost += 1000;
	}
	return cost;
}

bool AStarExpansion(const Grid& grid, priority_queue<MazeNode2>& frontierNodes,
	std::vector<MazeNode2>& nodes, const vec2& endPos)
{
	MazeNode2 frontierNode = frontierNodes.front();
	if (frontierNode.Pos == endPos)
	{
		// We're done searching
		return true;
	}
	frontierNodes.pop_front();

	std::vector<MazeNode> nextNodes = FindNextNodes(grid, MazeNode(frontierNode.Pos, frontierNode.IncomingDir, frontierNode.CostToReach));
	for (auto& node : nextNodes)
	{
		node.CostToReach += frontierNode.CostToReach;
		MazeNode2 node2 = MazeNode2(node, Heuristic(node.Pos, node.IncomingDir, endPos) + node.CostToReach);
		auto it = std::find_if(nodes.begin(), nodes.end(), [&node2](const MazeNode2& n) {
			return node2.Pos == n.Pos;
			});

		if (it == nodes.end())
		{
			frontierNodes.insert(node2);
			nodes.push_back(std::move(node2));
		}
		else if (it->CostToReach > node2.CostToReach)
		{
			it->CostToReach = node2.CostToReach;
			it->IncomingDir = node2.IncomingDir;
			it->EstimatedTotalCost = node2.EstimatedTotalCost;
			auto frontierIt = std::find_if(frontierNodes.begin(), frontierNodes.end(), [&node2](const MazeNode2& n) {
				return node2.Pos == n.Pos;
				});
			if (frontierIt != frontierNodes.end())
				*frontierIt = node2;
			else
				frontierNodes.insert(node2);
		}
	}
	return false;
}

int Day16::Solution1ver3()
{
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));
	//Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true));
	vec2 startPos = grid.FindChar('S');
	vec2 endPos = grid.FindChar('E');
	if (!(grid.IsValidPos(startPos) && grid.IsValidPos(endPos)))
		return 1;
	std::vector<MazeNode2> nodes({ MazeNode2(startPos, OrthDirection::Left, 0, Heuristic(startPos, OrthDirection::Left, endPos)) });
	priority_queue<MazeNode2> frontierNodes([](const MazeNode2& a, const MazeNode2& b) { return a.EstimatedTotalCost < b.EstimatedTotalCost;});
	frontierNodes.push_back(nodes.back());

	while (!frontierNodes.empty())
	{
		if (AStarExpansion(grid, frontierNodes, nodes, endPos))
			break;
	}

	std::cout << std::find_if(nodes.begin(), nodes.end(), [&endPos](const MazeNode2& node2) {
		return node2.Pos == endPos;
		})->CostToReach;
	return 0;
}

struct MazeNode3 : public MazeNode2
{
	std::shared_ptr<MazeNode3> PreviousNode;

	MazeNode3() : MazeNode2(vec2(), OrthDirection::None, -1, -1), PreviousNode(nullptr)
	{ }

	MazeNode3(vec2 pos, OrthDirection dir, unsigned int cost, unsigned int costPlusHeuristic)
		: MazeNode2(pos, dir, cost, costPlusHeuristic), PreviousNode(nullptr)
	{
	}

	MazeNode3(vec2 pos, OrthDirection dir, unsigned int cost, unsigned int costPlusHeuristic, std::shared_ptr<MazeNode3> link)
		: MazeNode2(pos, dir, cost, costPlusHeuristic), PreviousNode(link)
	{ }
};

bool AStarWithMultipleBests(const Grid& grid, priority_queue<MazeNode3>& frontierNodes,
	std::vector<MazeNode3>& nodes, const vec2& endPos)
{
	MazeNode3 frontierNode = frontierNodes.front();
	if (frontierNode.Pos == endPos)
	{
		// Found a best path
		return true;
	}
	frontierNodes.pop_front();

	std::vector<MazeNode> nextNodes = FindNextNodes(grid, MazeNode(frontierNode.Pos, frontierNode.IncomingDir, frontierNode.CostToReach));
	for (auto& node : nextNodes)
	{
		node.CostToReach += frontierNode.CostToReach;
		auto prevNode = std::find_if(nodes.begin(), nodes.end(), [&frontierNode](const MazeNode3& n) {
			return frontierNode.Pos == n.Pos;
			});
		MazeNode3& prevNodeRef = *prevNode;
		auto prevNodePtr = std::shared_ptr<MazeNode3>(&prevNodeRef);
		MazeNode3 node3(node.Pos, node.IncomingDir, node.CostToReach, 
			Heuristic(node.Pos, node.IncomingDir, endPos) + node.CostToReach, prevNodePtr);

		auto it = std::find_if(nodes.begin(), nodes.end(), [&node3](const MazeNode3& n) {
			return node3.Pos == n.Pos;
			});

		if (it == nodes.end())
		{
			frontierNodes.insert(node3);
			nodes.push_back(node3);
		}
		else if (it->CostToReach > node3.CostToReach)
		{
			*it = node3;
			auto frontierIt = std::find_if(frontierNodes.begin(), frontierNodes.end(), [&node3](const MazeNode3& n) {
				return node3.Pos == n.Pos;
				});
			if (frontierIt != frontierNodes.end())
				*frontierIt = node3;
			else
				frontierNodes.insert(node3);
		}
		else if (it->CostToReach == node3.CostToReach)
		{
			frontierNodes.insert(node3, [](const MazeNode3& a, const MazeNode3& b) {
				return a.EstimatedTotalCost <= b.EstimatedTotalCost;
				});
		}
	}
	return false;
}

bool PathFindNodeToNode(const Grid& grid, const MazeNode& from, const MazeNode& to, unsigned int maxCost, unsigned int currCost,
	std::vector<vec2>& pathPositions)
{
	if (grid.at(from.Pos) == '#')
		return false;
	else if (currCost > maxCost)
		return false;
	else if (from.Pos == to.Pos)
	{
		pathPositions.push_back(from.Pos);
		return true;
	}

	currCost += 1;
	for (OrthDirection dir = from.IncomingDir + 1; dir != from.IncomingDir; ++dir)
	{
		MazeNode next(NextPos(from.Pos, dir), GetOppositeDir(dir), 0);
		if (next.IncomingDir != from.IncomingDir)
			currCost += 1000;
		if (PathFindNodeToNode(grid, next, to, maxCost, currCost, pathPositions))
		{
			pathPositions.push_back(from.Pos);
			return true;
		}
		if (next.IncomingDir != from.IncomingDir)
			currCost -= 1000;
	}

	return false;
}

void DrawPath(Grid& grid, std::deque<MazeNode3>& path, priority_queue<MazeNode3>& remainingFrontiers, Testing::DebugFile* dbg = nullptr)
{
	if (path.size() < 2)
		return;

	std::vector<std::deque<MazeNode3>> equivalentPaths;
	auto from = path.begin(), to = from + 1;
	while (to != path.end())
	{
		auto frontierIt = std::find_if(remainingFrontiers.begin(), remainingFrontiers.end(), [&to](const MazeNode3& n) {
			return to->Pos == n.Pos /*&& to->CostToReach == n.CostToReach*/;
			});
		if (frontierIt != remainingFrontiers.end())
		{
			MazeNode3 node = *frontierIt;
			std::deque<MazeNode3> path({ node });
			while (node.PreviousNode != nullptr)
			{
				node = *(node.PreviousNode);
				path.push_front(node);
			}
			equivalentPaths.push_back(path);
		}
		std::vector<vec2> pathTiles;
		PathFindNodeToNode(grid, *from, *to, to->CostToReach - from->CostToReach, 0, pathTiles);
		for (const vec2& tile : pathTiles)
			grid[tile] = 'X';
		++from, ++to;
	}
	
	for (std::deque<MazeNode3> path : equivalentPaths)
	{
		auto from = path.begin(), to = from + 1;
		while (to != path.end())
		{
			std::vector<vec2> pathTiles;
			PathFindNodeToNode(grid, *from, *to, to->CostToReach - from->CostToReach, 0, pathTiles);
			for (const vec2& tile : pathTiles)
				grid[tile] = 'X';
			++from, ++to;
		}
	}

	if (dbg)
		dbg->OutputResultGrid(grid.grid);
}

static constexpr unsigned int c_MaxNodes = 141 * 141;
static std::vector<MazeNode3> g_Nodes(c_MaxNodes, MazeNode3());
static unsigned int g_BackIndex = 0;

int Day16::Solution2()
{
	//Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true));
	vec2 startPos = grid.FindChar('S');
	vec2 endPos = grid.FindChar('E');
	if (!(grid.IsValidPos(startPos) && grid.IsValidPos(endPos)))
		return 1;
	std::vector<MazeNode3> nodes({ MazeNode3(startPos, OrthDirection::Left, 0, Heuristic(startPos, OrthDirection::Left, endPos)) });
	priority_queue<MazeNode3> frontierNodes([](const MazeNode2& a, const MazeNode2& b) { return a.EstimatedTotalCost <= b.EstimatedTotalCost; });
	frontierNodes.push_back(nodes.back());

	bool foundPath = false;
	while (!frontierNodes.empty() && !foundPath)
	{
		if (AStarWithMultipleBests(grid, frontierNodes, nodes, endPos))
		{
			MazeNode3 node = frontierNodes.front();
			std::deque<MazeNode3> path({ node });
			while (node.PreviousNode != nullptr)
			{
				node = *(node.PreviousNode);
				path.push_front(node);
			}
			Testing::DebugFile dbg(__FILE__);
			DrawPath(grid, path, frontierNodes, &dbg);
			foundPath = true;
		}
	}

	unsigned int totalTiles = 0;
	for (unsigned int y = 1; y < grid.size() - 1; ++y)
	{
		for (unsigned int x = 1; x < grid[y].size() - 1; ++x)
		{
			if (grid.at(x, y) == 'X' || grid.at(x, y) == 'S' || grid.at(x, y) == 'E')
				totalTiles += 1;
		}
	}

	std::cout << totalTiles;

	return 0;
}
