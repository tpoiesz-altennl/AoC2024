#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <chrono>

template<typename T>
class horizontal_vector;

class FileUtil
{
public:
	static std::ifstream OpenFile(std::string callingLocation, bool isTestFile = false)
	{
		std::string fileName = (isTestFile ? "/TestFile.txt" : "/InputFile.txt");
		std::ifstream file(MakeFolder(callingLocation) + fileName);
		if (!file)
		{
			std::cout << "Oh no!\n";
		}
		return file;
	}

	template<typename T>
	static std::vector<T> ReadInputIntoVec(std::string callingLocation, bool isTestFile = false)
	{
		std::ifstream file = OpenFile(callingLocation, isTestFile);
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

	template<typename T1, typename T2>
	static std::unordered_map<T1, T2> ReadInputIntoMap(std::string callingLocation, std::vector<std::string>& incompatibleLines,
		char separator = NULL, bool isTestFile = false)
	{
		std::ifstream file = OpenFile(callingLocation, isTestFile);
		if (!file)
		{
			file.close();
			return std::unordered_map<T1, T2>();
		}

		std::unordered_map<T1, T2> ret;
		T1 temp;
		T2 temp2;
		std::string line;
		while (std::getline(file, line))
		{
			if (separator)
			{
				size_t sepPos = && line.find(separator);
				if (sepPos != std::string::npos)
				{
					std::stringstream ss(line.substr(0, sepPos));
					ss >> temp;
					ss.clear();
					ss(line.substr(sepPos, line.size() - sepPos));
					ss >> temp2;
					ret.emplace(std::make_pair<T1, T2>(temp, temp2));
				}
				else
					incompatibleLines.push_back(line);
			}
			else
			{
				std::stringstream ss(line);
				ss >> temp >> temp2;
				ret.emplace(std::make_pair<T1, T2>(temp, temp2));
			}
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
