#include "Day11.h"
#include "../Util.h"

#include <sstream>
#include <thread>
#include <deque>

bool ApplyRule1(u64& stone)
{
	if (stone != 0)
	{
		return false;
	}
	else
	{
		stone = 1;
		return true;
	}
}

bool ApplyRule2(u64& stone, u64& newStone)
{
	std::stringstream ss;
	ss << stone;
	size_t numDigits = ss.str().length();
	if (numDigits % 2 != 0)
	{
		return false;
	}
	else
	{
		std::string temp = "";
		for (size_t j = 0; j < numDigits / 2; ++j)
		{
			temp += ss.get();
		}
		stone = std::stoull(temp);
		temp = "";
		for (size_t j = 0; j < numDigits / 2; ++j)
		{
			temp += ss.get();
		}
		newStone = std::stoull(temp);
		return true;
	}
}

void ApplyDefaultRule(u64& stone)
{
	stone *= 2024;
}

void ApplyRuleset(std::vector<u64>& stones)
{
	std::vector<u64> newStones = std::vector<u64>();
	for (u64& stone : stones)
	{
		if (!ApplyRule1(stone))
		{
			if (u64 newStone = 0; ApplyRule2(stone, newStone))
				newStones.push_back(newStone);
			else
				ApplyDefaultRule(stone);
		}
	}
	stones.insert(stones.end(), newStones.begin(), newStones.end());
}

int Day11::Solution1ver1()
{
	//horizontal_vector<u64> stones = FileUtil::ReadInputIntoVec<u64>(__FILE__, true);
	horizontal_vector<u64> stones = FileUtil::ReadInputIntoVec<u64>(__FILE__);

	//Testing::DebugFile dbg(__FILE__);
	//dbg.OutputSomething(stones);

	unsigned int iterations = 25;
	for (unsigned int i = 0; i < iterations; ++i)
	{
		ApplyRuleset(stones);
		//dbg.OutputSomething(stones);
	}

	std::cout << stones.size();

	return 0;
}

std::vector<u64> ApplyRulesetToStone(u64 stone)
{
	std::vector<u64> newStones;
	if (!ApplyRule1(stone))
	{
		if (u64 newStone = 0; ApplyRule2(stone, newStone))
			newStones.push_back(newStone);
		else
			ApplyDefaultRule(stone);
	}
	newStones.insert(newStones.begin(), stone);
	return newStones;
}

u64 BuildCostTable(cost_lookup_table<u64>& table, u64 entry, unsigned int recursionDepth, const unsigned int maxRecursionDepth)
{
	auto it = table.find(entry);
	if (it == table.end())
		it = table.emplace_empty(entry, maxRecursionDepth, 0).first;
	else if (it->second[recursionDepth - 1] != 0) // Entry has already been made
		return it->second[recursionDepth - 1];

	std::vector<u64> expandedEntry = ApplyRulesetToStone(entry);
	if (recursionDepth == 1)
	{
		it->second[0] = expandedEntry.size();
		return expandedEntry.size();
	}
	else
	{
		u64 stonesCost = 0;
		for (u64 nextStone : expandedEntry)
		{
			stonesCost += BuildCostTable(table, nextStone, recursionDepth - 1, maxRecursionDepth);
		}
		it->second[recursionDepth - 1] = stonesCost;
		return stonesCost;
	}
}

int Day11::Solution1ver2()
{
	//std::vector<u64> input = FileUtil::ReadInputIntoVec<u64>(__FILE__, true);
	std::vector<u64> input = FileUtil::ReadInputIntoVec<u64>(__FILE__);

	unsigned int iterations = 25;
	u64 totalCost = 0;
	cost_lookup_table<u64> table;
	for (u64 stone : input)
	{
		totalCost += BuildCostTable(table, stone, iterations, iterations);
	}

	std::cout << totalCost;

	return 0;
}

int Day11::Solution2()
{
	//std::vector<u64> input = FileUtil::ReadInputIntoVec<u64>(__FILE__, true);
	std::vector<u64> input = FileUtil::ReadInputIntoVec<u64>(__FILE__);

	unsigned int iterations = 75;
	u64 totalCost = 0;
	cost_lookup_table<u64> table;
	for (u64 stone : input)
	{
		totalCost += BuildCostTable(table, stone, iterations, iterations);
	}

	std::cout << totalCost;

	return 0;
}
