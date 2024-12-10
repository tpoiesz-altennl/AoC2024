#pragma once
#include "../Util.h"

class Day10
{
public:
	static int Solution1();
	static int Solution2();
private:
	static std::vector<vec2> GetCharCoordinates(const std::vector<std::string>& input, char val);
};

