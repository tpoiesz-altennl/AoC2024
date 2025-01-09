#include "Day21.h"
#include "../Util.h"
#include <map>
#include <deque>
#include <algorithm>

vec2 NumericButtonPos(const char button)
{
	switch (button)
	{
	case 'A':
		return vec2(2, 3);
	case '0':
		return vec2(1, 3);
	case '1':
		return vec2(0, 2);
	case '2':
		return vec2(1, 2);
	case '3':
		return vec2(2, 2);
	case '4':
		return vec2(0, 1);
	case '5':
		return vec2(1, 1);
	case '6':
		return vec2(2, 1);
	case '7':
		return vec2(0, 0);
	case '8':
		return vec2(1, 0);
	case '9':
		return vec2(2, 0);
	default:
		return vec2(0, 3);
	}
}

vec2 DirectionalButtonPos(const char button)
{
	switch (button)
	{
	case 'A':
		return vec2(2, 0);
	case '^':
		return vec2(1, 0);
	case '<':
		return vec2(0, 1);
	case 'v':
		return vec2(1, 1);
	case '>':
		return vec2(2, 1);
	default:
		return vec2(0, 0);
	}
}

std::vector<std::vector<vec2>> ExpandOperation(vec2 startPos, vec2 endPos, vec2 illegalPos, vec2 APos)
{
	vec2 dir = endPos - startPos;
	auto loopInit = [](int targetVal) -> int {
		return (targetVal > 0 ? 1 : -1);
		};
	auto loopCondition = [](int n, int compareTo) -> bool {
		if (compareTo > 0)
			return n <= compareTo;
		else if (compareTo < 0)
			return n >= compareTo;
		else
			return false;
		};
	auto loopInc = [](int& n, int targetVal) -> void {
		n += (targetVal > 0 ? 1 : -1);
		};

	std::vector<vec2> instruction;
	{ // Build horizontal then vertical path
		// If we would cross over the illegal position, it would be at the end of the first path stretch,
		// which for the horizontal then vertical path would be dir.x steps from the starting position
		if (vec2(startPos.x + dir.x, startPos.y) != illegalPos)
		{
			vec2 directionButton = (dir.x > 0 ? DirectionalButtonPos('>') : DirectionalButtonPos('<'));
			for (int x = loopInit(dir.x); loopCondition(x, dir.x); loopInc(x, dir.x))
				instruction.push_back(directionButton);
			directionButton = (dir.y > 0 ? DirectionalButtonPos('v') : DirectionalButtonPos('^'));
			for (int y = loopInit(dir.y); loopCondition(y, dir.y); loopInc(y, dir.y))
				instruction.push_back(directionButton);
		}
	}
	std::vector<vec2> mirrored;
	{ // Build vertical then horizontal path
		if (vec2(startPos.x, startPos.y + dir.y) != illegalPos)
		{
			vec2 directionButton = (dir.y > 0 ? DirectionalButtonPos('v') : DirectionalButtonPos('^'));
			for (int y = loopInit(dir.y); loopCondition(y, dir.y); loopInc(y, dir.y))
				mirrored.push_back(directionButton);
			directionButton = (dir.x > 0 ? DirectionalButtonPos('>') : DirectionalButtonPos('<'));
			for (int x = loopInit(dir.x); loopCondition(x, dir.x); loopInc(x, dir.x))
				mirrored.push_back(directionButton);
		}
	}

	std::vector<std::vector<vec2>> instructionSet;
	if (instruction.size() && std::find(instruction.cbegin(), instruction.cend(), illegalPos) == instruction.cend())
	{
		instruction.push_back(APos);
		instructionSet.push_back(instruction);
	}
	if (mirrored.size() && (instruction.empty() || mirrored[0] != instruction[0]) &&
		std::find(mirrored.cbegin(), mirrored.cend(), illegalPos) == mirrored.cend())
	{
		mirrored.push_back(APos);
		instructionSet.push_back(mirrored);
	}
	if (instructionSet.empty())
		instructionSet.push_back({ APos });

	return instructionSet;
}

std::vector<vec2> ExpandInstructionSet(std::vector<std::vector<vec2>>& instructionSet, 
	unsigned int recursionDepth, const vec2 robotStartingPos)
{
	if (recursionDepth == 0)
	{
		std::sort(instructionSet.begin(), instructionSet.end(), [](const std::vector<vec2>& a, const std::vector<vec2>& b) {
			return a.size() < b.size();
			});
		return instructionSet[0];
	}

	std::vector<std::vector<vec2>> expandedInstructions;
	for (std::vector<vec2> instruction : instructionSet)
	{
		std::deque<std::vector<vec2>> possibleInstructionsSet;
		instruction.insert(instruction.begin(), robotStartingPos);
		for (unsigned int i = 0; i < instruction.size() - 1; ++i)
		{
			std::vector<std::vector<vec2>> expanded =
				ExpandOperation(instruction[i], instruction[i + 1], DirectionalButtonPos('X'), DirectionalButtonPos('A'));
			unsigned int num = static_cast<unsigned int>(possibleInstructionsSet.size());
			if (num)
			{
				for (unsigned int i = 0; i < num; ++i)
				{
					std::vector<vec2> stem = possibleInstructionsSet[0];
					possibleInstructionsSet.pop_front();
					for (std::vector<vec2> ext : expanded)
					{
						std::vector<vec2> copy = stem;
						copy.insert(copy.cend(), ext.begin(), ext.end());
						possibleInstructionsSet.push_back(copy);
					}
				}
			}
			else
			{
				for (std::vector<vec2> ext : expanded)
				{
					possibleInstructionsSet.push_back(ext);
				}
			}
		}
		for (std::vector<vec2>& possibleInstructions : possibleInstructionsSet)
		{
			expandedInstructions.push_back(std::move(possibleInstructions));
		}
	}
	return ExpandInstructionSet(expandedInstructions, recursionDepth - 1, robotStartingPos);
}

