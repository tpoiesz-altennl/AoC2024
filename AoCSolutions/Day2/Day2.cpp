#include "Day2.h"

#include <fstream>
#include <sstream>
#include <vector>
#include "../Util.h"

bool IsSafe(const std::vector<int>& levels, bool maySkip = false)
{
	if (levels.size() < 2)
	{
		return true;
	}

	bool ascending = (levels[1] > levels[0]);
	bool skipUsed = false;
	unsigned int prevIndex = 0;
	for (unsigned int i = 1; i < levels.size(); ++i)
	{
		bool compatible = (ascending ? 
			levels[i] - levels[prevIndex] <= 3 && levels[i] - levels[prevIndex] > 0 :
			levels[prevIndex] - levels[i] <= 3 && levels[prevIndex] - levels[i] > 0);
		if (!compatible)
		{
			if (!maySkip)
				return false;
			else if (skipUsed)
			{
				std::vector<int> fixedLevels = levels;
				// Try skipping the first conflicting level instead of the second one
				fixedLevels.erase(fixedLevels.begin() + prevIndex);
				if (IsSafe(fixedLevels, false))
					return true;
				else
				{
					// Try skipping the first level
					fixedLevels = levels;
					fixedLevels.erase(fixedLevels.begin());
					return IsSafe(fixedLevels, false);
				}
			}
			else
			{
				skipUsed = true;
				// Let prevIndex trail when using a skip
			}
		}
		else
		{
			// Levels are compatible, continue to next index
			prevIndex = i;
		}
	}

	return true;
}

int Day2::Solution1()
{
	std::vector<horizontal_vector<int>> reports = FileUtil::ReadInputIntoVec<horizontal_vector<int>>(__FILE__);
	//std::vector<horizontal_vector<int>> reports = FileUtil::ReadInputIntoVec<horizontal_vector<int>>(__FILE__, true);

	unsigned int numSafe = 0;
	for (std::vector<int> levels : reports)
	{
		if (IsSafe(levels))
			++numSafe;
	}

	std::cout << numSafe << std::endl;
	return 0;
}

int Day2::Solution2ver1()
{
	std::vector<horizontal_vector<int>> reports = FileUtil::ReadInputIntoVec<horizontal_vector<int>>(__FILE__);
	//std::vector<horizontal_vector<int>> reports = FileUtil::ReadInputIntoVec<horizontal_vector<int>>(__FILE__, true);

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputMatches<horizontal_vector<int>, bool>(reports, &IsSafe, false, true);

	unsigned int numSafe = 0;
	for (std::vector<int> levels : reports)
	{
		if (IsSafe(levels, true))
			++numSafe;
	}

	std::cout << numSafe;
	return 0;
}

bool IsSafeDiffs(const std::vector<int>& diffs)
{
	if (diffs.size() < 1)
	{
		return true;
	}

	auto ascCondition = [](int diff) -> bool { return diff > 0 && diff <= 3; };
	auto descCondition = [](int diff) -> bool { return diff < 0 && diff >= -3; };

	auto checkCondition = [&](const auto& condition) -> bool
		{
			bool skipped = false;
			bool failed = false;
			for (unsigned int i = 0; i < diffs.size(); ++i)
			{
				if (!condition(diffs[i]))
				{
					if (skipped)
						return false;
					// If error is caused by first or last diff, can just skip. Otherwise, check if previous diff makes up for the error
					else if (i == 0 || i == diffs.size() - 1 || condition(diffs[i - 1] + diffs[i]))
						skipped = true;
					// Check if next diff makes up for the error and skip to that one
					else if (i < diffs.size() - 1 && condition(diffs[i] + diffs[i + 1]))
					{
						++i;
						skipped = true;
					}
					else
						return false;
				}
			}
			return true;
		};

	return checkCondition(ascCondition) || checkCondition(descCondition);
}

int Day2::Solution2ver2()
{
	std::ifstream inFile = FileUtil::OpenFile(__FILE__);
	//std::ifstream inFile = FileUtil::OpenFile(__FILE__, true);

	if (!inFile)
	{
		inFile.close();
		return 1;
	}

	std::vector<horizontal_vector<int>> reports;
	std::string reportString;
	while (std::getline(inFile, reportString))
	{
		std::stringstream ss(reportString);
		int temp = 0, prevTemp = 0;
		ss >> prevTemp;
		horizontal_vector<int> differences;
		while (ss >> temp)
		{
			differences.push_back(temp - prevTemp);
			prevTemp = temp;
		}
		reports.push_back(differences);
	}
	inFile.close();

	Testing::DebugFile dbg(__FILE__);
	dbg.OutputMatches<horizontal_vector<int>> (reports, &IsSafeDiffs, false);

	unsigned int numSafe = 0;
	for (horizontal_vector<int> diffs : reports)
	{
		if (IsSafeDiffs(diffs))
			++numSafe;
	}

	std::cout << numSafe;

	return 0;
}
