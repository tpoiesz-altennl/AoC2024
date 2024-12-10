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
class horizontal_vector : public std::vector<T>
{
	// This class needs no body; it only exists to differentiate vectors that need to be printed horizontally
};

#pragma region fileUtil
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

	template <typename T1, typename T2>
	static bool MakeMapEntryFromLine(const std::string& line, std::pair<T1, T2>& output, const char separator = NULL)
	{
		T1 tempKey;
		T2 tempVal;
		if (separator && !line.empty())
		{
			size_t sepPos = line.find(separator);
			if (sepPos != std::string::npos)
			{
				std::stringstream ss(line.substr(0, sepPos));
				ss >> tempKey;
				std::stringstream ss2(line.substr(sepPos + 1, line.size() - sepPos));
				ss2 >> tempVal;

				output = std::make_pair(tempKey, tempVal);
			}
			else
			{
				return false;
			}
		}
		else if (!line.empty())
		{
			std::stringstream ss(line);
			ss >> tempKey >> tempVal;
			output = std::make_pair(tempKey, tempVal);
		}
		else
			return false;

		return true;
	}

	template <typename T1, typename T2>
	static std::unordered_map<T1, horizontal_vector<T2>> ReadInputIntoLookupTable(std::string callingLocation,
		std::vector<std::string>& incompatibleLines, char separator = NULL, bool isTestFile = false)
	{
		std::ifstream file = OpenFile(callingLocation, isTestFile);
		if (!file)
		{
			file.close();
			return std::unordered_map<T1, horizontal_vector<T2>>();
		}

		std::unordered_map<T1, horizontal_vector<T2>> ret;

		std::string line;
		while (std::getline(file, line))
		{
			std::pair<T1, T2> entry;
			if (MakeMapEntryFromLine(line, entry, separator))
			{
				if (auto it = ret.find(entry.first); it != ret.end())
				{
					(*it).second.push_back(entry.second);
				}
				else
				{
					horizontal_vector<T2> tempVec;
					tempVec.push_back(entry.second);
					ret.emplace(std::make_pair(entry.first, tempVec));
				}
			}
			else
				incompatibleLines.push_back(line);
		}

		file.close();
		return ret;
	}

	// WIP
	/*template<typename T1, typename T2, typename ContainerType>
	static std::unordered_map<T1, ContainerType> ReadInputIntoMap(std::string callingLocation, std::vector<std::string>& incompatibleLines,
		char separator = NULL, bool uniqueKeysOnly = true, bool isTestFile = false)
	{
		std::ifstream file = OpenFile(callingLocation, isTestFile);
		if (!file)
		{
			file.close();
			return std::unordered_map<T1, ContainerType>();
		}

		std::unordered_map<T1, ContainerType> ret;
		T1 tempKey;
		ContainerType tempVal;
		std::string line;
		while (std::getline(file, line))
		{
			if (separator)
			{
				size_t sepPos = line.find(separator);
				if (sepPos != std::string::npos)
				{
					std::stringstream ss(line.substr(0, sepPos));
					ss >> tempKey;
					ss.clear();
					ss(line.substr(sepPos, line.size() - sepPos));
					ss >> tempVal;

					if (uniqueKeysOnly)
						ret.emplace(std::make_pair<T1, ContainerType>(tempKey, tempVal));
					else
					{
						if (auto it = ret.find(tempKey); it != ret.end())
						{
							if (std::is_same<std::vector<T2>,ContainerType>::value)
							{
								(*it).second.push_back(tempVal);
							}
						}
						else
							ret.emplace(std::make_pair<T1, ContainerType>(tempKey, { tempVal }));
					}
				}
				else
					incompatibleLines.push_back(line);
			}
			else
			{
				std::stringstream ss(line);
				ss >> tempKey >> tempVal;
				if (uniqueKeysOnly)
					ret.emplace(std::make_pair<T1, ContainerType>(tempKey, tempVal));
				else
				{
					if (auto it = ret.find(tempKey); it != ret.end())
						(*it).push_back(tempKey);
					else;
						//ret.emplace(std::make_pair<T1, std::set<ContainerType>>(tempKey, std::set<T2>(tempVal)));
				}
			}
		}
		file.close();
		return ret;
	}*/

	static std::string MakeFolder(const std::string& filepath)
	{
		size_t lastSep, secLastSep;
		lastSep = filepath.rfind('\\');
		secLastSep = filepath.rfind('\\', lastSep - 1) + 1;
		return filepath.substr(secLastSep, lastSep - secLastSep);
	}

	template<typename T>
	static std::vector<horizontal_vector<T>> SplitInputLines(const std::vector<std::string>& input, const char separator,
		std::vector<std::string>& incompatibleLines)
	{
		std::vector<horizontal_vector<T>> ret;
		for (const std::string& s : input)
		{
			if (size_t pos = s.find(separator); pos != std::string::npos)
			{
				horizontal_vector<T> temp;
				std::stringstream ss(s);
				T elem;
				char throwaway;
				do
				{
					ss >> elem >> throwaway;
					temp.push_back(elem);
				} while (throwaway == separator && !ss.eof());
				
				ret.push_back(temp);
			}
			else
				incompatibleLines.push_back(s);
		}

		return ret;
	}

	static horizontal_vector<int> ParseFileCharByChar(std::string callingLocation, bool isTestFile = false)
	{
		std::ifstream file = OpenFile(callingLocation, isTestFile);
		if (!file)
		{
			file.close();
			return horizontal_vector<int>();
		}

		horizontal_vector<int> ret;
		char c;
		while (file >> c)
		{
			ret.push_back(c - '0');
		}

		file.close();
		return ret;
	}
};
#pragma endregion

#pragma region debugUtil
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
#pragma endregion

#pragma region IOoperators
template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& operand)
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

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1, T2>& operand)
{
	stream << operand.first << ": " << operand.second;
	return stream;
}

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::unordered_map<T1, T2>& operand)
{
	for (const auto& pair : operand)
	{
		stream << pair;
	}
	return stream;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, horizontal_vector<T> operand)
{
	for (const T& elem : operand)
	{
		stream << elem << ' ';
	}
	return stream;
}
#pragma endregion

#pragma region UsefulStructs
enum class Direction : unsigned int
{
	None = 0,
	UpLeft,
	Up,
	UpRight,
	Left,
	Right,
	DownLeft,
	Down,
	DownRight
};

// Order: up-left, up, up-right, left, right, down-left, down, down-right, none
Direction& operator++(Direction& dir);

enum class OrthDirection : unsigned int
{
	None = 0,
	Up,
	Right,
	Down,
	Left
};

// Order: up, right, down, left, loops to up
OrthDirection& operator++(OrthDirection& dir);
OrthDirection& operator--(OrthDirection& dir);

struct vec2
{
	int x, y;

	vec2() : x(0), y(0)
	{
	}
	vec2(int _x, int _y) : x(_x), y(_y)
	{
	}

	vec2 operator+(const vec2& other) const
	{
		return vec2(x + other.x, y + other.y);
	}
	vec2 operator-(const vec2& other) const
	{
		return vec2(x - other.x, y - other.y);
	}

	vec2 operator*(int a) const
	{
		return vec2(x * a, y * a);
	}

	bool operator==(const vec2& other) const
	{
		return other.x == x && other.y == y;
	}

	unsigned int dist(const vec2& other) const
	{
		return std::abs(other.x - x) + std::abs(other.y - y);
	}
};
#pragma endregion
