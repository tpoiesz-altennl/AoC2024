#include "Day23.h"
#include "../Util.h"
#include <map>
#include <set>
#include <algorithm>
#include <span>

std::map<std::string, std::vector<std::string>> BuildConnections(const std::vector<horizontal_vector<std::string>>& pairs)
{
	std::map<std::string, std::vector<std::string>> ret;
	auto insertEntry = [&ret](std::string computer1, std::string computer2) -> void {
		auto it = ret.find(computer1);
		if (it == ret.end())
			ret.emplace(computer1, std::vector<std::string>({ computer2 }));
		else
			if (std::find(it->second.begin(), it->second.end(), computer2) == it->second.end())
				it->second.push_back(computer2);
		};

	for (auto pair : pairs)
	{
		std::string computer1 = pair[0], computer2 = pair[1];
		insertEntry(computer1, computer2);
		insertEntry(computer2, computer1);
	}

	return ret;
}

std::set<std::set<std::string>> FindUniqueTriangles(const std::map<std::string, std::vector<std::string>>& connections, const char mustInclude = 0)
{
	std::set<std::set<std::string>> uniqueTriangles;
	for (auto computer : connections)
	{
		if (!mustInclude || computer.first[0] == mustInclude)
		{
			for (std::string connected : computer.second)
			{
				for (std::string alsoConnected : computer.second)
				{
					if (connected != alsoConnected)
					{
						std::vector<std::string> connectionsOfConnected = connections.find(connected)->second;
						if (std::find(connectionsOfConnected.cbegin(), connectionsOfConnected.cend(), alsoConnected) != connectionsOfConnected.cend())
							uniqueTriangles.insert(std::set<std::string>({ computer.first, connected, alsoConnected }));
					}
				}
			}
		}
	}
	return uniqueTriangles;
}

int Day23::Solution1()
{
	std::vector<std::string> shouldBeEmpty;
	std::vector<horizontal_vector<std::string>> input = 
		FileUtil::SplitInputLines<std::string>(FileUtil::ReadInputIntoVec<std::string>(__FILE__), '-', shouldBeEmpty);
	/*std::vector<horizontal_vector<std::string>> input = 
		FileUtil::SplitInputLines<std::string>(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true), '-', shouldBeEmpty);*/
	if (shouldBeEmpty.size())
		return 1;
	std::map<std::string, std::vector<std::string>> connections = BuildConnections(input);

	std::set<std::set<std::string>> uniqueTriangles = FindUniqueTriangles(connections, 't');
	for (auto triangle : uniqueTriangles)
	{
		for (auto it = triangle.begin(); it != triangle.end(); ++it)
			std::cout << *it << ' ';
		std::cout << '\n';
	}
	std::cout << uniqueTriangles.size();
	return 0;
}

using Computer = std::pair<std::string, std::vector<std::string>>;

bool IsConnectedSet(const std::vector<std::reference_wrapper<Computer>>& computers,
	std::set<std::set<std::string>>& unconnected)
{
	bool connected = true;
	for (unsigned int i = 0; i < computers.size() - 1; ++i)
	{
		const std::vector<std::string>& connections = computers[i].get().second;
		for (int j = i + 1; j < computers.size(); ++j)
		{
			if (std::find(connections.cbegin(), connections.cend(), computers[j].get().first) == connections.cend())
			{
				unconnected.insert({ computers[i].get().first, computers[j].get().first });
				connected = false;
			}
		}
	}
	return connected;
}

bool CheckSubset(const std::span<Computer>& entireSet, std::vector<std::reference_wrapper<Computer>>& builtSubset,
	std::set<std::set<std::string>>& unconnected,
	unsigned int leftFence, unsigned int recurseDepth)
{
	// Returns index of second element that causes a problem, or -1 if there is no problem
	auto preCheckConnectivity = [&]() -> int {
		for (const auto& unconnectedPair : unconnected)
		{
			auto it1 = std::find_if(builtSubset.begin(), builtSubset.end(), [&unconnectedPair](const std::reference_wrapper<Computer>& elem) {
				return *(unconnectedPair.begin()) == elem.get().first;
				}),
				it2 = std::find_if(builtSubset.begin(), builtSubset.end(), [&unconnectedPair](const std::reference_wrapper<Computer>& elem) {
				return *(std::next(unconnectedPair.begin(), 1)) == elem.get().first;
					});
			if (it1 != builtSubset.end() && it2 != builtSubset.end())
			{
				int it1Dist = static_cast<int>(std::distance(builtSubset.begin(), it1)),
					it2Dist = static_cast<int>(std::distance(builtSubset.begin(), it2));
				return (it1Dist > it2Dist ? it1Dist : it2Dist);
			}
		}
		return -1;
		};

	if (recurseDepth == 0)
	{
		if (preCheckConnectivity() == -1)
			return IsConnectedSet(builtSubset, unconnected);
		else
			return false;
	}

	for (unsigned int i = leftFence; i <= entireSet.size() - recurseDepth; ++i)
	{
		builtSubset.push_back(entireSet[i]);
		if (int incompatibleIndex = preCheckConnectivity(); incompatibleIndex != -1)
		{
			builtSubset.pop_back();
			// Non-matching index is further up in the branch, so no need to keep checking this branch
			if (incompatibleIndex < builtSubset.size())
				return false;
			else
				continue;
		}
		if (CheckSubset(entireSet, builtSubset, unconnected, i + 1, recurseDepth - 1))
			return true;
		else
			builtSubset.pop_back();
	}

	return false;
}

