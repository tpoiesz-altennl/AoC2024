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

u64 ApplyOperators(const horizontal_vector<u64>& values, 
	const std::vector<opSig>& operators)
{
	u64 total = values[0];
	for (unsigned int i = 1; i < values.size(); ++i)
	{
		total = operators[i - 1](total, values[i]);
	}
	return total;
}

bool Recurse(std::vector<opSig> operators, const std::vector<opSig>& possibleOperators,
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
	std::unordered_map<u64, horizontal_vector<u64>> testCases = 
		FileUtil::ReadInputIntoMap<u64, horizontal_vector<u64>>(__FILE__, remainingLines, ':');
	/*std::unordered_map<int, horizontal_vector<horizontal_vector<int>>> testCases =
		FileUtil::ReadInputIntoLookupTable<int, horizontal_vector<int>>(__FILE__, remainingLines, ':', true);*/
	std::vector<opSig> possibleOperators{ &mul, &add };

	u64 total = 0;
	for (const auto& testCaseSet : testCases)
	{
		const horizontal_vector<u64>& input = testCaseSet.second;

		std::vector<opSig> bestOperators;
		// We need one fewer operator than the number of numbers in input
		for (unsigned int i = 0; i < input.size() - 1; ++i)
		{
			// Build the operator set that yields the highest value
			if ((i == 0 && input[i] < 2) || input[i + 1] < 2)
				bestOperators.push_back(&add);
			else
				bestOperators.push_back(&mul);
		}
		// Optimisation: if applying the best set of operators yields too low a value,
		// no operator modifications are going to get us there
		u64 bestResult = ApplyOperators(input, bestOperators);

		std::vector<opSig> worstOperators;
		for (unsigned int i = 0; i < input.size() - 1; ++i)
		{
			// Build the operator set that yields the lowest value
			if ((i == 0 && input[i] < 2) || input[i + 1] < 2)
				worstOperators.push_back(&mul);
			else
				worstOperators.push_back(&add);
		}
		// Optimisation: if applying the worst set of operators yields too high a value,
		// no operator modifications are going to get us there
		u64 worstResult = ApplyOperators(input, worstOperators);

		if (bestResult == testCaseSet.first || worstResult == testCaseSet.first)
			total += testCaseSet.first;
		else if (bestResult > testCaseSet.first && worstResult < testCaseSet.first
			&& TryEverything(testCaseSet, bestOperators, possibleOperators))
			total += testCaseSet.first;
	}

	std::cout << total << std::endl;

	return 0;
}

int Day7::Solution2()
{
	std::vector<std::string> remainingLines;
	std::unordered_map<u64, horizontal_vector<u64>> testCases =
		FileUtil::ReadInputIntoMap<u64, horizontal_vector<u64>>(__FILE__, remainingLines, ':');
	/*std::unordered_map<u64, horizontal_vector<horizontal_vector<u64>>> testCases =
		FileUtil::ReadInputIntoLookupTable<u64, horizontal_vector<u64>>(__FILE__, remainingLines, ':', true);*/
	std::vector<opSig> possibleOperators{ &mul, &add, &concat };

	u64 total = 0;
	for (const auto& testCaseSet : testCases)
	{
		const horizontal_vector<u64>& input = testCaseSet.second;

		std::vector<opSig> bestOperators;
		for (unsigned int i = 0; i < input.size() - 1; ++i) // We need one fewer operator than the number of numbers in input
		{
			// Build the operator set that yields the highest value
			bestOperators.push_back(&concat);
		}
		// Optimisation: if applying the best set of operators yields too low a value, no operator modifications are going to get us there
		u64 bestResult = ApplyOperators(input, bestOperators);

		std::vector<opSig> worstOperators;
		for (unsigned int i = 0; i < input.size() - 1; ++i)
		{
			// Build the operator set that yields the lowest value
			if ((i == 0 && input[i] < 2) || input[i + 1] < 2)
				worstOperators.push_back(&mul);
			else
				worstOperators.push_back(&add);
		}
		// Optimisation: if applying the worst set of operators yields too high a value,
		// no operator modifications are going to get us there
		u64 worstResult = ApplyOperators(input, worstOperators);

		if (bestResult == testCaseSet.first || worstResult == testCaseSet.first)
			total += testCaseSet.first;
		else if (bestResult > testCaseSet.first && worstResult < testCaseSet.first
			&& TryEverything(testCaseSet, bestOperators, possibleOperators))
			total += testCaseSet.first;
	}

	std::cout << total << std::endl;

	return 0;
}
