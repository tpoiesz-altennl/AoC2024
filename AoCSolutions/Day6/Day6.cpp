#include "Day6.h"
#include "../Util.h"

#include <vector>
#include <string>
#include <regex>

OrthDirection FindGuard(const std::vector<std::string>& initialMap, int& posX, int& posY)
{
	std::regex reg("[\\^>v<]");
	for (unsigned int y = 0; y < initialMap.size(); ++y)
	{
		std::smatch m;
		if (std::regex_search(initialMap[y], m, reg))
		{
			posX = static_cast<int>(m.prefix().length());
			posY = y;
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
bool MoveGuard(std::vector<std::string>& map, int& numSteps, int& guardPosX, int& guardPosY, OrthDirection& dir, bool countDuplicateSteps = false)
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
		{
			if (countDuplicateSteps)
				++numSteps;
			return true;
		}
		case '#':
		default:
			return false;
		}
		};

	auto getGuardChar = [&](OrthDirection dir) -> char {
		switch (dir)
		{
		case OrthDirection::Up:
		{
			return '^';
		}
		case OrthDirection::Right:
		{
			return '>';
		}
		case OrthDirection::Down:
		{
			return 'v';
		}
		case OrthDirection::Left:
		default:
		{
			return '<';
		}
		}
		};

	switch (dir)
	{
	case OrthDirection::Up:
	{
		if (guardPosY == 0)
		{
			++numSteps;
			map[guardPosY][guardPosX] = 'X';
			return false;
		}
		else
		{
			if (takeStep(map[guardPosY - 1][guardPosX]))
			{
				map[guardPosY][guardPosX] = 'X';
				guardPosY -= 1;
				map[guardPosY][guardPosX] = getGuardChar(dir);
				return true;
			}
			else
			{
				map[guardPosY][guardPosX] = getGuardChar(++dir);
				return true;
			}
		}
	}
	case OrthDirection::Right:
	{
		if (guardPosX == map[guardPosY].length() - 1)
		{
			++numSteps;
			return false;
		}
		else
		{
			if (takeStep(map[guardPosY][guardPosX + 1]))
			{
				map[guardPosY][guardPosX] = 'X';
				guardPosX += 1;
				map[guardPosY][guardPosX] = getGuardChar(dir);
				return true;
			}
			else
			{
				map[guardPosY][guardPosX] = getGuardChar(++dir);
				return true;
			}
		}
	}
	case OrthDirection::Down:
	{
		if (guardPosY == map.size() - 1)
		{
			++numSteps;
			return false;
		}
		else
		{
			if (takeStep(map[guardPosY + 1][guardPosX]))
			{
				map[guardPosY][guardPosX] = 'X';
				guardPosY += 1;
				map[guardPosY][guardPosX] = getGuardChar(dir);
				return true;
			}
			else
			{
				map[guardPosY][guardPosX] = getGuardChar(++dir);
				return true;
			}
		}
	}
	case OrthDirection::Left:
	default:
	{
		if (guardPosX == 0)
		{
			++numSteps;
			return false;
		}
		else
		{
			if (takeStep(map[guardPosY][guardPosX - 1]))
			{
				map[guardPosY][guardPosX] = 'X';
				guardPosX -= 1;
				map[guardPosY][guardPosX] = getGuardChar(dir);
				return true;
			}
			else
			{
				map[guardPosY][guardPosX] = getGuardChar(++dir);
				return true;
			}
		}
	}
	}
}

int Day6::Solution1()
{
	//std::vector<std::string> map = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	std::vector<std::string> input = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	int guardPosX, guardPosY;
	OrthDirection initDir = FindGuard(input, guardPosX, guardPosY);

	int numSteps = 0;
	while (MoveGuard(input, numSteps, guardPosX, guardPosY, initDir)) {}

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputResultGrid<std::string>(input);

	std::cout << numSteps;
	return 0;
}

bool GuardLoops(std::vector<std::string> map, int posX, int posY, OrthDirection dir, Testing::DebugFile* dbg = nullptr)
{
	int pathLength = 0;
	int upPathLength = -1, rightPathLength = -1, downPathLength = -1, leftPathLength = -1;
	OrthDirection prevDir = dir;
	bool onGrid = true;

	while (true) // always returns within body, either by hitting a loop or running off the map
	{
		while (prevDir == dir && onGrid)
			onGrid = MoveGuard(map, pathLength, posX, posY, dir);
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
					dbg->OutputResultGrid(map);
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
	//std::vector<std::string> map = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	std::vector<std::string> map = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	int guardPosX, guardPosY;
	OrthDirection dir = FindGuard(map, guardPosX, guardPosY);

	Testing::DebugFile dbg(__FILE__);
	int dummy = 0;
	int numPossibleObstacles = 0;
	do
	{
		int nextX = guardPosX, nextY = guardPosY;
		switch (dir)
		{
		case OrthDirection::Up:
		{
			nextY -= 1;
			break;
		}
		case OrthDirection::Right:
		{
			nextX += 1;
			break;
		}
		case OrthDirection::Down:
		{
			nextY += 1;
			break;
		}
		case OrthDirection::Left:
		{
			nextX -= 1;
			break;
		}
		}
		
		auto validGridPos = [&](int posX, int posY) -> bool {
			return (posY >= 0 && posY < map.size() && posX >= 0 && posX < map[0].size());
			};

		if (validGridPos(nextX, nextY) && map[nextY][nextX] != '#' && map[nextY][nextX] != 'X')
		{
			char prevChar = map[nextY][nextX];
			map[nextY][nextX] = '#';
			++dir;
			//if (GuardLoops(map, guardPosX, guardPosY, dir, &dbg))
			if (GuardLoops(map, guardPosX, guardPosY, dir))
			{
				++numPossibleObstacles;
			}
			map[nextY][nextX] = prevChar;
			--dir;
		}

	} while (MoveGuard(map, dummy, guardPosX, guardPosY, dir));

	std::cout << numPossibleObstacles;
	return 0;
}
