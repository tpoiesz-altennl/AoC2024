#include "Day11.h"
#include "../Util.h"

#include <sstream>
#include <thread>
#include <deque>

bool ApplyRule1(u64& stone)
{
	if (stone != 0)
	{
		return false;
	}
	else
	{
		stone = 1;
		return true;
	}
}

bool ApplyRule2(u64& stone, u64& newStone)
{
	std::stringstream ss;
	ss << stone;
	size_t numDigits = ss.str().length();
	if (numDigits % 2 != 0)
	{
		return false;
	}
	else
	{
		std::string temp = "";
		for (size_t j = 0; j < numDigits / 2; ++j)
		{
			temp += ss.get();
		}
		stone = std::stoull(temp);
		temp = "";
		for (size_t j = 0; j < numDigits / 2; ++j)
		{
			temp += ss.get();
		}
		newStone = std::stoull(temp);
		return true;
	}
}

void ApplyDefaultRule(u64& stone)
{
	stone *= 2024;
}

void ApplyRuleset(std::vector<u64>& stones)
{
	std::vector<u64> newStones = std::vector<u64>();
	for (u64& stone : stones)
	{
		if (!ApplyRule1(stone))
		{
			u64 newStone;
			if (ApplyRule2(stone, newStone))
			{
				newStones.push_back(newStone);
			}
			else
				ApplyDefaultRule(stone);
		}
	}
	stones.insert(stones.end(), newStones.begin(), newStones.end());
}

void Iterate(unsigned int its, std::pair<std::vector<u64>, std::vector<u64>>& stonesAndSizes)
{
	for (unsigned int i = 0; i < its; ++i)
	{
		ApplyRuleset(stonesAndSizes.first);
		stonesAndSizes.second[i] = stonesAndSizes.first.size();
	}
}

int Day11::Solution1()
{
	std::vector<u64> input = FileUtil::ReadInputIntoVec<u64>(__FILE__, true);
	//std::vector<u64> input = FileUtil::ReadInputIntoVec<u64>(__FILE__);
	horizontal_vector<u64> stones;
	stones.resize(input.size());
	std::move(input.begin(), input.end(), stones.begin());

	//Testing::DebugFile dbg(__FILE__);
	//dbg.OutputRule(stones);

	unsigned int iterations = 30;
	for (unsigned int i = 0; i < iterations; ++i)
	{
		ApplyRuleset(stones);
		//dbg.OutputRule(stones);
	}

	//std::cout << stones.size();

	return 0;
}

horizontal_vector<u64> RunTestStone(unsigned int iterations, unsigned int subLoops = 1)
{
	horizontal_vector<u64> sizes;
	std::vector<u64> testStones;
	testStones.push_back(0);

	for (unsigned int i = 0; i < subLoops; ++i)
	{
		std::deque<std::thread> subProcesses;
		std::vector<std::pair<std::vector<u64>, std::vector<u64>>> stonesAndSizes(testStones.size());
		for (unsigned int n = 0; n < testStones.size(); ++n)
		{
			std::vector<u64> stoneVec({ testStones[n] });
			std::vector<u64> sizeVec(iterations / subLoops);
			stonesAndSizes[n] = std::make_pair(stoneVec, sizeVec);
			//allStones.push_back(std::vector<u64>({ stone }));
			//subProcesses.push_back(std::thread(&Iterate, iterations / subLoops, stonesVec, sizesVec));
			//auto& pair = stonesAndSizesCumul.back();
			subProcesses.push_back(std::thread(&Iterate, iterations / subLoops, std::ref(stonesAndSizes[n])));
			if (subProcesses.size() >= std::thread::hardware_concurrency())
			{
				subProcesses[0].join();
				subProcesses.pop_front();
			}
		}
		for (auto& thread : subProcesses)
		{
			thread.join();
		}

		// All result vectors are stored in allStones, so testStones should be cleared so it can be refilled with those result vectors
		testStones.clear();
		for (unsigned int n = 0; n < stonesAndSizes.size(); ++n)
		{
			// subSizes stores the size of each sub-iteration, so we accumulate them
			const std::vector<u64>& subSizes = stonesAndSizes[n].second;
			for (u64 m = 0; m < subSizes.size(); ++m)
			{
				if ((iterations / subLoops) * i + m >= sizes.size())
				{
					sizes.push_back(subSizes[m]);
				}
				else
				{
					sizes[(iterations / subLoops) * i + m] += subSizes[m];
				}
			}
			// Accumulate all allStones vectors into testStones vector
			const std::vector<u64>& subStones = stonesAndSizes[n].first;
			testStones.insert(testStones.end(), subStones.begin(), subStones.end());
		}
	}
	return sizes;
}

int Day11::Solution2()
{
	std::vector<u64> input = FileUtil::ReadInputIntoVec<u64>(__FILE__, true);
	//std::vector<u64> input = FileUtil::ReadInputIntoVec<u64>(__FILE__);
	horizontal_vector<u64> stones;
	stones.resize(input.size());
	std::move(input.begin(), input.end(), stones.begin());

	Testing::DebugFile dbg(__FILE__);
	horizontal_vector<u64> stone0Sizes = RunTestStone(30, 10);
	//dbg.OutputRule(stone0Sizes);

	/*unsigned int iterations = 75;
	unsigned int outerLoopLen = 5;
	for (u64 stone : stones)
	{
		horizontal_vector<u64> singleStone;
		singleStone.push_back(stone);
		std::thread thr(&ApplyRuleset);
		for (unsigned int i = 0; i < outerLoopLen; ++i)
		{
			ApplyRuleset(stones);
			dbg.OutputRule(stones);
		}
		thr.join();
	}*/

	//std::cout << stones.size();

	return 0;
}