bool BruteForce(const std::span<Computer>& searchSet, std::set<std::string>& found)
{
	unsigned int numConnectionsToLookFor = static_cast<unsigned int>(searchSet.back().second.size());
	std::vector<std::reference_wrapper<Computer>> compatibleSubset;
	std::set<std::set<std::string>> unconnectedPairs;
	if (CheckSubset(searchSet, compatibleSubset, unconnectedPairs, 0, numConnectionsToLookFor))
	{
		for (auto computer : compatibleSubset)
		{
			found.insert(computer.get().first);
		}
		return true;
	}
	return false;
}

int Day23::Solution2ver1()
{
	std::vector<std::string> shouldBeEmpty;
	/*std::vector<horizontal_vector<std::string>> input =
		FileUtil::SplitInputLines<std::string>(FileUtil::ReadInputIntoVec<std::string>(__FILE__), '-', shouldBeEmpty);*/
	std::vector<horizontal_vector<std::string>> input =
		FileUtil::SplitInputLines<std::string>(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true), '-', shouldBeEmpty);
	if (shouldBeEmpty.size())
		return 1;
	std::map<std::string, std::vector<std::string>> connections = BuildConnections(input);
	std::vector<Computer> computers;
	for (auto computer : connections)
	{
		computers.push_back(computer);
	}
	// Find the largest subset of interconnected computers
	std::set<std::string> interconnectedSet;
	{
		std::sort(computers.begin(), computers.end(), [](const Computer& a, const Computer& b) {
			return a.second.size() > b.second.size();
			});
		// Index of first element that has fewer connections than all the previous computers
		unsigned int i = 0;
		// Use i as "the number of computers before the current subset of computers with x connections";
		// skip all subsets of computers that have fewer total computers in them than the number of connections allows.
		// Computers are sorted by number of connections in descending order, so we only have to do this skip once
		while (i + 1 <= computers.size() && i < computers[i].second.size())
			++i;
		std::span<Computer> subset;
		do
		{
			// If we enter the 'do' section with i already being at the end of the vector, we cannot find an interconnected set
			if (i + 1 > computers.size())
				break;
			// Skip to the first computer that has fewer connections than the subset currently being checked has
			while (i + 1 <= computers.size() && computers[i].second.size() == computers[i - 1].second.size())
				++i;
			// Includes all computers that come before i
			subset = std::span<Computer>({ computers.begin(), computers.begin() + i });
		} while (!BruteForce(subset, interconnectedSet));
	}
	for (const auto& computer : interconnectedSet)
	{
		std::cout << computer << ',';
	}
	
	return 0;
}

void FindBestConnectedSet(const std::string& current, const std::vector<std::string>& elements,
	const std::map<std::string, std::vector<std::string>>& connections, std::vector<std::string>& currentTry, 
	std::set<std::string>& result, unsigned int leftFence, size_t& bestSetSize, bool& improvedResult)
{
	// Check if the number of elements we can still add could even make a better set
	if (currentTry.size() + elements.size() - leftFence < bestSetSize)
		return;

	// Check if new element fits, don't consider connection to first element (root node), since the elements list has been pre-pruned for that
	for (unsigned int i = 1; i < currentTry.size(); ++i)
	{
		const std::vector<std::string>& connected = connections.find(currentTry[i])->second;
		if (std::find(connected.cbegin(), connected.cend(), current) == connected.cend())
			return;
	}

	// Add new element
	currentTry.push_back(current);
	if (currentTry.size() > bestSetSize)
	{
		bestSetSize = currentTry.size();
		result.clear();
		for (std::string element : currentTry)
			result.insert(element);
		improvedResult = true;
	}

	// Expand to next elements, don't consider any that have been tried in previous iterations (resulting sets would be at most equally large)
	for (unsigned int i = leftFence; i < elements.size(); ++i)
	{
		FindBestConnectedSet(elements[i], elements, connections, currentTry, result, i + 1, bestSetSize, improvedResult);
	}

	// Pop the element, so a subsequent attempt can try to make a bigger set without this element
	currentTry.pop_back();
}

int Day23::Solution2ver2()
{
	std::vector<std::string> shouldBeEmpty;
	std::vector<horizontal_vector<std::string>> input =
		FileUtil::SplitInputLines<std::string>(FileUtil::ReadInputIntoVec<std::string>(__FILE__), '-', shouldBeEmpty);
	/*std::vector<horizontal_vector<std::string>> input =
		FileUtil::SplitInputLines<std::string>(FileUtil::ReadInputIntoVec<std::string>(__FILE__, true), '-', shouldBeEmpty);*/
	if (shouldBeEmpty.size())
		return 1;
	std::map<std::string, std::vector<std::string>> connections = BuildConnections(input);
	std::vector<std::string> computers;
	for (const auto& pair : connections)
	{
		computers.push_back(pair.first);
	}

	std::set<std::string> bestSet;
	size_t bestSetSize = 0;
	while (computers.size())
	{
		std::vector<std::string> attempt;
		std::set<std::string> newBest;
		std::string currComputer = computers[0];
		computers.erase(computers.begin());
		std::vector<std::string> pruned = computers;
		{ // Prune computers by removing any entries that don't connect to the root node
			std::vector<std::string> rootConnections = connections.find(currComputer)->second;
			for (int i = static_cast<int>(pruned.size()) - 1; i >= 0; --i)
			{
				if (std::find(rootConnections.cbegin(), rootConnections.cend(), pruned[i]) == rootConnections.cend())
					pruned.erase(pruned.begin() + i);
			}
		}
		bool newIsBetter = false;
		FindBestConnectedSet(currComputer, pruned, connections, attempt, newBest, 0, bestSetSize, newIsBetter);
		if (newIsBetter)
		{
			bestSet = newBest;
			bestSetSize = newBest.size();
		}
	}

	/*for (const std::string& computer : bestSet)
		std::cout << computer << ',';*/

	return 0;
}
