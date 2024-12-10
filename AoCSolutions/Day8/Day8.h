#pragma once
#include <tuple>

struct vec2
{
	int x, y;

	vec2() : x(0), y(0)
	{ }
	vec2(int _x, int _y) : x(_x), y(_y)
	{ }

	vec2 operator+(const vec2& other)
	{
		return vec2(x + other.x, y + other.y);
	}
	vec2 operator-(const vec2& other)
	{
		return vec2(x - other.x, y - other.y);
	}

	vec2 operator*(int a)
	{
		return vec2(x * a, y * a);
	}
};

class Day8
{
public:
	static int Solution1();
	static int Solution2();
};
