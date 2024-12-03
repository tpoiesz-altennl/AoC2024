#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>

class FileUtil
{
public:
	static std::ifstream ReadInputFile(std::string callingLocation)
	{
		std::ifstream file(MakeFolder(callingLocation) + "/InputFile.txt");
		if (!file)
		{
			std::cout << "Oh no!\n";
		}
		return file;
	}

	static std::ifstream ReadTestFile(std::string callingLocation)
	{
		std::ifstream file(MakeFolder(callingLocation) + "/TestFile.txt");
		if (!file)
		{
			std::cout << "Oh no!\n";
		}
		return file;
	}

	class DebugFile
	{
	public:
		DebugFile(const std::string callingLocation)
		{
			m_File = std::ofstream(FileUtil::MakeFolder(callingLocation) + "/OutputFile.txt");
		}
		~DebugFile()
		{
			m_File.close();
		}

		template<typename T, typename... Args>
		void OutputMatches(const std::vector<T>& testCases, std::function<bool(const T&, Args...)> conditional,
			bool matchConditional = true, Args... argsForConditional)
		{
			if (!m_File)
			{
				std::cout << "Oh no!\n";
			}

			for (const T& testCase : testCases)
			{
				if (conditional(testCase, argsForConditional...) == matchConditional)
				{
					m_File << testCase << std::endl;
				}
			}
		}
	private:
		std::ofstream m_File;
	};

private:
	static std::string MakeFolder(const std::string& filepath)
	{
		size_t lastSep, secLastSep;
		lastSep = filepath.rfind('\\');
		secLastSep = filepath.rfind('\\', lastSep - 1) + 1;
		return filepath.substr(secLastSep, lastSep - secLastSep);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, std::vector<T> operand)
{
	for (const T& elem : operand)
	{
		stream << elem << '\n';
	}
	return stream;
}

template<class T>
class horizontal_vector : public std::vector<T>
{
	// This class needs no body; it only exists to differentiate vectors that need to be printed horizontally
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, horizontal_vector<T> operand)
{
	for (const T& elem : operand)
	{
		stream << elem << ' ';
	}
	return stream;
}
