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
	size_t prevIndex = 0;
	for (size_t i = 1; i < levels.size(); ++i)
	{
		if (ascending)
		{
			if (levels[i] - levels[prevIndex] > 3 || levels[i] - levels[prevIndex] <= 0)
			{
				if (skipUsed || !maySkip)
				{
					if (!maySkip)
						return false;
					else
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
				}
				else
				{
					skipUsed = true;
					// Let prevIndex trail when using a skip
				}
			}
			else
			{
				prevIndex = i;
			}
		}
		else
		{
			if (levels[prevIndex] - levels[i] > 3 || levels[prevIndex] - levels[i] <= 0)
			{
				if (skipUsed || !maySkip)
				{
					if (!maySkip)
						return false;
					else
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
				}
				else
				{
					skipUsed = true;
					// Let prevIndex trail when using a skip
				}
			}
			else
			{
				prevIndex = i;
			}
		}
	}

	return true;
}

int Day2::Solution1()
{
	std::vector<horizontal_vector<int>> reports = FileUtil::ReadInputFileIntoVec<horizontal_vector<int>>(__FILE__);
	//std::vector<horizontal_vector<int>> reports = FileUtil::ReadTestFileIntoVec<horizontal_vector<int>>(__FILE__);

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
	std::vector<horizontal_vector<int>> reports = FileUtil::ReadInputFileIntoVec<horizontal_vector<int>>(__FILE__);
	//std::vector<horizontal_vector<int>> reports = FileUtil::ReadTestFileIntoVec<horizontal_vector<int>>(__FILE__);

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

	auto ascCondition = [](int a) -> bool { return a > 0 && a <= 3; };
	auto descCondition = [](int a) -> bool { return a < 0 && a >= -3; };

	auto checkCondition = [&](const auto& condition) -> bool
		{
			bool skipped = false;
			bool failed = false;
			for (size_t i = 0; i < diffs.size(); ++i)
			{
				if (!condition(diffs[i]))
				{
					if (i < diffs.size() - 1 && condition(diffs[i] + diffs[i + 1]))
					{
						if (skipped)
						{
							failed = true;
							break;
						}
						else
						{
							++i;
							skipped = true;
						}
					}
					else if (i > 0 && condition(diffs[i - 1] + diffs[i]))
					{
						if (skipped)
						{
							failed = true;
							break;
						}
						else
						{
							skipped = true;
						}
					}
					else if (i == 0 || i == diffs.size() - 1)
					{
						if (skipped)
						{
							failed = true;
							break;
						}
						else
						{
							skipped = true;
						}
					}
					else
					{
						failed = true;
						break;
					}
				}
			}
			if (!failed)
				return true;
			else
				return false;
		};

	if (checkCondition(ascCondition) || checkCondition(descCondition))
		return true;
	else
		return false;
}

int Day2::Solution2ver2()
{
	std::ifstream inFile = FileUtil::OpenInputFile(__FILE__);
	//std::ifstream inFile = FileUtil::OpenTestFile(__FILE__);

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

	//std::cout << numSafe;

	return 0;
}
