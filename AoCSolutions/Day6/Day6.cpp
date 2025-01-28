#include "Day6.h"
#include "../Util.h"

#include <vector>
#include <string>
#include <regex>

OrthDirection FindGuard(const Grid& initialMap, vec2& pos)
{
	std::regex reg("[\\^>v<]");
	for (unsigned int y = 0; y < initialMap.size(); ++y)
	{
		std::smatch m;
		if (std::regex_search(initialMap.at(y), m, reg))
		{
			pos.x = static_cast<int>(m.prefix().length());
			pos.y = y;
			switch (m[0].str()[0])
			{
			case '^':
				return OrthDirection::Up;
			case '>':
				return OrthDirection::Right;
			case 'v':
				return OrthDirection::Down;
			case '<':
				return OrthDirection::Left;
			default:
				return OrthDirection::None;
			}
		}
	}
	return OrthDirection::None;
}

// Returns false if guard walks off the map
bool MoveGuard(Grid& map, int& numSteps, vec2& guardPos, OrthDirection& dir)
{
	auto takeStep = [=, &numSteps](char nextPos) mutable -> bool {
		switch (nextPos)
		{
		case '.':
		{
			++numSteps;
			return true;
		}
		case 'X':
			return true;
		case '#':
		default:
			return false;
		}
		};

	auto getGuardChar = [&](OrthDirection dir) -> char {
		switch (dir)
		{
		case OrthDirection::Up:
			return '^';
		case OrthDirection::Right:
			return '>';
		case OrthDirection::Down:
			return 'v';
		case OrthDirection::Left:
		default:
			return '<';
		}
		};

	if (!map.IsValidPos(NextPos(guardPos, dir)))
	{
		++numSteps;
		map[guardPos] = 'X';
		return false;
	}
	else
	{
		if (takeStep(map[NextPos(guardPos, dir)]))
		{
			map[guardPos] = 'X';
			guardPos = NextPos(guardPos, dir);
			map[guardPos] = getGuardChar(dir);
		}
		else
		{
			map[guardPos] = getGuardChar(++dir);
		}
		return true;
	}
}

int Day6::Solution1()
{
	//Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	vec2 guardPos;
	OrthDirection initDir = FindGuard(grid, guardPos);

	int numSteps = 0;
	while (MoveGuard(grid, numSteps, guardPos, initDir)) {}

	/*Testing::DebugFile dbg(__FILE__);
	dbg.OutputGrid<std::string>(grid.container);*/

	std::cout << numSteps;
	return 0;
}

bool GuardLoops(Grid map, vec2 pos, OrthDirection dir, Testing::DebugFile* dbg = nullptr)
{
	int pathLength = 0;
	int upPathLength = -1, rightPathLength = -1, downPathLength = -1, leftPathLength = -1;
	OrthDirection prevDir = dir;
	bool onGrid = true;

	while (true) // always returns within body, either by hitting a loop or running off the map
	{
		while (prevDir == dir && onGrid)
			onGrid = MoveGuard(map, pathLength, pos, dir);
		if (onGrid)
		{
			switch (prevDir)
			{
			case OrthDirection::Up:
			{
				upPathLength = pathLength;
				break;
			}
			case OrthDirection::Right:
			{
				rightPathLength = pathLength;
				break;
			}
			case OrthDirection::Down:
			{
				downPathLength = pathLength;
				break;
			}
			case OrthDirection::Left:
			default:
			{
				leftPathLength = pathLength;
				break;
			}
			}

			// MoveGuard does not count spaces that have been moved over before, so this condition hits when the guard has only passed over
			// spaces that she moved over before (which constitutes a loop)
			if (upPathLength == 0 && rightPathLength == 0 && downPathLength == 0 && leftPathLength == 0)
			{
				if (dbg)
				{
					dbg->Overwrite();
					dbg->OutputGrid(map);
					__debugbreak(); // Break program so we can see the output (can safely continue afterwards)
				}
				return true;
			}

			pathLength = 0;
			prevDir = dir;
		}
		else
			return false;
	}
}

int Day6::Solution2()
{
	//Grid map = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	Grid map = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	vec2 guardPos;
	OrthDirection dir = FindGuard(map, guardPos);

	Testing::DebugFile dbg(__FILE__);
	int dummy = 0;
	int numPossibleObstacles = 0;
	do
	{
		vec2 nextPos = NextPos(guardPos, dir);
		if (map.IsValidPos(nextPos) && map[nextPos] != '#' && map[nextPos] != 'X')
		{
			char prevChar = map[nextPos];
			map[nextPos] = '#';
			++dir;
			//if (GuardLoops(map, guardPosX, guardPosY, dir, &dbg))
			if (GuardLoops(map, guardPos, dir))
			{
				++numPossibleObstacles;
			}
			map[nextPos] = prevChar;
			--dir;
		}

	} while (MoveGuard(map, dummy, guardPos, dir));

	std::cout << numPossibleObstacles;
	return 0;
}
