#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <chrono>

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

	static std::string MakeFolder(const std::string& filepath)
	{
		size_t lastSep, secLastSep;
		lastSep = filepath.rfind('\\');
		secLastSep = filepath.rfind('\\', lastSep - 1) + 1;
		return filepath.substr(secLastSep, lastSep - secLastSep);
	}
};

class Testing
{
public:
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
			bool matchConditional, Args... argsForConditional)
		{
			if (!m_File)
			{
				std::cout << "Oh no!\n";
			}

			int caseNum = 0;
			for (const T& testCase : testCases)
			{
				++caseNum;
				if (conditional(testCase, argsForConditional...) == matchConditional)
				{
					m_File << caseNum << '\t' << testCase << std::endl;
				}
			}
		}

		// Specialisation for no Args...
		template<typename T>
		void OutputMatches(const std::vector<T>& testCases, std::function<bool(const T&)> conditional,
			bool matchConditional)
		{
			if (!m_File)
			{
				std::cout << "Oh no!\n";
			}

			int caseNum = 0;
			for (const T& testCase : testCases)
			{
				++caseNum;
				if (conditional(testCase) == matchConditional)
				{
					m_File << caseNum << '\t' << testCase << std::endl;
				}
			}
		}

	private:
		std::ofstream m_File;
	};

	static void TimeSolution(std::function<int()> solution, unsigned long long numIterations)
	{
		double avgDuration = 0.;
		for (int i = 0; i < numIterations; ++i)
		{
			auto t1 = std::chrono::steady_clock::now();
			int exitCode = solution();
			auto t2 = std::chrono::steady_clock::now();
			avgDuration += std::chrono::duration<double>(t2 - t1).count();
			if (exitCode)
				exit(exitCode);
		}
		avgDuration /= numIterations;

		std::cout << '\n' << avgDuration << '\n';
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
