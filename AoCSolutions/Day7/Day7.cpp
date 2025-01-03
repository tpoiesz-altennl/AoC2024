#include "Day7.h"
#include "../Util.h"

#include <vector>
#include <string>
#include <queue>

unsigned long long mul(unsigned long long a, unsigned int b)
{
	return a * b;
}

unsigned long long add(unsigned long long a, unsigned int b)
{
	return a + b;
}

unsigned long long ApplyOperators(horizontal_vector<int> values, 
	const std::vector<std::function<unsigned long long(unsigned long long, unsigned int)>>& operators)
{
	unsigned long long total = values[0];
	for (unsigned int i = 1; i < values.size(); ++i)
	{
		total = operators[i - 1](total, static_cast<unsigned int>(values[i]));
	}
	return total;
}

bool Recurse(std::vector<std::function<unsigned long long(unsigned long long, unsigned int)>> operators,
	const std::vector<std::function<unsigned long long(unsigned long long, unsigned int)>>& possibleOperators,
	const horizontal_vector<int>& values, const unsigned long long targetVal, unsigned int m, int n)
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

bool TryEverything(const std::pair<int, horizontal_vector<int>>& testCase,
	const std::vector<std::function<unsigned long long(unsigned long long, unsigned int)>>& operators,
	const std::vector<std::function<unsigned long long(unsigned long long, unsigned int)>>& possibleOperators)
{
	unsigned long long resultVal = static_cast<unsigned long long>(testCase.first);
	horizontal_vector<int> inputVals = testCase.second;

	unsigned int m = static_cast<unsigned int>(possibleOperators.size()); // Range of for-loop of operators to try
	int n = static_cast<int>(operators.size()) - 1; // number of iterations per for-loop (positions of each operator)
	return Recurse(operators, possibleOperators, inputVals, resultVal, m, n);
}

int Day7::Solution1()
{
	std::vector<std::string> remainingLines;
	std::unordered_map<int, horizontal_vector<horizontal_vector<int>>> testCases = 
		FileUtil::ReadInputIntoLookupTable<int, horizontal_vector<int>>(__FILE__, remainingLines, ':');
	//std::unordered_map<int, horizontal_vector<horizontal_vector<int>>> testCases =
	//	FileUtil::ReadInputIntoLookupTable<int, horizontal_vector<int>>(__FILE__, remainingLines, ':', true);
	std::vector<std::function<unsigned long long(unsigned long long, unsigned int)>> possibleOperators{ &mul, &add };

	unsigned long long total = 0;
	std::vector<std::pair<int, horizontal_vector<int>>> testCasesForDebug;
	Testing::DebugFile dbg(__FILE__);
	for (const auto& testCaseSet : testCases)
	{
		for (const horizontal_vector<int>& input : testCaseSet.second)
		{
			std::vector<std::function<unsigned long long(unsigned long long, unsigned int)>> operators;
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
			unsigned long long bestSet = ApplyOperators(input, operators);
			if (bestSet == testCaseSet.first)
			{
				total += testCaseSet.first;
			}
			else if (bestSet > testCaseSet.first)
			{
				std::pair<int, horizontal_vector<int>> testCase(testCaseSet.first, input);
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
				std::pair<int, horizontal_vector<int>> testCase(testCaseSet.first, input);
				dbg.OutputRule<std::pair<int, horizontal_vector<int>>>(testCase);
			}
		}
	}

	std::cout << total;

	return 0;
}

int Day7::Solution2()
{
	return 0;
}
