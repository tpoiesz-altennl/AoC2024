#include "Day9.h"
#include "../Util.h"

struct File
{
	int fileNum;
	int size;

	File() : fileNum(0), size(0)
	{
	}

	File(int _num, int _size) : fileNum(_num), size(_size)
	{
	}
};

void OutputToDebug(Testing::DebugFile& dbg, const horizontal_vector<File>& fileList)
{
	std::string output = "";
	for (int i = 0; i < fileList[0].size; ++i)
	{
		output += "0|";
	}
	for (unsigned int i = 1; i < fileList.size(); ++i)
	{
		if (fileList[i].fileNum > 0)
		{
			for (int j = 0; j < fileList[i].size; ++j)
			{
				output += std::to_string(fileList[i].fileNum);
				output += '|';
			}
		}
		else
		{
			for (int j = 0; j < fileList[i].size; ++j)
				output += '.';
			output += '|';
		}
	}
	dbg.OutputRule<std::string>(output);
}

int Day9::Solution1()
{
	horizontal_vector<int> input = FileUtil::ParseFileCharByChar(__FILE__);
	if (input.size() % 2 == 0)
		input.pop_back(); // Remove empty space at the end

	unsigned int frontIndex = 0, backIndex = static_cast<unsigned int>(input.size() - 1);
	unsigned int actualFileIndex = 0;
	unsigned long long checkSum = 0;
	while (frontIndex < backIndex)
	{
		if (frontIndex % 2 == 0)
		{
			unsigned int fileNum = frontIndex / 2;
			for (int i = 0; i < input[frontIndex]; ++i)
			{
				checkSum += fileNum * actualFileIndex++;
			}
			++frontIndex;
		}
		else
		{
			while (input[frontIndex] > 0)
			{
				unsigned int fileNum = backIndex / 2;
				int toSubtract = input[backIndex] - input[frontIndex] < 0 ? input[backIndex] : input[frontIndex];
				for (int i = 0; i < toSubtract; ++i)
				{
					checkSum += fileNum * actualFileIndex++;
				}
				input[frontIndex] -= toSubtract;
				input[backIndex] -= toSubtract;
				if (input[backIndex] == 0)
					backIndex -= 2;
			}
			++frontIndex;
		}
	}
	// Add files at the end to checksum (above while-loop stops after all the moving's done)
	if (frontIndex % 2 == 0)
	{
		unsigned int fileNum = frontIndex / 2;
		for (int i = 0; i < input[frontIndex]; ++i)
		{
			checkSum += fileNum * actualFileIndex++;
		}
	}

	std::cout << checkSum;

	return 0;
}

int Day9::Solution2()
{
	horizontal_vector<int> input = FileUtil::ParseFileCharByChar(__FILE__);
	if (input.size() % 2 == 0)
		input.pop_back(); // Remove empty space at the end

	//Testing::DebugFile dbg(__FILE__);

	horizontal_vector<File> files;
	for (size_t i = 0; i < input.size(); ++i)
	{
		if (i % 2 == 0)
		{
			files.push_back(File(static_cast<int>(i) / 2, input[i]));
		}
		else
		{
			files.push_back(File(0, input[i])); // Empty space gets fileNum 0
		}
	}
	//OutputToDebug(dbg, files);

	// input.size() - 1 is even, so this does not run indefinitely
	for (unsigned int backIndex = static_cast<unsigned int>(input.size()) - 1; backIndex > 0; backIndex -= 2)
	{
		File toBeMoved = files[backIndex];
		for (unsigned int freeSpaceIndex = 1; freeSpaceIndex < backIndex; freeSpaceIndex += 2)
		{
			File emptySpace = files[freeSpaceIndex];
			if (emptySpace.fileNum == 0 && emptySpace.size >= toBeMoved.size)
			{
				// Insert empty space indicators around moved file
				// Starting with right after moved file, as to not invalidate freeSpaceIndex
				files.insert(files.begin() + freeSpaceIndex + 1, File(0, emptySpace.size - toBeMoved.size));
				files[freeSpaceIndex] = toBeMoved;
				files.insert(files.begin() + freeSpaceIndex, File());
				// Update backwards iterator
				backIndex += 2;
				// Prettify for debug output
				/*if (backIndex + 1 > files.size() - 1)
				{
					// Remove trailing whitespace
					files.erase(files.begin() + backIndex - 1, files.end());
				}
				else
				{
					// Consolidate whitespace
					File inFront = files[backIndex - 1], behind = files[backIndex + 1];
					files[backIndex - 1] = File(0, inFront.size + toBeMoved.size + behind.size);
					files.erase(files.begin() + backIndex, files.begin() + backIndex + 2);
				}
				OutputToDebug(dbg, files);*/
				// Or just do the efficient thing
				files[backIndex].fileNum = 0;
				break;
			}
		}
	}

	unsigned long long checkSum = 0;
	unsigned int filePos = 0;
	for (File f : files)
	{
		if (f.fileNum > 0)
		{
			for (int i = 0; i < f.size; ++i)
			{
				checkSum += static_cast<unsigned long long>(f.fileNum) * filePos++;
			}
		}
		else
		{
			filePos += f.size;
		}
	}

	std::cout << checkSum;

	return 0;
}
