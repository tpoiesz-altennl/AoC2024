#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <chrono>

template<typename T>
class horizontal_vector;

class FileUtil
{
public:
	static std::ifstream OpenInputFile(std::string callingLocation)
	{
		std::ifstream file(MakeFolder(callingLocation) + "/InputFile.txt");
		if (!file)
		{
			std::cout << "Oh no!\n";
		}
		return file;
	}

	template<typename T>
	static std::vector<T> ReadInputFileIntoVec(std::string callingLocation)
	{
		std::ifstream file = OpenInputFile(callingLocation);
		if (!file)
		{
			file.close();
			return std::vector<T>();
		}

		std::vector<T> ret;
		T temp;
		while (file >> temp)
		{
			ret.push_back(temp);
		}
		file.close();

		return ret;
	}

	static std::ifstream OpenTestFile(std::string callingLocation)
	{
		std::ifstream file(MakeFolder(callingLocation) + "/TestFile.txt");
		if (!file)
		{
			std::cout << "Oh no!\n";
		}
		return file;
	}

	template<typename T>
	static std::vector<T> ReadTestFileIntoVec(std::string callingLocation)
	{
		std::ifstream file = OpenTestFile(callingLocation);
		if (!file)
		{
			file.close();
			return std::vector<T>();
		}

		std::vector<T> ret;
		T temp;
		while (file >> temp)
		{
			ret.push_back(temp);
		}
		file.close();

		return ret;
	}

	static std::string MakeFolder(const std::string& filepath)
	{
		size_t lastSep, secLastSep;
		lastSep = filepath.rfind('\\');
		secLastSep = filepath.rfind('\\', lastSep - 1) + 1;
		return filepath.substr(secLastSep, lastSep - secLastSep);
	}

	template<typename T>
	static std::vector<horizontal_vector<T>> SplitInputLines(const std::vector<std::string>& input, const char divider,
		std::vector<std::string>& incompatibleLines)
	{
		std::vector<horizontal_vector<T>> ret;
		for (const std::string& s : input)
		{
			if (size_t pos = s.find(divider); pos != std::string::npos)
			{
				horizontal_vector<T> temp;
				std::stringstream ss(s);
				T elem;
				char throwaway;
				do
				{
					ss >> elem >> throwaway;
					temp.push_back(elem);
				} while (throwaway == divider && !ss.eof());
				
				ret.push_back(temp);
			}
			else
				incompatibleLines.push_back(s);
		}

		return ret;
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
			m_Address = FileUtil::MakeFolder(callingLocation) + "/OutputFile.txt";
			m_File = std::ofstream(m_Address);
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
				return;
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
			m_File.flush();
		}

		template<typename T>
		void OutputRule(const T& rule)
		{
			if (!m_File)
			{
				std::cout << "Oh no!\n";
				return;
			}

			m_File << rule << '\n';
			m_File.flush();
		}

		template<typename T>
		void OutputResultGrid(const std::vector<T>& grid)
		{
			if (!m_File)
			{
				std::cout << "Oh no!\n";
				return;
			}

			for (const T& t : grid)
			{
				m_File << t << '\n';
			}
			m_File.flush();
		}

		// Specialisation for no Args...
		template<typename T>
		void OutputMatches(const std::vector<T>& testCases, std::function<bool(const T&)> conditional,
			bool matchConditional)
		{
			if (!m_File)
			{
				std::cout << "Oh no!\n";
				return;
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

		void Overwrite()
		{
			if (!m_File)
			{
				std::cout << "Oh no!\n";
				return;
			}

			m_File.close();
			m_File = std::ofstream(m_Address);
		}

	private:
		std::ofstream m_File;
		std::string m_Address;
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

template<typename T>
std::istream& operator>>(std::istream& stream, std::vector<T>& operand)
{
	operand.clear();
	std::string str = "";
	std::getline(stream, str);
	std::stringstream ss(str);
	T temp;
	while (ss >> temp)
	{
		operand.push_back(temp);
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
