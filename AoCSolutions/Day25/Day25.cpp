#include "Day25.h"
#include "../Util.h"
#include <algorithm>

using container = std::vector<horizontal_vector<unsigned int>>;
using LockOrKey = horizontal_vector<unsigned int>;

void ParseSchematic(const std::vector<std::string>& schematic, container& locks, container& keys)
{
	// Is key
	if (schematic.at(0).at(0) == '.')
	{
		LockOrKey key;
		for (unsigned int column = 0; column < schematic.at(0).size(); ++column)
		{
			unsigned int row = 1; // 0th row cannot contain a #
			while (schematic.at(row).at(column) == '.')
				++row;
			key.push_back(static_cast<unsigned int>(schematic.size()) - row - 1);
		}
		keys.push_back(key);
	}
	// Is lock
	else
	{
		LockOrKey lock;
		for (unsigned int column = 0; column < schematic.at(0).size(); ++column)
		{
			unsigned int row = 1; // 0th row cannot contain a .
			while (schematic.at(row).at(column) == '#')
				++row;
			lock.push_back(row - 1);
		}
		locks.push_back(lock);
	}
}

void ParseFile(container& locks, container& keys, unsigned int& schematicSize)
{
	//std::ifstream file = FileUtil::OpenFile(__FILE__, true);
	std::ifstream file = FileUtil::OpenFile(__FILE__);
	std::string temp;
	while (std::getline(file, temp))
	{
		std::vector<std::string> schematic;
		while (!temp.empty())
		{
			schematic.push_back(temp);
			if (file.eof())
				break;
			std::getline(file, temp);
		}
		ParseSchematic(schematic, locks, keys);
		if (!schematicSize)
			schematicSize = static_cast<unsigned int>(schematic.size());
	}
	file.close();
}

unsigned int TryKeysOnLock(const LockOrKey& lock, container& keys, unsigned int schematicSize)
{
	unsigned int keyIdx = 0;
	for (unsigned int column = 0; column < lock.size(); ++column)
	{
		// Sort by column value, then skip any keys that don't fit with the lock in that column. In the end, only keys that fit all columns will remain.
		// A key fits in a column if, in that column, the key height and lock height add up to two less than the schematic height
		// (since the top and bottom row of the schematic are not considered, given that keys always have the entire bottom row filled and
		// locks have the entire top row filled)
		std::sort(keys.begin() + keyIdx, keys.end(), [column](const LockOrKey& a, const LockOrKey& b) {
			return a[column] > b[column];
			});
		while (keyIdx < keys.size() && keys[keyIdx][column] + lock[column] > schematicSize - 2)
		{
			++keyIdx;
		}
	}
	/*Testing::DebugFile dbg(__FILE__);
	dbg.OutputSomething<LockOrKey>(lock);
	dbg.OutputSomething<char>(' ');
	for (unsigned int i = 0; i < keyIdx; ++i)
	{
		dbg.OutputSomething<LockOrKey>(keys[i]);
		//dbg.OutputSomething<char>('\n');
	}
	__debugbreak();*/
	return static_cast<unsigned int>(keys.size()) - keyIdx;
}

int Day25::Solution1()
{
	container locks, keys;
	unsigned int schematicSize = 0;
	ParseFile(locks, keys, schematicSize);
	u64 totalFitting = 0;
	for (LockOrKey lock : locks)
		totalFitting += TryKeysOnLock(lock, keys, schematicSize);

	std::cout << totalFitting;
	return 0;
}

int Day25::Solution2()
{
	return 0;
}
