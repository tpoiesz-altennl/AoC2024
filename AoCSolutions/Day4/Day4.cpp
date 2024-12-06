#include "Day4.h"
#include "../Util.h"

Direction& operator++(Direction& dir)
{
	switch (dir)
	{
	case Direction::UpLeft:
	{
		dir = Direction::Up;
		return dir;
	}
	case Direction::Up:
	{
		dir = Direction::UpRight;
		return dir;
	}
	case Direction::UpRight:
	{
		dir = Direction::Left;
		return dir;
	}
	case Direction::Left:
	{
		dir = Direction::Right;
		return dir;
	}
	case Direction::Right:
	{
		dir = Direction::DownLeft;
		return dir;
	}
	case Direction::DownLeft:
	{
		dir = Direction::Down;
		return dir;
	}
	case Direction::Down:
	{
		dir = Direction::DownRight;
		return dir;
	}
	case Direction::DownRight:
	default:
	{
		dir = Direction::None; // end at None
		return dir;
	}
	}
}

int MakesXMASver1(const std::vector<std::string>& grid, const unsigned int indexX, const unsigned int indexY, 
	const char currChar = 'X', const Direction dir = Direction::UpLeft)
{
	if (currChar != grid[indexY][indexX])
		return 0;

	if (currChar == 'S')
		return 1;

	// Prune cases that run up against the grid bounds
	auto breakEarly = [=](const Direction d) -> bool {
		switch (d)
		{
		case Direction::UpLeft:
		{
			if (indexX < 3 || indexY < 3)
				return true;
			break;
		}
		case Direction::Up:
		{
			if (indexY < 3)
				return true;
			break;
		}
		case Direction::UpRight:
		{
			if (grid[0].size() < 4 || indexX > grid[0].size() - 4 || indexY < 3)
				return true;
			break;
		}
		case Direction::Left:
		{
			if (indexX < 3)
				return true;
			break;
		}
		case Direction::Right:
		{
			if (grid[0].size() < 4 || indexX > grid[0].size() - 4)
				return true;
			break;
		}
		case Direction::DownLeft:
		{
			if (indexX < 3 || grid.size() < 4 || indexY > grid.size() - 4)
				return true;
			break;
		}
		case Direction::Down:
		{
			if (grid.size() < 4 || indexY > grid.size() - 4)
				return true;
			break;
		}
		case Direction::DownRight:
		{
			if (grid[0].size() < 4 || indexX > grid[0].size() - 4 || grid.size() < 4 || indexY > grid.size() - 4)
				return true;
			break;
		}
		default:
			return true;
		}

		return false;
	};

	auto determineNextPos = [=](unsigned int& x, unsigned int& y, const Direction d) {
		switch (d)
		{
		case Direction::UpLeft:
		{
			x = indexX - 1;
			y = indexY - 1;
			break;
		}
		case Direction::Up:
		{
			x = indexX;
			y = indexY - 1;
			break;
		}
		case Direction::UpRight:
		{
			x = indexX + 1;
			y = indexY - 1;
			break;
		}
		case Direction::Left:
		{
			x = indexX - 1;
			y = indexY;
			break;
		}
		case Direction::Right:
		{
			x = indexX + 1;
			y = indexY;
			break;
		}
		case Direction::DownLeft:
		{
			x = indexX - 1;
			y = indexY + 1;
			break;
		}
		case Direction::Down:
		{
			x = indexX;
			y = indexY + 1;
			break;
		}
		case Direction::DownRight:
		{
			x = indexX + 1;
			y = indexY + 1;
			break;
		}
		}
	};

	if (currChar == 'X')
	{
		int totalXMASses = 0;
		for (Direction d = Direction::UpLeft; d != Direction::None; ++d)
		{
			if (breakEarly(d))
			{
				continue;
			}

			char nextChar = 'M';
			unsigned int nextIndexX = -1, nextIndexY = -1;
			determineNextPos(nextIndexX, nextIndexY, d);
			totalXMASses += MakesXMASver1(grid, nextIndexX, nextIndexY, nextChar, d);
		}
		return totalXMASses;
	}
	else
	{
		unsigned int nextIndexX = -1, nextIndexY = -1;
		determineNextPos(nextIndexX, nextIndexY, dir);
		char nextChar = '0';
		switch (currChar)
		{
		case 'M':
		{
			nextChar = 'A';
			return MakesXMASver1(grid, nextIndexX, nextIndexY, nextChar, dir);
		}
		case 'A':
		{
			nextChar = 'S';
			return MakesXMASver1(grid, nextIndexX, nextIndexY, nextChar, dir);
		}
		}
	}
	// Should likely never hit
	return 0;
}

