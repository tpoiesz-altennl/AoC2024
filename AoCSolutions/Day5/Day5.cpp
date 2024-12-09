#include "Day5.h"
#include "../Util.h"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <list>

struct ListNode
{
	int Value;
	std::set<int> Barriers;
	ListNode(int val, std::set<int> bar) :
		Value(val),
		Barriers(bar)
	{ }
};

std::list<ListNode>::iterator PlaceRule(std::list<ListNode>& ruleConfig, ListNode ruleVal, const std::list<ListNode>::iterator& posLimit)
{
	auto rule1Pos = ruleConfig.end();
	auto it = ruleConfig.end();
	bool shouldPlace = true;
	while (it != posLimit) // We don't have to continue beyond previously placed rule[0]
	{
		--it;
		if (it->Value == ruleVal.Value)
		{
			shouldPlace = false;
			break;
		}
		else if (it->Barriers.find(ruleVal.Value) != it->Barriers.end())
		{
			break;
		}
		--rule1Pos;
	}
	if (shouldPlace)
		return ruleConfig.emplace(rule1Pos, ruleVal);
	else
		return it;
}

// Returns true if any reordering actually happened
bool ReorderRule(std::list<ListNode>& ruleConfig, int searchVal, 
	std::list<ListNode>::iterator startSearchPos, std::list<ListNode>::iterator placeLimitPos)
{
	auto it = startSearchPos;
	while (it != ruleConfig.begin())
	{
		--it;
		if (it->Value == searchVal)
		{
			ListNode l = *it;
			auto placementLimit = PlaceRule(ruleConfig, l, startSearchPos);
			startSearchPos = ruleConfig.erase(it);
			for (int i : l.Barriers)
			{
				ReorderRule(ruleConfig, i, startSearchPos, placementLimit);
			}
			return true;;
		}
	}
	return false;
}

horizontal_vector<int> ConstructRulesetVer1(const std::vector<horizontal_vector<int>>& rules)
{
	horizontal_vector<int> ruleset;
	std::list<ListNode> ruleConfig;
	for (horizontal_vector<int> rule : rules)
	{
		if (ruleConfig.size() < 1)
		{
			ruleConfig.emplace_back(ListNode(rule[0], { rule[1] }));
			ruleConfig.emplace_back(ListNode(rule[1], {}));
		}
		else
		{
			auto rule0Pos = ruleConfig.end();
			bool shouldPlace0 = true;
			bool shouldPlace1 = true;
			bool shouldReorder = false;
			{ // Place rule[0] in ruleConfig or update barrier
				auto it = ruleConfig.end();
				while (it != ruleConfig.begin())
				{
					--it;
					if (it->Value == rule[0])
					{
						it->Barriers.insert(rule[1]);
						--rule0Pos;
						shouldPlace0 = false;
						shouldReorder = true;
						break;
					}
					else if (it->Barriers.find(rule[0]) != it->Barriers.end())
					{
						shouldPlace0 = false;
						break;
					}
					--rule0Pos;
				}
				if (shouldPlace0)
					ruleConfig.emplace(rule0Pos, ListNode(rule[0], { rule[1] }));

			}
			if (shouldReorder) // If a barrier was updated, move elements back that don't conform to the barrier
			{
				if (!ReorderRule(ruleConfig, rule[1], rule0Pos, rule0Pos))
					PlaceRule(ruleConfig, ListNode(rule[1], {}), rule0Pos);
			}
			else
			{
				PlaceRule(ruleConfig, ListNode(rule[1], {}), rule0Pos);
			}
		}
	}
	for (ListNode node : ruleConfig)
	{
		ruleset.push_back(node.Value);
	}
	
	return ruleset;
}

