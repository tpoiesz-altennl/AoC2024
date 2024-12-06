#pragma once

enum class OrthDirection : unsigned int
{
	None = 0,
	Up,
	Right,
	Down,
	Left
};

OrthDirection& operator++(OrthDirection& dir);
OrthDirection& operator--(OrthDirection& dir);

class Day6
{
public:
	static int Solution1();
	static int Solution2();
};

