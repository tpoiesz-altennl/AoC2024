#include <iostream>
#include <fstream>
#include "../Util.h"
#include <vector>
#include <algorithm>
#include <cmath>

#include "Day1.h"

int Day1::Solution1()
{
	std::vector<unsigned int> leftList, rightList;
	{
		std::vector<unsigned int> completeList = FileUtil::ReadInputIntoVec<unsigned int>(__FILE__);
		//std::vector<unsigned int> completeList = FileUtil::ReadInputIntoVec<unsigned int>(__FILE__, true);
		for (unsigned int i = 0; i < completeList.size(); ++i)
		{
			if (i % 2 == 0)
				leftList.push_back(completeList[i]);
			else
				rightList.push_back(completeList[i]);
		}
		std::sort(leftList.begin(), leftList.end());
		std::sort(rightList.begin(), rightList.end());
	}

	u64 totalDiff = 0;
	for (unsigned int i = 0; i < leftList.size(); ++i)
	{
		unsigned int diff = (rightList[i] > leftList[i] ? rightList[i] - leftList[i] : leftList[i] - rightList[i]);
		//std::cout << leftList[i] << '\t' << rightList[i] << '\t' << diff << '\n';
		totalDiff += diff;
	}

	std::cout << totalDiff;

	return 0;
}

int Day1::Solution2()
{
	std::vector<unsigned int> leftList, rightList;
	{
		std::vector<unsigned int> completeList = FileUtil::ReadInputIntoVec<unsigned int>(__FILE__);
		//std::vector<unsigned int> completeList = FileUtil::ReadInputIntoVec<unsigned int>(__FILE__, true);
		for (unsigned int i = 0; i < completeList.size(); ++i)
		{
			if (i % 2 == 0)
				leftList.push_back(completeList[i]);
			else
				rightList.push_back(completeList[i]);
		}
		std::sort(leftList.begin(), leftList.end());
		std::sort(rightList.begin(), rightList.end());
	}

	unsigned int numMatches = 0;
	u64 simiScore = 0;
	auto rightListIt = rightList.begin();
	for (size_t i = 0; i < leftList.size(); ++i)
	{
		if (i > 0 && leftList[i - 1] != leftList[i])
		{
			numMatches = 0;
		}
		while (*rightListIt < leftList[i])
		{
			++rightListIt;
		}
		while (*rightListIt == leftList[i])
		{
			++numMatches;
			++rightListIt;
		}
		simiScore += numMatches * static_cast<u64>(leftList[i]);
		//std::cout << leftList[i] << '\t' << numMatches << '\t' << numMatches * static_cast<u64>(leftList[i]) << '\n';
	}

	std::cout << simiScore;

	return 0;
}
