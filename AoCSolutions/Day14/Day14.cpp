#include "Day14.h"
#include "../Util.h"

#include <vector>
#include <tuple>
#include <regex>

static constexpr unsigned int c_GridW = 101, c_GridH = 103, c_TestGridW = 11, c_TestGridH = 7;

std::vector<std::pair<vec2, vec2>> ParseRobotEquations(bool isTest)
{
	std::ifstream file = FileUtil::OpenFile(__FILE__, isTest);
	unsigned int w = isTest ? c_TestGridW : c_GridW;
	unsigned int h = isTest ? c_TestGridH : c_GridH;
	std::vector<std::pair<vec2, vec2>> ret;
	std::string temp;
	while (std::getline(file, temp))
	{
		std::regex findNum("\\d+");
		std::smatch match;
		std::regex_search(temp.cbegin(), temp.cend(), match, findNum);
		int bx = std::stoi(match[0].str());
		std::regex_search(match.suffix().first, temp.cend(), match, findNum);
		int by = std::stoi(match[0].str());
		std::regex_search(match.suffix().first, temp.cend(), match, findNum);
		int ax = std::stoi(match[0].str());
		if (match.prefix().str().back() == '-')
			ax = w - ax;
		std::regex_search(match.suffix().first, temp.cend(), match, findNum);
		int ay = std::stoi(match[0].str());
		if (match.prefix().str().back() == '-')
			ay = h - ay;

		ret.push_back(std::pair<vec2, vec2>(vec2(ax, bx), vec2(ay, by)));
	}

	file.close();
	return ret;
}

vec2 GetRobotPosAtT(const std::pair<vec2, vec2>& robotEquations, const u64 t, bool isTestGrid = false)
{
	unsigned int w = isTestGrid ? c_TestGridW : c_GridW;
	unsigned int h = isTestGrid ? c_TestGridH : c_GridH;
	int x = (static_cast<u64>(robotEquations.first.x) * t + robotEquations.first.y) % w;
	int y = (static_cast<u64>(robotEquations.second.x) * t + robotEquations.second.y) % h;
	return vec2(x, y);
}

#pragma warning(push)
#pragma warning(disable : 4018) // signed/unsigned mismatch (don't care)
int Day14::Solution1()
{
	//std::vector<std::pair<vec2, vec2>> robots = ParseRobotEquations(true);
	std::vector<std::pair<vec2, vec2>> robots = ParseRobotEquations(false);

	// Quadrant scores
	unsigned int topLeftQ = 0, topRightQ = 0, bottomLeftQ = 0, bottomRightQ = 0;
	unsigned int time = 100;
	//unsigned int gridMiddleX = c_TestGridW / 2, gridMiddleY = c_TestGridH / 2;
	unsigned int gridMiddleX = c_GridW / 2, gridMiddleY = c_GridH / 2;

	for (const auto& eq : robots)
	{
		//vec2 robotPos = GetRobotPosAtT(eq, time, true);
		vec2 robotPos = GetRobotPosAtT(eq, time, false);
		if (robotPos.x < gridMiddleX)
		{
			if (robotPos.y < gridMiddleY)
				topLeftQ += 1;
			else if (robotPos.y > gridMiddleY)
				bottomLeftQ += 1;
		}
		else if (robotPos.x > gridMiddleX)
		{
			if (robotPos.y < gridMiddleY)
				topRightQ += 1;
			else if (robotPos.y > gridMiddleY)
				bottomRightQ += 1;
		}
	}

	std::cout << topLeftQ * topRightQ * bottomLeftQ * bottomRightQ;

	return 0;
}
#pragma warning(pop)

Grid BuildGrid(std::vector<vec2>& robotPositions, bool isTestGrid = false)
{
	std::sort(robotPositions.begin(), robotPositions.end(), [](const vec2& a, const vec2& b) -> bool {
		return a.y < b.y || (a.y == b.y && a.x < b.x);
		});

	std::vector<std::string> grid;

	unsigned int w = isTestGrid ? c_TestGridW : c_GridW;
	unsigned int h = isTestGrid ? c_TestGridH : c_GridH;
	unsigned int posIdx = 0;

	while (grid.size() < h)
	{
		std::string row(w, '.');
		while (posIdx < robotPositions.size() && robotPositions[posIdx].y == grid.size())
		{
			row[robotPositions[posIdx].x] = '0';
			++posIdx;
		}
		grid.push_back(row);
	}

	return grid;
}

bool MightBeChristmasTree(const Grid& grid)
{
	unsigned int peakY = 0;
	while (peakY < grid.size() - 2)
	{
		unsigned int peakX = static_cast<unsigned int>(grid.at(peakY).find('0'));
		while (peakX != std::string::npos)
		{
			if (peakX > 1 && peakX < grid.width() - 2)
				if (grid.at(peakX - 1, peakY + 1) == '0' && grid.at(peakX + 1, peakY + 1) == '0')
					if (grid.at(peakX - 2, peakY + 2) == '0' && grid.at(peakX + 2, peakY + 2) == '0')
						return true;

			peakX = static_cast<unsigned int>(grid.at(peakY).find('0', peakX + 1));
		}
		++peakY;
	}
	return false;
}

int Day14::Solution2()
{
	//std::vector<std::pair<vec2, vec2>> robots = ParseRobotEquations(true);
	std::vector<std::pair<vec2, vec2>> robots = ParseRobotEquations(false);

	Testing::DebugFile dbg(__FILE__);

	std::vector<vec2> positions(robots.size());
	for (unsigned int t = 0; t < 10403; ++t) // After 10403 seconds (incidentally, gridW * gridH), the robots loop
	{
		for (unsigned int i = 0; i < robots.size(); ++i)
		{
			const auto& eq = robots[i];
			//positions[i] = GetRobotPosAtT(eq, t, true);
			positions[i] = GetRobotPosAtT(eq, t, false);
		}
		//Grid grid = BuildGrid(positions, true);
		Grid grid = BuildGrid(positions, false);
		if (MightBeChristmasTree(grid))
		{
			dbg.Overwrite();
			dbg.OutputGrid(grid);
			__debugbreak();
		}
		std::cout << t << '\r';
	}

	return 0;
}
