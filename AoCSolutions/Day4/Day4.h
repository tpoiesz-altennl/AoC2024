#pragma once

enum class Direction : unsigned int
{
	None = 0,
	UpLeft,
	Up,
	UpRight,
	Left,
	Right,
	DownLeft,
	Down,
	DownRight
};

Direction& operator++(Direction& dir);

class Day4
{
public:
	static int Solution1ver1();
	static int Solution1ver2();
	static int Solution2();
};
