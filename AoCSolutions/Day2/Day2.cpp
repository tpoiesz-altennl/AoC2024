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
	std::ifstream inFile = FileUtil::ReadInputFile(__FILE__);
	//std::ifstream inFile = FileUtil::ReadTestFile(__FILE__);

	if (!inFile)
	{
		inFile.close();
		return 1;
	}

	std::vector<std::vector<int>> reports;
	std::string reportString;
	while (std::getline(inFile, reportString))
	{
		std::stringstream ss(reportString);
		std::vector<int> levels {
			std::istream_iterator<int>(ss),
			std::istream_iterator<int>()
		};
		reports.push_back(levels);
	}

	unsigned int numSafe = 0;
	for (std::vector<int> levels : reports)
	{
		if (IsSafe(levels))
			++numSafe;
	}

	std::cout << numSafe;
	return 0;
}

int Day2::Solution2()
{
	std::ifstream inFile = FileUtil::ReadInputFile(__FILE__);
	//std::ifstream inFile = FileUtil::ReadTestFile(__FILE__);

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
		int temp = 0;
		horizontal_vector<int> levels;
		while (ss >> temp)
			levels.push_back(temp);
		reports.push_back(levels);
	}

	FileUtil::DebugFile dbg(__FILE__);
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
