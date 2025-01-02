#include "Util.h"

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

OrthDirection& operator++(OrthDirection& dir)
{
	switch (dir)
	{
	case OrthDirection::Up:
	{
		dir = OrthDirection::Right;
		return dir;
	}
	case OrthDirection::Right:
	{
		dir = OrthDirection::Down;
		return dir;
	}
	case OrthDirection::Down:
	{
		dir = OrthDirection::Left;
		return dir;
	}
	case OrthDirection::Left:
	default:
	{
		dir = OrthDirection::Up;
		return dir;
	}
	}
}

OrthDirection& operator--(OrthDirection& dir)
{
	switch (dir)
	{
	case OrthDirection::Up:
	{
		dir = OrthDirection::Left;
		return dir;
	}
	case OrthDirection::Right:
	{
		dir = OrthDirection::Up;
		return dir;
	}
	case OrthDirection::Down:
	{
		dir = OrthDirection::Right;
		return dir;
	}
	case OrthDirection::Left:
	default:
	{
		dir = OrthDirection::Down;
		return dir;
	}
	}
}

OrthDirection operator+(OrthDirection dir, int add)
{
	add %= 4;
	if (add < 0)
		add += 4;

	switch (add)
	{
	case 0:
		return dir;
	case 1:
		++dir;
		return dir;
	case 2:
		return GetOppositeDir(dir);
	case 3:
		--dir;
		return dir;
	default:
		return OrthDirection::None;
	}
}

OrthDirection operator-(OrthDirection dir, int subtract)
{
	subtract = 4 - subtract;
	return dir + subtract;
}

OrthDirection GetOppositeDir(OrthDirection dir)
{
	switch (dir)
	{
	case OrthDirection::Up:
		return OrthDirection::Down;
	case OrthDirection::Right:
		return OrthDirection::Left;
	case OrthDirection::Down:
		return OrthDirection::Up;
	case OrthDirection::Left:
		return OrthDirection::Right;
	default:
		return OrthDirection::None;
	}
}

std::istream& operator>>(std::istream& stream, vec2& operand)
{
	char separator;
	stream >> operand.x >> separator >> operand.y;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, vec2& operand)
{
	char separator = ',';
	stream << operand.x << separator << operand.y;
	return stream;
}

vec2 NextPos(const vec2& currPos, OrthDirection dir)
{
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
}

vec2 NextPos(const vec2& currPos, Direction dir)
{
	switch (dir)
	{
	case Direction::UpLeft:
		return vec2(currPos.x - 1, currPos.y - 1);
	case Direction::Up:
		return vec2(currPos.x, currPos.y - 1);
	case Direction::UpRight:
		return vec2(currPos.x + 1, currPos.y - 1);
	case Direction::Left:
		return vec2(currPos.x - 1, currPos.y);
	case Direction::Right:
		return vec2(currPos.x + 1, currPos.y);
	case Direction::DownLeft:
		return vec2(currPos.x - 1, currPos.y + 1);
	case Direction::Down:
		return vec2(currPos.x, currPos.y + 1);
	case Direction::DownRight:
		return vec2(currPos.x + 1, currPos.y + 1);
	default:
		return vec2(-1, -1);
	}
}
