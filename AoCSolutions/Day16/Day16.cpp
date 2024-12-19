#include "Day16.h"
#include "../Util.h"

#include <thread>
#include <deque>
#include <algorithm>

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
		if (grid.IsValidPos(checkPos) && (grid.at(checkPos) == '.' || grid.at(checkPos) == 'E'))
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

	/*std::cout << std::find_if(nodes.begin(), nodes.end(), [&endPos](const MazeNode& node) {
		return node.Pos == endPos;
		})->CostToReach;*/
	return 0;
}

struct MazeNode2
{
	vec2 Pos;
	OrthDirection IncomingDir;
	unsigned int CostToReach;
	unsigned int EstimatedTotalCost;

	MazeNode2() : Pos(vec2()), IncomingDir(OrthDirection::None), CostToReach(-1), EstimatedTotalCost(-1)
	{ }

	MazeNode2(vec2 pos, OrthDirection dir, unsigned int cost, unsigned int costPlusHeuristic)
		: Pos(pos), IncomingDir(dir), CostToReach(cost), EstimatedTotalCost(costPlusHeuristic)
	{ }

	MazeNode2(MazeNode base, unsigned int costPlusHeuristic)
		: Pos(base.Pos), IncomingDir(base.IncomingDir), CostToReach(base.CostToReach), EstimatedTotalCost(costPlusHeuristic)
	{ }
};

struct priority_queue
{
	priority_queue(std::function<bool(const MazeNode2& a, const MazeNode2& b)> comp)
		: Queue(std::deque<MazeNode2>()), Comparator(comp)
	{ }

	std::deque<MazeNode2> Queue;
	std::function<bool(const MazeNode2& a, const MazeNode2& b)> Comparator;

	std::deque<MazeNode2>::iterator insert(MazeNode2 element)
	{
		auto it = Queue.begin();
		while (it != Queue.end() && Comparator(*it, element))
		{
			++it;
		}
		return Queue.insert(it, element);
	}

	MazeNode2& operator[](unsigned int index) { return Queue[index]; }
	const MazeNode2& front() const { return Queue.front(); }
	const MazeNode2& back() const { return Queue.back(); }
	void push_back(const MazeNode2& val) { Queue.push_back(val); }
	void push_front(const MazeNode2& val) { Queue.push_front(val); }
	void pop_front() { Queue.pop_front(); }
	void pop_back() { Queue.pop_back(); }
	std::deque<MazeNode2>::iterator erase(std::deque<MazeNode2>::iterator where) { return Queue.erase(where); }
	bool empty() { return Queue.empty(); }
	std::deque<MazeNode2>::iterator begin() { return Queue.begin(); }
	std::deque<MazeNode2>::iterator end() { return Queue.end(); }
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

bool AStarExpansion(const Grid& grid, priority_queue& frontierNodes,
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
			auto it = std::find_if(frontierNodes.begin(), frontierNodes.end(), [&node2](const MazeNode2& n) {
				return node2.Pos == n.Pos;
				});
			if (it != frontierNodes.end())
				*it = node2;
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
	priority_queue frontierNodes([](const MazeNode2& a, const MazeNode2& b) { return a.EstimatedTotalCost < b.EstimatedTotalCost;});
	frontierNodes.push_back(nodes.back());

	while (!frontierNodes.empty())
	{
		if (AStarExpansion(grid, frontierNodes, nodes, endPos))
			break;
	}

	/*std::cout << std::find_if(nodes.begin(), nodes.end(), [&endPos](const MazeNode2& node2) {
		return node2.Pos == endPos;
		})->CostToReach;*/
	return 0;
}

int Day16::Solution2()
{
	return 0;
}
