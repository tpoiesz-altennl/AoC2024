#include "Day2.h"

#include <fstream>
#include <sstream>
#include <vector>
#include "../Util.h"

bool isSafe(const std::vector<int>& levels, bool maySkip = false)
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
					return false;
				}
				else
				{
					skipUsed = true;
				}
			}
			else
			{
				// Let prevIndex trail when using a skip
				prevIndex = i;
			}
		}
		else
		{
			if (levels[prevIndex] - levels[i] > 3 || levels[prevIndex] - levels[i] <= 0)
			{
				if (skipUsed || !maySkip)
				{
					return false;
				}
				else
				{
					skipUsed = true;
				}
			}
			else
			{
				// Let prevIndex trail when using a skip
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
		if (isSafe(levels))
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

	std::vector<std::vector<int>> reports;
	std::string reportString;
	while (std::getline(inFile, reportString))
	{
		std::stringstream ss(reportString);
		std::vector<int> levels{
			std::istream_iterator<int>(ss),
			std::istream_iterator<int>()
		};
		reports.push_back(levels);
	}

	//FileUtil::OutputMatches<std::vector<int>, bool>(__FILE__, reports, &isSafe, true, true);

	int count = 0;
	unsigned int numSafe = 0;
	for (std::vector<int> levels : reports)
	{
		++count;
		if (isSafe(levels, true))
			++numSafe;
		else
		{
			// If only the first level is wrong, we need to check that separately
			std::vector<int> fixedLevels = levels;
			fixedLevels.erase(fixedLevels.begin());
			if (isSafe(fixedLevels, false))
				++numSafe;
			else
			{
				// Could be that our assumption for ascending/descending got screwed up by the second level being wrong, 
				// so we might need to check again with the second level removed
				fixedLevels = levels;
				if ((fixedLevels[1] >= fixedLevels[0] && fixedLevels[2] < fixedLevels[0]) ||
					(fixedLevels[1] <= fixedLevels[0] && fixedLevels[2] > fixedLevels[0]))
				{
					fixedLevels.erase(fixedLevels.begin() + 1);
					if (isSafe(fixedLevels, false))
						++numSafe;
					else
					{
						std::cout << count << '\n';
					}
				}
			}
		}
	}

	std::cout << numSafe;
	return 0;
}
