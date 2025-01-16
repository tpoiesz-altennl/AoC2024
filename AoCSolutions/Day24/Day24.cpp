#include "Day24.h"
#include "../Util.h"
#include <sstream>
#include <map>
#include <queue>

bool AndGate(bool a, bool b)
{
	return a & b;
}

bool OrGate(bool a, bool b)
{
	return a | b;
}

bool XOrGate(bool a, bool b)
{
	return a ^ b;
}

struct Gate
{
	std::string Key1;
	std::string Key2;
	std::string Output;
	std::function<bool(bool, bool)> Operation;

	Gate() : Key1(""), Key2(""), Output(""), Operation()
	{ }

	Gate(std::string input1, std::string input2, std::string output, std::function<bool(bool, bool)> operation)
		: Key1(input1), Key2(input2), Output(output), Operation(operation)
	{ }

	bool CanExecute(const std::map<std::string, bool>& lookup)
	{
		return lookup.find(Key1) != lookup.end() && lookup.find(Key2) != lookup.end();
	}

	std::pair<std::string, bool> Execute(const std::map<std::string, bool>& lookup)
	{
		return std::make_pair(Output, Operation(lookup.find(Key1)->second, lookup.find(Key2)->second));
	}
};

std::map<std::string, bool> ParseWires(std::ifstream& file)
{
	std::map<std::string, bool> ret;
	std::string temp;
	std::getline(file, temp);
	while (!temp.empty()) // This runs until the first empty line in the file is read into temp (after which the gates start)
	{
		std::stringstream ss(temp);
		std::string key;
		bool value;
		ss >> key >> value;
		key.erase(key.end() - 1); // Remove colon at end of key
		ret.emplace(std::make_pair(key, value));
		std::getline(file, temp);
	}
	return ret;
}

std::queue<Gate> ParseGates(std::ifstream& file)
{
	std::queue<Gate> ret;
	std::string temp;
	while (std::getline(file, temp))
	{
		std::stringstream ss(temp);
		std::string key1, operation, key2, throwaway, outputKey;
		ss >> key1 >> operation >> key2 >> throwaway >> outputKey;
		std::function<bool(bool, bool)> func;
		switch (operation[0])
		{
		case 'A':
			func = &AndGate;
			break;
		case 'O':
			func = &OrGate;
			break;
		case 'X':
		default:
			func = &XOrGate;
		}
		ret.push(Gate(key1, key2, outputKey, func));
	}
	return ret;
}

int Day24::Solution1()
{
	//std::ifstream file = FileUtil::OpenFile(__FILE__, true);
	std::ifstream file = FileUtil::OpenFile(__FILE__);
	std::map<std::string, bool> wires = ParseWires(file);
	std::queue<Gate> gates = ParseGates(file);
	file.close();

	while (!gates.empty())
	{
		Gate current = gates.front();
		gates.pop();
		if (current.CanExecute(wires))
		{
			wires.emplace(current.Execute(wires));
		}
		else
		{
			gates.push(current);
		}
	}

	u64 output = 0;
	for (auto wire : wires)
	{
		if (wire.first[0] == 'z' && wire.second)
		{
			output += (static_cast<u64>(1) << std::stoi(wire.first.substr(1, 2)));
		}
	}

	std::cout << output;
	return 0;
}

int Day24::Solution2()
{
	return 0;
}
