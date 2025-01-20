#include "Day7.h"
#include "../Util.h"

#include <vector>
#include <string>
#include <sstream>

using u64 = unsigned long long;
// operator signature
using opSig = std::function<u64(u64, u64)>;

u64 mul(u64 a, u64 b)
{
	return a * b;
}

u64 add(u64 a, u64 b)
{
	return a + b;
}

u64 concat(u64 a, u64 b)
{
	std::stringstream ss;
	ss << a << b;
	return std::stoull(ss.str());
}

u64 ApplyOperators(horizontal_vector<u64> values, 
	const std::vector<opSig>& operators)
{
	u64 total = values[0];
	for (unsigned int i = 1; i < values.size(); ++i)
	{
		total = operators[i - 1](total, values[i]);
	}
	return total;
}

bool Recurse(std::vector<opSig> operators,
	const std::vector<opSig>& possibleOperators,
	const horizontal_vector<u64>& values, const u64 targetVal, unsigned int m, int n)
{
	if (n < 0)
		return false;

	for (unsigned int i = 0; i < m; ++i)
	{
		operators[n] = possibleOperators[i];
		if (ApplyOperators(values, operators) == targetVal)
			return true;
		else if (Recurse(operators, possibleOperators, values, targetVal, m, n - 1))
			return true;
	}
	return false;
}

bool TryEverything(const std::pair<u64, horizontal_vector<u64>>& testCase,
	const std::vector<opSig>& operators,
	const std::vector<opSig>& possibleOperators)
{
	u64 resultVal = static_cast<u64>(testCase.first);
	horizontal_vector<u64> inputVals = testCase.second;

	unsigned int m = static_cast<unsigned int>(possibleOperators.size()); // Range of for-loop of operators to try
	int n = static_cast<int>(operators.size()) - 1; // number of iterations per for-loop (positions of each operator)
	return Recurse(operators, possibleOperators, inputVals, resultVal, m, n);
}

int Day7::Solution1()
{
	std::vector<std::string> remainingLines;
	std::unordered_map<u64, horizontal_vector<horizontal_vector<u64>>> testCases = 
		FileUtil::ReadInputIntoLookupTable<u64, horizontal_vector<u64>>(__FILE__, remainingLines, ':');
	/*std::unordered_map<int, horizontal_vector<horizontal_vector<int>>> testCases =
		FileUtil::ReadInputIntoLookupTable<int, horizontal_vector<int>>(__FILE__, remainingLines, ':', true);*/
	std::vector<opSig> possibleOperators{ &mul, &add };

	u64 total = 0;
	Testing::DebugFile dbg(__FILE__);
	for (const auto& testCaseSet : testCases)
	{
		for (const horizontal_vector<u64>& input : testCaseSet.second)
		{
			std::vector<opSig> operators;
			for (unsigned int i = 0; i < input.size() - 1; ++i) // We need one fewer operator than the number of numbers in input
			{
				// Build the operator set that yields the highest value
				if (i == 0)
				{
					if (input[i] < 2 || input[i + 1] < 2)
						operators.push_back(&add);
					else
						operators.push_back(&mul);
				}
				else
				{
					if (input[i + 1] < 2)
						operators.push_back(&add);
					else
						operators.push_back(&mul);
				}
			}
			// Optimisation: if applying the best set of operators yields too low a value, no operator modifications are going to get us there
			u64 bestSet = ApplyOperators(input, operators);
			if (bestSet == testCaseSet.first)
			{
				total += testCaseSet.first;
			}
			else if (bestSet > testCaseSet.first)
			{
				std::pair<u64, horizontal_vector<u64>> testCase(testCaseSet.first, input);
				if (TryEverything(testCase, operators, possibleOperators))
				{
					total += testCaseSet.first;
				}
				/*else
				{
					dbg.OutputRule<std::pair<int, horizontal_vector<int>>>(testCase);
				}*/
			 }
			else
			{
				std::pair<u64, horizontal_vector<u64>> testCase(testCaseSet.first, input);
				dbg.OutputRule<std::pair<u64, horizontal_vector<u64>>>(testCase);
			}
		}
	}

	std::cout << total;

	return 0;
}

int Day7::Solution2()
{
	std::vector<std::string> remainingLines;
	std::unordered_map<u64, horizontal_vector<horizontal_vector<u64>>> testCases =
		FileUtil::ReadInputIntoLookupTable<u64, horizontal_vector<u64>>(__FILE__, remainingLines, ':');
	/*std::unordered_map<u64, horizontal_vector<horizontal_vector<u64>>> testCases =
		FileUtil::ReadInputIntoLookupTable<u64, horizontal_vector<u64>>(__FILE__, remainingLines, ':', true);*/
	std::vector<opSig> possibleOperators{ &mul, &add, &concat };

	u64 total = 0;
	Testing::DebugFile dbg(__FILE__);
	for (const auto& testCaseSet : testCases)
	{
		for (const horizontal_vector<u64>& input : testCaseSet.second)
		{
			std::vector<opSig> operators;
			for (unsigned int i = 0; i < input.size() - 1; ++i) // We need one fewer operator than the number of numbers in input
			{
				// Build the operator set that yields the highest value
				operators.push_back(&concat);
			}
			// Optimisation: if applying the best set of operators yields too low a value, no operator modifications are going to get us there
			u64 bestSet = ApplyOperators(input, operators);
			if (bestSet == testCaseSet.first)
			{
				total += testCaseSet.first;
			}
			else if (bestSet > testCaseSet.first)
			{
				std::pair<u64, horizontal_vector<u64>> testCase(testCaseSet.first, input);
				if (TryEverything(testCase, operators, possibleOperators))
				{
					total += testCaseSet.first;
					std::cout << testCaseSet.first << '\n';
				}
			}
			else
			{
				std::pair<u64, horizontal_vector<u64>> testCase(testCaseSet.first, input);
			}
		}
	}

	std::cout << total;

	return 0;
}
