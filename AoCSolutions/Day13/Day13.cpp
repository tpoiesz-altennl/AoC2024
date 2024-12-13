#include "Day13.h"
#include "../Util.h"

#include <regex>

std::vector<std::vector<vec2>> ParseFile(std::ifstream& file)
{
	std::vector<std::vector<vec2>> testCases;
	while (!file.eof())
	{
		std::string caseLine = "";
		testCases.push_back(std::vector<vec2>());
		for (unsigned int i = 0; i < 3; ++i)
		{
			std::getline(file, caseLine);
			std::regex findXY("\\d+");
			std::smatch match;
			std::regex_search(caseLine.cbegin(), caseLine.cend(), match, findXY);
			int X = std::stoi(match[0].str());
			std::regex_search(match.suffix().first, caseLine.cend(), match, findXY);
			int Y = std::stoi(match[0].str());
			testCases.back().push_back(vec2(X, Y));
		}
		// Skip empty line between testcases
		std::getline(file, caseLine);
	}
	return testCases;
}

std::vector<std::vector<vec2T<u64>>> ParseFileB(std::ifstream& file)
{
	std::vector<std::vector<vec2>> testCases = ParseFile(file);
	std::vector<std::vector<vec2T<u64>>> actualTestCases;
	for (auto& testCase : testCases)
	{
		actualTestCases.push_back({ 
			vec2T<u64>(testCase[0].x, testCase[0].y),
			vec2T<u64>(testCase[1].x, testCase[1].y),
			vec2T<u64>(static_cast<u64>(testCase[2].x) + 10000000000000, static_cast<u64>(testCase[2].y) + 10000000000000)
			});
	}
	return actualTestCases;
}

bool BruteForce(const std::vector<vec2>& testCase, vec2& match)
{
	for (unsigned int a = 0; a <= 100; ++a)
	{
		for (unsigned int b = 0; b <= 100; ++b)
		{
			if (testCase[0] * a + testCase[1] * b == testCase[2])
			{
				match = vec2(a, b);
				return true;
			}
		}
	}
	return false;
}

int Day13::Solution1ver1()
{
	std::ifstream file = FileUtil::OpenFile(__FILE__);
	std::vector<std::vector<vec2>> testCases = ParseFile(file);
	file.close();

	int totalCost = 0;
	for (const auto& testCase : testCases)
	{
		vec2 match;
		if (BruteForce(testCase, match))
		{
			totalCost += 3 * match.x + match.y;
		}
	}

	std::cout << totalCost;
	return 0;
}

bool isWhole(double num, double epsilon = 0.0001)
{
	double rounded = std::floor(num);
	return rounded == num || (rounded <= num + epsilon && rounded >= num - epsilon);
}

bool SolveEquationSystem(const std::vector<vec2>& testCase, vec2& solution)
{
	int x1 = testCase[0].x, y1 = testCase[0].y, x2 = testCase[1].x, y2 = testCase[1].y, xr = testCase[2].x, yr = testCase[2].y;
	int denom = x1 * y2 - y1 * x2;
	int aPart = y2 * xr - x2 * yr, bPart = x1 * yr - y1 * xr;

	if (denom == 0) // vectors are colinear
	{
		if (xr % x1 != 0)
			return false; // Non-integer solution

		bool useA = xr / x1 < 3 * xr / x2;
		if (useA)
			solution = vec2(xr / x1, 0);
		else
			solution = vec2(0, xr / x2);
		return true;
	}

	if (aPart % denom != 0 || bPart % denom != 0)
		return false; // Non-integer solution

	solution = vec2(aPart / denom, bPart / denom);
	return true;
}

int Day13::Solution1ver2()
{
	std::ifstream file = FileUtil::OpenFile(__FILE__);
	std::vector<std::vector<vec2>> testCases = ParseFile(file);
	file.close();

	int totalCost = 0;
	for (const auto& testCase : testCases)
	{
		vec2 match;
		if (SolveEquationSystem(testCase, match))
		{
			totalCost += 3 * match.x + match.y;
		}
	}

	std::cout << totalCost;
	return 0;
}

bool SolveEquationSystemu64(const std::vector<vec2T<u64>>& testCase, vec2T<u64>& solution)
{
	u64 x1 = testCase[0].x, y1 = testCase[0].y, x2 = testCase[1].x, y2 = testCase[1].y, xr = testCase[2].x, yr = testCase[2].y;
	int denom = static_cast<int>(x1 * y2 - y1 * x2);
	long long int aPart = y2 * xr - x2 * yr, bPart = x1 * yr - y1 * xr;

	if (denom == 0) // vectors are colinear
	{
		if (xr % x1 != 0)
			return false; // Non-integer solution

		bool useA = xr / x1 < 3 * xr / x2;
		if (useA)
			solution = vec2T<u64>(xr / x1, 0);
		else
			solution = vec2T<u64>(0, xr / x2);
		return true;
	}

	if (aPart % denom != 0 || bPart % denom != 0)
		return false; // Non-integer solution

	solution = vec2T<u64>(aPart / denom, bPart / denom);
	return true;
}

int Day13::Solution2()
{
	std::ifstream file = FileUtil::OpenFile(__FILE__);
	std::vector<std::vector<vec2T<u64>>> testCases = ParseFileB(file);
	file.close();

	u64 totalCost = 0;
	for (const auto& testCase : testCases)
	{
		vec2T<u64> match;
		if (SolveEquationSystemu64(testCase, match))
		{
			totalCost += 3 * match.x + match.y;
		}
	}

	std::cout << totalCost;
	return 0;
}
