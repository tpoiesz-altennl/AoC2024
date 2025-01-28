#include "Day13.h"
#include "../Util.h"

#include <regex>

struct ClawMachine
{
	vec2 ButtonA;
	vec2 ButtonB;
	vec2 Prize;

	ClawMachine()
	{ }

	ClawMachine(vec2 a, vec2 b, vec2 prize)
		: ButtonA(a), ButtonB(b), Prize(prize)
	{ }
};

std::vector<ClawMachine> ParseFile(std::ifstream& file)
{
	std::vector<ClawMachine> testCases;
	while (!file.eof())
	{
		std::string caseLine = "";
		std::vector<vec2> testCase;
		for (unsigned int i = 0; i < 3; ++i)
		{
			std::getline(file, caseLine);
			std::regex findXY("\\d+");
			std::smatch match;
			std::regex_search(caseLine.cbegin(), caseLine.cend(), match, findXY);
			int X = std::stoi(match[0].str());
			std::regex_search(match.suffix().first, caseLine.cend(), match, findXY);
			int Y = std::stoi(match[0].str());
			testCase.push_back(vec2(X, Y));
		}
		testCases.push_back(ClawMachine(testCase[0], testCase[1], testCase[2]));
		// Skip empty line between testcases
		std::getline(file, caseLine);
	}
	return testCases;
}

struct ClawMachineB
{
	vec2 ButtonA;
	vec2 ButtonB;
	vec2T<u64> Prize;

	ClawMachineB()
	{
	}

	ClawMachineB(vec2 a, vec2 b, vec2T<u64> prize)
		: ButtonA(a), ButtonB(b), Prize(prize)
	{
	}
};

std::vector<ClawMachineB> ParseFileB(std::ifstream& file)
{
	std::vector<ClawMachine> testCases = ParseFile(file);
	std::vector<ClawMachineB> actualTestCases;
	for (const auto& testCase : testCases)
	{
		actualTestCases.push_back(ClawMachineB(testCase.ButtonA, testCase.ButtonB,
			vec2T<u64>(static_cast<u64>(testCase.Prize.x) + 10000000000000, static_cast<u64>(testCase.Prize.y) + 10000000000000)));
	}
	return actualTestCases;
}

bool BruteForce(const ClawMachine& testCase, vec2& match)
{
	for (unsigned int a = 0; a <= 100; ++a)
	{
		for (unsigned int b = 0; b <= 100; ++b)
		{
			if (testCase.ButtonA * a + testCase.ButtonB * b == testCase.Prize)
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
	std::vector<ClawMachine> testCases = ParseFile(file);
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

bool SolveEquationSystem(const ClawMachine& testCase, vec2& solution)
{
	int x1 = testCase.ButtonA.x, y1 = testCase.ButtonA.y,
		x2 = testCase.ButtonB.x, y2 = testCase.ButtonB.y,
		xr = testCase.Prize.x, yr = testCase.Prize.y;
	int denom = x1 * y2 - y1 * x2;
	int aPart = y2 * xr - x2 * yr,
		bPart = x1 * yr - y1 * xr;

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
	std::vector<ClawMachine> testCases = ParseFile(file);
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

bool SolveEquationSystemu64(const ClawMachineB& testCase, vec2T<u64>& solution)
{
	int x1 = testCase.ButtonA.x, y1 = testCase.ButtonA.y,
		x2 = testCase.ButtonB.x, y2 = testCase.ButtonB.y;
	u64 xr = testCase.Prize.x, yr = testCase.Prize.y;
	int denom = x1 * y2 - y1 * x2;
	long long int aPart = y2 * xr - x2 * yr,
		bPart = x1 * yr - y1 * xr;

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
	std::vector<ClawMachineB> testCases = ParseFileB(file);
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
