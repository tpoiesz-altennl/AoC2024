#include "Day5.h"
#include "../Util.h"
#include <vector>
#include <algorithm>
#include <unordered_map>

bool MatchToRuleset(const horizontal_vector<int>& testCase, const std::unordered_map<int, horizontal_vector<int>>& ruleset)
{
	for (unsigned int i = 1; i < testCase.size(); ++i)
	{
		for (unsigned int j = 0; j < i; ++j)
		{
			if (const auto prereqsIt = ruleset.find(testCase[i]); prereqsIt != ruleset.end())
			{
				const horizontal_vector<int>& prereqs = prereqsIt->second;
				if (std::find(prereqs.begin(), prereqs.end(), testCase[j]) != prereqs.end())
					return false;
			}
		}
	}
	return true;
}

int Day5::Solution1()
{
	std::vector<std::string> remainingLines;
	std::unordered_map<int, horizontal_vector<int>> ruleset = FileUtil::ReadInputIntoLookupTable<int, int>(__FILE__, remainingLines, '|');
	//std::unordered_map<int, horizontal_vector<int>> ruleset = FileUtil::ReadInputIntoLookupTable<int, int>(__FILE__, remainingLines, '|', true);
	std::vector<horizontal_vector<int>> updates = FileUtil::SplitInputLines<int>(remainingLines, ',', remainingLines);

	/*Testing::DebugFile dbg(__FILE__);
	dbg.OutputSomething<horizontal_vector<int>>(ruleset);
	dbg.OutputMatches<horizontal_vector<int>, std::unordered_map<int, horizontal_vector<int>>>(updates, &MatchToRuleset, true, ruleset);*/

	int total = 0;
	for (horizontal_vector<int> update : updates)
	{
		if (MatchToRuleset(update, ruleset))
			total += update[update.size() / 2]; // Add middle element of update sequences together
	}

	std::cout << total << std::endl;
	return 0;
}

bool MatchToRuleset2(const horizontal_vector<int>& testCase, const std::unordered_map<int, 
	horizontal_vector<int>>& ruleset, int& conflict1, int& conflict2)
{
	for (unsigned int i = 1; i < testCase.size(); ++i)
	{
		for (unsigned int j = 0; j < i; ++j)
		{
			if (const auto prereqsIt = ruleset.find(testCase[i]); prereqsIt != ruleset.end())
			{
				const horizontal_vector<int>& prereqs = prereqsIt->second;
				if (std::find(prereqs.begin(), prereqs.end(), testCase[j]) != prereqs.end())
				{
					conflict1 = j;
					conflict2 = i;
					return false;
				}
			}
		}
	}
	return true;
}

int Day5::Solution2()
{
	std::vector<std::string> remainingLines;
	std::unordered_map<int, horizontal_vector<int>> ruleset = FileUtil::ReadInputIntoLookupTable<int, int>(__FILE__, remainingLines, '|');
	std::vector<horizontal_vector<int>> updates = FileUtil::SplitInputLines<int>(remainingLines, ',', remainingLines);

	/*Testing::DebugFile dbg(__FILE__);
	dbg.OutputMatches<horizontal_vector<int>, std::unordered_map<int, horizontal_vector<int>>>(updates, &MatchToRulesetVer2, false, ruleset);*/

	int total = 0;
	for (horizontal_vector<int> update : updates)
	{
		int conflict1, conflict2;
		if (!MatchToRuleset2(update, ruleset, conflict1, conflict2))
		{
			do
			{
				int temp = update[conflict2];
				std::move(update.begin() + conflict1, update.begin() + conflict2, update.begin() + conflict1 + 1);
				update[conflict1] = temp;
			} while (!MatchToRuleset2(update, ruleset, conflict1, conflict2));
			total += update[update.size() / 2]; // Add middle element of update sequences together
		}
	}

	std::cout << total << std::endl;
	return 0;
}