int Day21::Solution1()
{
	//std::vector<std::string> inputLines = FileUtil::ReadInputIntoVec<std::string>(__FILE__);
	std::vector<std::string> inputLines = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);

	u64 total = 0;
	for (const std::string& line : inputLines)
	{
		char robot0Pos = 'A';
		std::vector<vec2> allInstructions;
		for (unsigned int i = 0; i < line.size(); ++i)
		{
			std::vector<std::vector<vec2>> robot1Operations =
				ExpandOperation(NumericButtonPos(robot0Pos), NumericButtonPos(line[i]),
					NumericButtonPos('X'), DirectionalButtonPos('A'));
			std::vector<vec2> instructions = ExpandInstructionSet(robot1Operations, 2, DirectionalButtonPos('A'));
			allInstructions.insert(allInstructions.cend(), instructions.begin(), instructions.end());

			robot0Pos = line[i];
		}
		int multiplier = std::stoi(line.substr(0, 3));

		std::cout << allInstructions.size() << ' ' << multiplier << std::endl;
		total += allInstructions.size() * multiplier;
	}

	std::cout << total;
	return 0;
}

struct Vec2Pair
{
	vec2 From;
	vec2 To;

	Vec2Pair() : From(vec2()), To(vec2())
	{ }
	Vec2Pair(const vec2 from, const vec2 to) : From(from), To(to)
	{ }

	bool operator==(const Vec2Pair& other) const
	{
		return From == other.From && To == other.To;
	}
};

namespace std
{
	template <>
	struct hash<Vec2Pair> {
		size_t operator()(const Vec2Pair& k) const {
			size_t h1 = std::hash<vec2>{}(k.From);
			size_t h2 = std::hash<vec2>{}(k.To);
			return h1 ^ (h2 << 1); // Combine the two hash values
		}
	};
}

u64 BuildCostTable(cost_lookup_table<Vec2Pair>& table, Vec2Pair entry, vec2 illegalPos, vec2 APos,
	unsigned int recursionDepth, const unsigned int maxRecursionDepth)
{
	auto it = table.find(entry);
	if (it == table.end())
		it = table.emplace_empty(entry, maxRecursionDepth, 0).first;
	else if (it->second[recursionDepth - 1] != 0) // Entry has already been made
		return it->second[recursionDepth - 1];

	std::vector<std::vector<vec2>> expandedEntry = ExpandOperation(entry.From, entry.To, illegalPos, APos);
	if (recursionDepth == 1)
	{
		it->second[0] = expandedEntry[0].size();
		return expandedEntry[0].size();
	}
	else
	{
		u64 bestLayerCost = -1;
		for (std::vector<vec2> layer : expandedEntry)
		{
			layer.insert(layer.begin(), APos);
			u64 layerCost = 0;
			for (unsigned int i = 0; i < layer.size() - 1; ++i)
			{
				Vec2Pair nextEntry(layer[i], layer[i + 1]);
				layerCost += BuildCostTable(table, nextEntry, illegalPos, APos, recursionDepth - 1, maxRecursionDepth);
			}
			if (layerCost < bestLayerCost)
				bestLayerCost = layerCost;
		}
		it->second[recursionDepth - 1] = bestLayerCost;
		return bestLayerCost;
	}
}

int Day21::Solution2()
{
	std::vector<std::string> inputLines = FileUtil::ReadInputIntoVec<std::string>(__FILE__);
	//std::vector<std::string> inputLines = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);

	u64 totalWithMult = 0;
	cost_lookup_table<Vec2Pair> lookupTable;
	for (const std::string& line : inputLines)
	{
		u64 total = 0;
		char robot0Pos = 'A';
		for (unsigned int i = 0; i < line.size(); ++i)
		{
			u64 bestOperationCost = -1;
			std::vector<std::vector<vec2>> robot1Operations =
				ExpandOperation(NumericButtonPos(robot0Pos), NumericButtonPos(line[i]),
					NumericButtonPos('X'), DirectionalButtonPos('A'));
			for (std::vector<vec2> operation : robot1Operations)
			{
				operation.insert(operation.begin(), DirectionalButtonPos('A'));
				u64 operationCost = 0;
				for (unsigned int i = 0; i < operation.size() - 1; ++i)
				{
					unsigned int maxRecursionDepth = 25;
					Vec2Pair nextEntry(operation[i], operation[i + 1]);
					operationCost += BuildCostTable(lookupTable, nextEntry, 
						DirectionalButtonPos('X'), DirectionalButtonPos('A'), maxRecursionDepth, maxRecursionDepth);
				}
				if (operationCost < bestOperationCost)
					bestOperationCost = operationCost;
			}
			total += bestOperationCost;

			robot0Pos = line[i];
		}
		int multiplier = std::stoi(line.substr(0, 3));

		std::cout << total << ' ' << multiplier << std::endl;
		totalWithMult += total * multiplier;
	}

	std::cout << totalWithMult;
	return 0;
}