bool MatchToRulesetVer1(const horizontal_vector<int>& testCase, const horizontal_vector<int>& ruleset)
{
	unsigned int lowestIndex = 0;
	for (unsigned int i = 0; i < ruleset.size(); ++i)
	{
		for (unsigned int j = lowestIndex; j < testCase.size(); ++j)
		{
			if (ruleset[i] == testCase[j])
			{
				if (j > lowestIndex)
				{
					// Check if maybe the previous elements don't occur in the ruleset at all
					for (unsigned int k = lowestIndex; k < j; ++k)
					{
						if (std::find(ruleset.begin(), ruleset.end(), testCase[k]) != ruleset.end())
						{
							return false;
						}
					}
					lowestIndex = j;
				}
				else
				{
					++lowestIndex;
					break;
				}
			}				
		}
	}
	return true;
}

int Day5::Solution1ver1()
{
	//std::vector<std::string> input = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);
	std::vector<std::string> input = FileUtil::ReadInputIntoVec<std::string>(__FILE__);

	std::vector<std::string> remainingLines;
	std::vector<horizontal_vector<int>> rules = FileUtil::SplitInputLines<int>(input, '|', remainingLines);
	std::vector<horizontal_vector<int>> updates = FileUtil::SplitInputLines<int>(remainingLines, ',', remainingLines);

	horizontal_vector<int> ruleset = ConstructRulesetVer1(rules);

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputRule<horizontal_vector<int>>(ruleset);
	dbg.OutputMatches<horizontal_vector<int>, horizontal_vector<int>>(updates, &MatchToRulesetVer1, true, ruleset);

	int total = 0;
	for (horizontal_vector<int> update : updates)
	{
		if (MatchToRulesetVer1(update, ruleset))
			total += update[update.size() / 2]; // Add middle element of update sequences together
	}

	std::cout << total;
	return 0;
}

bool MatchToRulesetVer2(const horizontal_vector<int>& testCase, const std::unordered_map<int, horizontal_vector<int>>& ruleset)
{
	for (unsigned int i = 0; i < testCase.size(); ++i)
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

int Day5::Solution1ver2()
{
	std::vector<std::string> remainingLines;
	std::unordered_map<int, horizontal_vector<int>> ruleset = FileUtil::ReadInputIntoLookupTable<int, int>(__FILE__, remainingLines, '|', true);
	std::vector<horizontal_vector<int>> updates = FileUtil::SplitInputLines<int>(remainingLines, ',', remainingLines);

	Testing::DebugFile dbg(__FILE__);
	//dbg.OutputRule<horizontal_vector<int>>(ruleset);
	dbg.OutputMatches<horizontal_vector<int>, std::unordered_map<int, horizontal_vector<int>>>(updates, &MatchToRulesetVer2, true, ruleset);

	int total = 0;
	for (horizontal_vector<int> update : updates)
	{
		if (MatchToRulesetVer2(update, ruleset))
			total += update[update.size() / 2]; // Add middle element of update sequences together
	}

	std::cout << total;
	return 0;
}

bool MatchToRulesetVer3(const horizontal_vector<int>& testCase, const std::unordered_map<int, 
	horizontal_vector<int>>& ruleset, int& conflict1, int& conflict2)
{
	for (unsigned int i = 0; i < testCase.size(); ++i)
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

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputMatches<horizontal_vector<int>, std::unordered_map<int, horizontal_vector<int>>>(updates, &MatchToRulesetVer2, false, ruleset);

	int total = 0;
	for (horizontal_vector<int> update : updates)
	{
		int conflict1, conflict2;
		if (!MatchToRulesetVer3(update, ruleset, conflict1, conflict2))
		{
			do
			{
				int temp = update[conflict2];
				std::move(update.begin() + conflict1, update.begin() + conflict2, update.begin() + conflict1 + 1);
				update[conflict1] = temp;
			} while (!MatchToRulesetVer3(update, ruleset, conflict1, conflict2));
			total += update[update.size() / 2]; // Add middle element of update sequences together
		}
	}

	std::cout << total;
	return 0;
}
