#include "Day15.h"
#include "../Util.h"

void ParseFile(std::vector<std::string>& gridOutput, std::vector<std::string>& instructionsOutput, bool isTestFile = false)
{
	std::ifstream file = FileUtil::OpenFile(__FILE__, isTestFile);
	std::string temp("");
	while (std::getline(file, temp) && !temp.empty())
	{
		gridOutput.push_back(temp);
	}
	while (std::getline(file, temp) && !temp.empty())
	{
		instructionsOutput.push_back(temp);
	}
	file.close();
}

void ParseFileB(std::vector<std::string>& gridOutput, std::vector<std::string>& instructionsOutput, bool isTestFile = false)
{
	std::ifstream file = FileUtil::OpenFile(__FILE__, isTestFile);
	std::string temp("");
	while (std::getline(file, temp) && !temp.empty())
	{
		std::string line("");
		for (char c : temp)
		{
			switch (c)
			{
			case '#':
				line.append("##");
				break;
			case '.':
				line.append("..");
				break;
			case 'O':
				line.append("[]");
				break;
			default:
				line += c;
				line += '.';
			}
		}
		gridOutput.push_back(line);
	}
	while (std::getline(file, temp) && !temp.empty())
	{
		instructionsOutput.push_back(temp);
	}
	file.close();
}

vec2 FindRobot(const Grid& grid)
{
	for (unsigned int y = 0; y < grid.size(); ++y)
	{
		for (unsigned int x = 0; x < grid.at(y).size(); ++x)
		{
			vec2 pos(x, y);
			if (grid.at(pos) == '@')
				return pos;
		}
	}
	return vec2(-1, -1);
}

bool IsWidePushPossible(const Grid& grid, const vec2& pushedPos, OrthDirection dir)
{
	if (grid.at(pushedPos) == '#')
		return false;
	else if (grid.at(pushedPos) == '.')
		return true;
	else if (grid.at(pushedPos) == '[')
	{
		switch (dir)
		{
		case OrthDirection::Left:
			return IsWidePushPossible(grid, vec2(pushedPos.x - 1, pushedPos.y), dir);
		case OrthDirection::Right:
			return IsWidePushPossible(grid, vec2(pushedPos.x + 2, pushedPos.y), dir);
		case OrthDirection::Up:
		{
			vec2 nextLeftPos(pushedPos.x, pushedPos.y - 1);
			vec2 nextRightPos(pushedPos.x + 1, pushedPos.y - 1);
			return IsWidePushPossible(grid, nextLeftPos, dir) && IsWidePushPossible(grid, nextRightPos, dir);
		}
		case OrthDirection::Down:
		{
			vec2 nextLeftPos(pushedPos.x, pushedPos.y + 1);
			vec2 nextRightPos(pushedPos.x + 1, pushedPos.y + 1);
			return IsWidePushPossible(grid, nextLeftPos, dir) && IsWidePushPossible(grid, nextRightPos, dir);
		}
		default:
			return false;
		}
	}
	else if (grid.at(pushedPos) == ']')
	{
		switch (dir)
		{
		case OrthDirection::Left:
			return IsWidePushPossible(grid, vec2(pushedPos.x - 2, pushedPos.y), dir);
		case OrthDirection::Right:
			return IsWidePushPossible(grid, vec2(pushedPos.x + 1, pushedPos.y), dir);
		case OrthDirection::Up:
		{
			vec2 nextLeftPos(pushedPos.x - 1, pushedPos.y - 1);
			vec2 nextRightPos(pushedPos.x, pushedPos.y - 1);
			return IsWidePushPossible(grid, nextLeftPos, dir) && IsWidePushPossible(grid, nextRightPos, dir);
		}
		case OrthDirection::Down:
		{
			vec2 nextLeftPos(pushedPos.x - 1, pushedPos.y + 1);
			vec2 nextRightPos(pushedPos.x, pushedPos.y + 1);
			return IsWidePushPossible(grid, nextLeftPos, dir) && IsWidePushPossible(grid, nextRightPos, dir);
		}
		default:
			return false;
		}
	}
	else
		return false;
}