int Day4::Solution1ver1()
{
	//std::vector<std::string> grid = FileUtil::ReadTestFileIntoVec<std::string>(__FILE__);
	std::vector<std::string> grid = FileUtil::ReadInputFileIntoVec<std::string>(__FILE__);

	// Catch this edge case here, so we don't try to access a non-existent vector or string element
	if (grid.size() < 4 || grid[0].size() < 4)
		return 1;

	unsigned int totalXMASses = 0;
	for (unsigned int i = 0; i < grid.size(); ++i)
	{
		for (unsigned int j = 0; j < grid[0].size(); ++j)
		{
			totalXMASses += MakesXMASver1(grid, j, i);
		}
	}

	//std::cout << totalXMASses;

	return 0;
}

int MakesXMASver2(const std::vector<std::string>& grid, const Direction dir)
{
	unsigned int numXMAS = 0;
	switch (dir)
	{
	case Direction::UpLeft:
	{
		for (unsigned int y = 3; y < grid.size(); ++y)
		{
			for (unsigned int x = 3; x < grid[0].size(); ++x) // Assume rectangular grid
			{
				if (grid[y][x] == 'X' && grid[y - 1][x - 1] == 'M' &&
					grid[y - 2][x - 2] == 'A' && grid[y - 3][x - 3] == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::Up:
	{
		for (unsigned int y = 3; y < grid.size(); ++y)
		{
			for (unsigned int x = 0; x < grid[0].size(); ++x)
			{
				if (grid[y][x] == 'X' && grid[y - 1][x] == 'M' &&
					grid[y - 2][x] == 'A' && grid[y - 3][x] == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::UpRight:
	{
		for (unsigned int y = 3; y < grid.size(); ++y)
		{
			for (unsigned int x = 0; x < grid[0].size() - 3; ++x)
			{
				if (grid[y][x] == 'X' && grid[y - 1][x + 1] == 'M' &&
					grid[y - 2][x + 2] == 'A' && grid[y - 3][x + 3] == 'S')
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
		for (std::string row : grid)
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
			for (unsigned int x = 3; x < grid[0].size(); ++x)
			{
				if (grid[y][x] == 'X' && grid[y + 1][x - 1] == 'M' &&
					grid[y + 2][x - 2] == 'A' && grid[y + 3][x - 3] == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::Down:
	{
		for (unsigned int y = 0; y < grid.size() - 3; ++y)
		{
			for (unsigned int x = 0; x < grid[0].size(); ++x)
			{
				if (grid[y][x] == 'X' && grid[y + 1][x] == 'M' &&
					grid[y + 2][x] == 'A' && grid[y + 3][x] == 'S')
					++numXMAS;
			}
		}
		break;
	}
	case Direction::DownRight:
	{
		for (unsigned int y = 0; y < grid.size() - 3; ++y)
		{
			for (unsigned int x = 0; x < grid[0].size() - 3; ++x)
			{
				if (grid[y][x] == 'X' && grid[y + 1][x + 1] == 'M' &&
					grid[y + 2][x + 2] == 'A' && grid[y + 3][x + 3] == 'S')
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
	//std::vector<std::string> grid = FileUtil::ReadTestFileIntoVec<std::string>(__FILE__);
	std::vector<std::string> grid = FileUtil::ReadInputFileIntoVec<std::string>(__FILE__);

	// Catch this edge case here, so we don't try to access a non-existent vector or string element
	if (grid.size() < 4 || grid[0].size() < 4)
		return 1;

	unsigned int totalXMASses = 0;
	for (Direction d = Direction::UpLeft; d != Direction::None; ++d)
	{
		totalXMASses += MakesXMASver2(grid, d);
	}

	//std::cout << totalXMASses;

	return 0;
}

bool MakesMASX(const std::vector<std::string>& grid, const unsigned int indexX, const unsigned int indexY)
{
	if (grid[indexY + 1][indexX + 1] != 'A')
		return false;

	if (grid[indexY][indexX] == 'M')
	{
		if (grid[indexY + 2][indexX + 2] != 'S')
			return false;

		if (grid[indexY][indexX + 2] == 'M' && grid[indexY + 2][indexX] == 'S')
			return true;
		else if (grid[indexY][indexX + 2] == 'S' && grid[indexY + 2][indexX] == 'M')
			return true;
		else
			return false;
	}
	else if (grid[indexY][indexX] == 'S')
	{
		if (grid[indexY + 2][indexX + 2] != 'M')
			return false;

		if (grid[indexY][indexX + 2] == 'M' && grid[indexY + 2][indexX] == 'S')
			return true;
		else if (grid[indexY][indexX + 2] == 'S' && grid[indexY + 2][indexX] == 'M')
			return true;
		else
			return false;
	}
	else
		return false;
}

int Day4::Solution2()
{
	//std::vector<std::string> grid = FileUtil::ReadTestFileIntoVec<std::string>(__FILE__);
	std::vector<std::string> grid = FileUtil::ReadInputFileIntoVec<std::string>(__FILE__);

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

	std::cout << totalXMASses;

	return 0;
}
