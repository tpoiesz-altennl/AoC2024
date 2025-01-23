#include "Day4.h"
#include "../Util.h"

int MakesXMASver1(const Grid& grid, const vec2 pos, const char currChar = 'X', const Direction dir = Direction::UpLeft)
{
	if (currChar != grid.at(pos))
		return 0;

	if (currChar == 'S')
		return 1;

	// Prune cases that run up against the grid bounds
	auto breakEarly = [=](const Direction d) -> bool {
		switch (d)
		{
		case Direction::UpLeft:
			return !grid.IsValidPos(vec2(pos.x - 3, pos.y - 3));
		case Direction::Up:
			return !grid.IsValidPos(vec2(pos.x, pos.y - 3));
		case Direction::UpRight:
			return !grid.IsValidPos(vec2(pos.x + 3, pos.y - 3));
		case Direction::Left:
			return !grid.IsValidPos(vec2(pos.x - 3, pos.y));
		case Direction::Right:
			return !grid.IsValidPos(vec2(pos.x + 3, pos.y));
		case Direction::DownLeft:
			return !grid.IsValidPos(vec2(pos.x - 3, pos.y + 3));
		case Direction::Down:
			return !grid.IsValidPos(vec2(pos.x, pos.y + 3));
		case Direction::DownRight:
			return !grid.IsValidPos(vec2(pos.x + 3, pos.y + 3));
		default:
			return true;
		}
	};

	if (currChar == 'X')
	{
		int totalXMASses = 0;
		for (Direction d = Direction::UpLeft; d != Direction::None; ++d)
		{
			if (breakEarly(d))
				continue;

			totalXMASses += MakesXMASver1(grid, NextPos(pos, d), 'M', d);
		}
		return totalXMASses;
	}
	else
	{
		switch (currChar)
		{
		case 'M':
			return MakesXMASver1(grid, NextPos(pos, dir), 'A', dir);
		case 'A':
			return MakesXMASver1(grid, NextPos(pos, dir), 'S', dir);
		}
	}
	// Should likely never hit
	return 0;
}

int Day4::Solution1ver1()
{
	//Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true));
	Grid grid(FileUtil::ReadInputIntoVec<std::string>(__FILE__));

	// Catch this edge case here, so we don't try to access a non-existent vector or string element
	if (grid.size() < 4 || grid[0].size() < 4)
		return 1;

	unsigned int totalXMASses = 0;
	for (unsigned int y = 0; y < grid.size(); ++y)
	{
		for (unsigned int x = 0; x < grid[0].size(); ++x)
		{
			totalXMASses += MakesXMASver1(grid, vec2(x, y));
		}
	}

	std::cout << totalXMASses << std::endl;

	return 0;
}

