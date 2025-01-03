#include "Day19.h"
#include "../Util.h"
#include <map>
#include <set>

using towel_container = std::map<char, std::map<unsigned int, std::set<std::string>>>;

towel_container ParseTowelContainer(std::ifstream& file, unsigned int& maxTowelSize)
{
	towel_container ret;
	std::string line;
	std::getline(file, line);
	size_t prevSepPos = 0;
	size_t sepPos = line.find(',');
	while (sepPos != std::string::npos)
	{
		std::string towel = line.substr(prevSepPos, sepPos - prevSepPos);
		if (towel.size() > maxTowelSize)
			maxTowelSize = static_cast<unsigned int>(towel.size());
		auto it = ret.find(towel[0]);
		if (it != ret.end())
		{
			auto it2 = it->second.find(static_cast<unsigned int>(towel.size()));
			if (it2 != it->second.end())
			{
				it2->second.emplace(towel);
			}
			else
			{
				it->second.emplace(std::make_pair(static_cast<unsigned int>(towel.size()), std::set<std::string>({ towel })));
			}
		}
		else
		{
			std::set<std::string> temp1({ towel });
			std::map<unsigned int, std::set<std::string>> temp2;
			temp2.emplace(std::make_pair(static_cast<unsigned int>(towel.size()), temp1));
			ret.emplace(std::make_pair(towel[0], temp2));
		}
		prevSepPos = sepPos + 2; // Skip separator and space
		sepPos = line.find(',', prevSepPos + 1);
	}
	// Add last entry that isn't proceeded by a separator
	{
		std::string towel = line.substr(prevSepPos);
		if (towel.size() > maxTowelSize)
			maxTowelSize = static_cast<unsigned int>(towel.size());
		auto it = ret.find(towel[0]);
		if (it != ret.end())
		{
			auto it2 = it->second.find(static_cast<unsigned int>(towel.size()));
			if (it2 != it->second.end())
			{
				it2->second.emplace(towel);
			}
			else
			{
				it->second.emplace(std::make_pair(static_cast<unsigned int>(towel.size()), std::set<std::string>({ towel })));
			}
		}
		else
		{
			std::set<std::string> temp1({ towel });
			std::map<unsigned int, std::set<std::string>> temp2;
			temp2.emplace(std::make_pair(static_cast<unsigned int>(towel.size()), temp1));
			ret.emplace(std::make_pair(towel[0], temp2));
		}
	}
	// Skip over empty line
	std::getline(file, line);
	return ret;
}

std::vector<std::string> ParseTowelDesigns(std::ifstream& file)
{
	std::vector<std::string> ret;
	std::string temp;
	while (file >> temp)
	{
		ret.push_back(temp);
	}
	return ret;
}

bool MatchTowelDesign(const std::string& design, const towel_container& towels, const unsigned int maxTowelLength)
{
	if (design.size() == 0)
		return true;

	char towelStart = design[0];
	auto charMapIt = towels.find(towelStart);
	if (charMapIt != towels.end())
	{
		int max = static_cast<int>(maxTowelLength > design.size() ? design.size() : maxTowelLength);
		for (unsigned int len = max; len > 0; --len)
		{
			auto lenMapIt = charMapIt->second.find(len);
			if (lenMapIt != charMapIt->second.end())
			{
				std::string designSub = design.substr(0, len);
				if (lenMapIt->second.find(designSub) != lenMapIt->second.end())
				{
					if (MatchTowelDesign(design.substr(len), towels, maxTowelLength))
						return true;
				}
			}
		}
	}
	return false;
}

int Day19::Solution1()
{
	//std::ifstream file = FileUtil::OpenFile(__FILE__, true);
	std::ifstream file = FileUtil::OpenFile(__FILE__);
	unsigned int maxTowelSize = 0;
	towel_container towelTypes = ParseTowelContainer(file, maxTowelSize);
	std::vector<std::string> towelDesigns = ParseTowelDesigns(file);
	file.close();
	
	unsigned int numPossibleDesigns = 0;
	for (const std::string& design : towelDesigns)
	{
		if (MatchTowelDesign(design, towelTypes, maxTowelSize))
			++numPossibleDesigns;
	}

	std::cout << numPossibleDesigns;
	return 0;
}

using design_tree = std::map<unsigned int, std::set<unsigned int>>;

design_tree BuildTree(const std::string& design, const towel_container& towels, const unsigned int maxTowelLength)
{
	design_tree tree;
	std::vector<bool> hasStem(design.size() + 1, 0);
	hasStem[0] = true;
	bool firstCharCompatible = false;
	for (unsigned int i = 0; i < design.size(); ++i)
	{
		// Skip branches that don't have a stem (a previous branch pointing to them)
		if (!hasStem[i])
			continue;

		std::set<unsigned int> branch;
		
		auto charMapIt = towels.find(design[i]);
		if (charMapIt != towels.end())
		{
			unsigned int max = static_cast<unsigned int>(i + maxTowelLength > design.size() ? design.size() - i : maxTowelLength);
			for (unsigned int len = max; len > 0; --len)
			{
				auto lenMapIt = charMapIt->second.find(len);
				if (lenMapIt != charMapIt->second.end())
				{
					std::string designSub = design.substr(i, len);
					if (lenMapIt->second.find(designSub) != lenMapIt->second.end())
					{
						branch.emplace_hint(branch.begin(), i + len);
						hasStem[i + len] = true;
						firstCharCompatible = true;
					}
				}
			}
		}

		// Cut off early if the first character does not have a valid branch
		if (!firstCharCompatible)
			return design_tree();

		tree.emplace(std::make_pair(i, branch));
	}

	// If there is no branch that runs to the end, return an empty tree to save the trouble of traversing it again
	if (!hasStem[design.size()])
		return design_tree();

	return tree;
}

u64 CollapseTree(const design_tree& tree, unsigned int designLength)
{
	std::map<unsigned int, u64> lookup;
	lookup.emplace(std::make_pair(designLength, 1));
	for (int i = designLength - 1; i >= 0; --i)
	{
		auto it = tree.find(i);
		if (it == tree.end())
			lookup.emplace(std::make_pair(i, 0));
		else
		{
			u64 totalPaths = 0;
			for (unsigned int n : it->second)
			{
				totalPaths += lookup.find(n)->second;
			}
			lookup.emplace(std::make_pair(i, totalPaths));
		}
	}
	return lookup.find(0)->second;
}

u64 FindAllDesigns(const std::string& design, const towel_container& towels, const unsigned int maxTowelLength)
{
	design_tree designTree = BuildTree(design, towels, maxTowelLength);
	return CollapseTree(designTree, static_cast<unsigned int>(design.size()));
}

int Day19::Solution2()
{
	//std::ifstream file = FileUtil::OpenFile(__FILE__, true);
	std::ifstream file = FileUtil::OpenFile(__FILE__);
	unsigned int maxTowelSize = 0;
	towel_container towelTypes = ParseTowelContainer(file, maxTowelSize);
	std::vector<std::string> towelDesigns = ParseTowelDesigns(file);
	file.close();

	u64 numPossibleDesigns = 0;
	for (const std::string& design : towelDesigns)
	{
		numPossibleDesigns += FindAllDesigns(design, towelTypes, maxTowelSize);
	}

	std::cout << numPossibleDesigns;
	return 0;
}
