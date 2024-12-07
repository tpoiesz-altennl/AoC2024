#include <iostream>
#include <fstream>
#include "../Util.h"
#include <vector>
#include <algorithm>
#include <cmath>

#include "Day1.h"

int Day1::Solution1()
{
	std::ifstream inFile = FileUtil::OpenFile(__FILE__);
	//std::ifstream inFile = FileUtil::OpenFile(__FILE__, true);

	if (!inFile)
	{
		inFile.close();
		return 1;
	}
	
	std::vector<int> leftList, rightList;

	int leftElem, rightElem;
	while (inFile >> leftElem >> rightElem)
	{
		leftList.push_back(leftElem);
		rightList.push_back(rightElem);
	}
	inFile.close();

	std::sort(leftList.begin(), leftList.end());
	std::sort(rightList.begin(), rightList.end());

	unsigned long long totalDiff = 0;
	for (size_t i = 0; i < leftList.size(); ++i)
	{
		unsigned int diff = static_cast<unsigned long long>(std::abs(rightList[i] - leftList[i]));
		std::cout << leftList[i] << '\t' << rightList[i] << '\t' << diff << '\n';
		totalDiff += diff;
	}

	std::cout << totalDiff;

	return 0;
}

int Day1::Solution2()
{
	std::ifstream inFile = FileUtil::OpenFile(__FILE__);
	//std::ifstream inFile = FileUtil::OpenTestFile(__FILE__, true);

	if (!inFile)
	{
		inFile.close();
		return 1;
	}

	std::vector<int> leftList, rightList;

	int leftElem, rightElem;
	while (inFile >> leftElem >> rightElem)
	{
		leftList.push_back(leftElem);
		rightList.push_back(rightElem);
	}
	inFile.close();

	std::sort(leftList.begin(), leftList.end());
	std::sort(rightList.begin(), rightList.end());

	unsigned int numMatches = 0;
	unsigned long long simiScore = 0;
	auto rightListIt = rightList.begin();
	for (size_t i = 0; i < leftList.size(); ++i)
	{
		if (i > 0 && leftList[i - 1] != leftList[i])
		{
			numMatches = 0;
		}
		unsigned int count = 0;
		while (*rightListIt < leftList[i])
		{
			++rightListIt;
		}
		while (*rightListIt == leftList[i])
		{
			++numMatches;
			++rightListIt;
		}
		simiScore += numMatches * static_cast<unsigned long long>(leftList[i]);
		std::cout << leftList[i] << '\t' << numMatches << '\t' << numMatches * static_cast<unsigned long long>(leftList[i]) << '\n';
	}

	std::cout << simiScore;

	return 0;
}