int MakesXMASver2(const Grid& grid, const Direction dir)
{
	unsigned int numXMAS = 0;
	switch (dir)
	{
	case Direction::UpLeft:
	{
		for (unsigned int y = 3; y < grid.size(); ++y)
		{
			for (unsigned int x = 3; x < grid.at(0).size(); ++x) // Assume rectangular grid
			{
				if (grid.at(x, y) == 'X' && grid.at(x - 1, y - 1) == 'M' &&
					grid.at(x - 2, y - 2) == 'A' && grid.at(x - 3, y - 3) == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::Up:
	{
		for (unsigned int y = 3; y < grid.size(); ++y)
		{
			for (unsigned int x = 0; x < grid.at(0).size(); ++x)
			{
				if (grid.at(x, y) == 'X' && grid.at(x, y - 1) == 'M' &&
					grid.at(x, y - 2) == 'A' && grid.at(x, y - 3) == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::UpRight:
	{
		for (unsigned int y = 3; y < grid.size(); ++y)
		{
			for (unsigned int x = 0; x < grid.at(0).size() - 3; ++x)
			{
				if (grid.at(x, y) == 'X' && grid.at(x + 1, y - 1) == 'M' &&
					grid.at(x + 2, y - 2) == 'A' && grid.at(x + 3, y - 3) == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::Left:
	{
		for (std::string row : grid)
		{
			std::reverse(row.begin(), row.end());
			size_t pos = row.find("XMAS", 0);
			while (pos != std::string::npos)
			{
				++numXMAS;
				pos = row.find("XMAS", pos + 1);
			}
		}
		break;
	}
	case Direction::Right:
	{
		for (const std::string& row : grid)
		{
			size_t pos = row.find("XMAS", 0);
			while (pos != std::string::npos)
			{
				++numXMAS;
				pos = row.find("XMAS", pos + 1);
			}
		}
		break;
	}
	case Direction::DownLeft:
	{
		for (unsigned int y = 0; y < grid.size() - 3; ++y)
		{
			for (unsigned int x = 3; x < grid.at(0).size(); ++x)
			{
				if (grid.at(x, y) == 'X' && grid.at(x - 1, y + 1) == 'M' &&
					grid.at(x - 2, y + 2) == 'A' && grid.at(x - 3, y + 3) == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::Down:
	{
		for (unsigned int y = 0; y < grid.size() - 3; ++y)
		{
			for (unsigned int x = 0; x < grid.at(0).size(); ++x)
			{
				if (grid.at(x, y) == 'X' && grid.at(x, y + 1) == 'M' &&
					grid.at(x, y + 2) == 'A' && grid.at(x, y + 3) == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::DownRight:
	{
		for (unsigned int y = 0; y < grid.size() - 3; ++y)
		{
			for (unsigned int x = 0; x < grid.at(0).size() - 3; ++x)
			{
				if (grid.at(x, y) == 'X' && grid.at(x + 1, y + 1) == 'M' &&
					grid.at(x + 2, y + 2) == 'A' && grid.at(x + 3, y + 3) == 'S')
					++numXMAS;
			}
		}
		break;
	}
	}
	return numXMAS;
}

int Day4::Solution1ver2()
{
	//std::vector<std::string> grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	std::vector<std::string> grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	// Catch this edge case here, so we don't try to access a non-existent vector or string element
	if (grid.size() < 4 || grid[0].size() < 4)
		return 1;

	unsigned int totalXMASses = 0;
	for (Direction d = Direction::UpLeft; d != Direction::None; ++d)
	{
		totalXMASses += MakesXMASver2(grid, d);
	}

	std::cout << totalXMASses << std::endl;

	return 0;
}

bool MakesMASX(const Grid& grid, const unsigned int indexX, const unsigned int indexY)
{
	if (grid.at(indexX + 1, indexY + 1) != 'A')
		return false;

	if (grid.at(indexX, indexY) == 'M')
	{
		if (grid.at(indexX + 2, indexY + 2) != 'S')
			return false;

		if (grid.at(indexX + 2, indexY) == 'M' && grid.at(indexX, indexY + 2) == 'S')
			return true;
		else if (grid.at(indexX + 2, indexY) == 'S' && grid.at(indexX, indexY + 2) == 'M')
			return true;
		else
			return false;
	}
	else if (grid.at(indexX, indexY) == 'S')
	{
		if (grid.at(indexX + 2, indexY + 2) != 'M')
			return false;

		if (grid.at(indexX + 2, indexY) == 'M' && grid.at(indexX, indexY + 2) == 'S')
			return true;
		else if (grid.at(indexX + 2, indexY) == 'S' && grid.at(indexX, indexY + 2) == 'M')
			return true;
		else
			return false;
	}
	else
		return false;
}

int Day4::Solution2()
{
	//Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	Grid grid = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	// Catch this edge case here, so we don't try to access a non-existent vector or string element
	if (grid.size() < 4 || grid[0].size() < 4)
		return 1;

	unsigned int totalXMASses = 0;
	for (unsigned int i = 0; i < grid.size() - 2; ++i)
	{
		for (unsigned int j = 0; j < grid[0].size() - 2; ++j)
		{
			totalXMASses += MakesMASX(grid, j, i);
		}
	}

	std::cout << totalXMASses << std::endl;

	return 0;
}
