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