void ExecuteWidePush(Grid& grid, const vec2& pushedPos, OrthDirection dir)
{
	if (grid[pushedPos] == '[')
	{
		switch (dir)
		{
		case OrthDirection::Left:
		{
			vec2 nextPos(pushedPos.x - 1, pushedPos.y);
			ExecuteWidePush(grid, nextPos, dir);
			grid[nextPos] = '[';
			return;
		}
		case OrthDirection::Right:
		{
			vec2 nextPos(pushedPos.x + 1, pushedPos.y);
			ExecuteWidePush(grid, nextPos, dir);
			grid[nextPos] = '[';
			return;
		}
		case OrthDirection::Up:
		{
			vec2 nextLeftPos(pushedPos.x, pushedPos.y - 1);
			vec2 rightHalf(pushedPos.x + 1, pushedPos.y);
			ExecuteWidePush(grid, nextLeftPos, dir);
			grid[nextLeftPos] = '[';
			grid[pushedPos] = '.';
			ExecuteWidePush(grid, rightHalf, dir);
			return;
		}
		case OrthDirection::Down:
		{
			vec2 nextLeftPos(pushedPos.x, pushedPos.y + 1);
			vec2 rightHalf(pushedPos.x + 1, pushedPos.y);
			ExecuteWidePush(grid, nextLeftPos, dir);
			grid[nextLeftPos] = '[';
			grid[pushedPos] = '.';
			ExecuteWidePush(grid, rightHalf, dir);
			return;
		}
		}
	}
	else if (grid[pushedPos] == ']')
	{
		switch (dir)
		{
		case OrthDirection::Left:
		{
			vec2 nextPos(pushedPos.x - 1, pushedPos.y);
			ExecuteWidePush(grid, nextPos, dir);
			grid[nextPos] = ']';
			return;
		}
		case OrthDirection::Right:
		{
			vec2 nextPos(pushedPos.x + 1, pushedPos.y);
			ExecuteWidePush(grid, nextPos, dir);
			grid[nextPos] = ']';
			return;
		}
		case OrthDirection::Up:
		{
			vec2 leftHalf(pushedPos.x - 1, pushedPos.y);
			vec2 nextRightPos(pushedPos.x, pushedPos.y - 1);
			ExecuteWidePush(grid, nextRightPos, dir);
			grid[nextRightPos] = ']';
			grid[pushedPos] = '.';
			ExecuteWidePush(grid, leftHalf, dir);
			return;
		}
		case OrthDirection::Down:
		{
			vec2 leftHalf(pushedPos.x - 1, pushedPos.y);
			vec2 nextRightPos(pushedPos.x, pushedPos.y + 1);
			ExecuteWidePush(grid, nextRightPos, dir);
			grid[nextRightPos] = ']';
			grid[pushedPos] = '.';
			ExecuteWidePush(grid, leftHalf, dir);
			return;
		}
		}
	}
}

bool Push(Grid& grid, const vec2& pushedPos, OrthDirection dir)
{
	if (grid[pushedPos] == '#')
		return false;
	else if (grid[pushedPos] == '[' || grid[pushedPos] == ']')
	{
		if (IsWidePushPossible(grid, pushedPos, dir))
		{
			ExecuteWidePush(grid, pushedPos, dir);
			return true;
		}
	}
	else
	{
		vec2 nextPos = NextPos(pushedPos, dir);
		if (grid.IsValidPos(nextPos))
		{
			if (grid[nextPos] == '#')
				return false;
			else if (grid[nextPos] == 'O')
				return Push(grid, nextPos, dir);
			else if (grid[nextPos] == '.')
			{
				grid[nextPos] = 'O';
				return true;
			}
		}
	}
	return false;
}

void MoveRobot(Grid& grid, vec2& robotPos, OrthDirection dir)
{
	vec2 nextPos = NextPos(robotPos, dir);
	if (grid[nextPos] != '.')
	{
		if (Push(grid, nextPos, dir))
		{
			grid[robotPos] = '.';
			grid[nextPos] = '@';
			robotPos = nextPos;
		}
	}
	else
	{
		grid[robotPos] = '.';
		grid[nextPos] = '@';
		robotPos = nextPos;
	}
}

int Day15::Solution1()
{
	std::vector<std::string> gridInput, instructions;
	ParseFile(gridInput, instructions);
	//ParseFile(gridInput, instructions, true);
	Grid grid(gridInput);

	vec2 robotPos = FindRobot(grid);
	for (const std::string& instructionLine : instructions)
	{
		for (const char instruction : instructionLine)
		{
			switch (instruction)
			{
			case '^':
				MoveRobot(grid, robotPos, OrthDirection::Up);
				break;
			case '>':
				MoveRobot(grid, robotPos, OrthDirection::Right);
				break;
			case 'v':
				MoveRobot(grid, robotPos, OrthDirection::Down);
				break;
			case '<':
				MoveRobot(grid, robotPos, OrthDirection::Left);
				break;
			}
		}
	}

	unsigned int totalGPS = 0;
	// Ignore grid borders
	for (unsigned int y = 1; y < grid.size() - 1; ++y)
	{
		for (unsigned int x = 1; x < grid.at(y).size() - 1; ++x)
		{
			vec2 pos(x, y);
			if (grid[pos] == 'O')
				totalGPS += 100 * y + x;
		}
	}

	std::cout << totalGPS;
	return 0;
}

int Day15::Solution2()
{
	std::vector<std::string> gridInput, instructions;
	//ParseFileB(gridInput, instructions);
	ParseFileB(gridInput, instructions);
	Grid grid(gridInput);

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputResultGrid(grid.grid);

	vec2 robotPos = FindRobot(grid);
	for (const std::string& instructionLine : instructions)
	{
		for (const char instruction : instructionLine)
		{
			switch (instruction)
			{
			case '^':
				MoveRobot(grid, robotPos, OrthDirection::Up);
				break;
			case '>':
				MoveRobot(grid, robotPos, OrthDirection::Right);
				break;
			case 'v':
				MoveRobot(grid, robotPos, OrthDirection::Down);
				break;
			case '<':
				MoveRobot(grid, robotPos, OrthDirection::Left);
				break;
			}
			//dbg.Overwrite();
			//dbg.OutputResultGrid(grid.grid);
		}
	}
	dbg.OutputResultGrid(grid.grid);

	unsigned int totalGPS = 0;
	// Ignore grid borders
	for (unsigned int y = 1; y < grid.size() - 1; ++y)
	{
		for (unsigned int x = 1; x < grid.at(y).size() - 1; ++x)
		{
			vec2 pos(x, y);
			if (grid[pos] == '[')
				totalGPS += 100 * y + x;
		}
	}

	std::cout << totalGPS;
	return 0;
}
