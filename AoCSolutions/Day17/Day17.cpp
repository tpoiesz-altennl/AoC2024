#include "Day17.h"
#include "../Util.h"
#include <iostream>

static unsigned long long A, B, C;
static int InstructionPointer;

unsigned long long comboOp(int operand)
{
	switch (operand)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return operand;
	case 4:
		return A;
	case 5:
		return B;
	case 6:
		return C;
	default:
		throw 0;
	}
}

// instruction 0
void adv(int operand)
{
	A = A >> comboOp(operand);
}

// instruction 1
void bxl(int operand)
{
	B = B ^ operand;
}

// instruction 2
void bst(int operand)
{
	B = comboOp(operand) & 0b111;
}

// instruction 3
void jnz(int operand)
{
	if (A)
		InstructionPointer = operand - 2;
}

// instruction 4
void bxc(int operand)
{
	B = B ^ C;
}

// instruction 5
void out(int operand)
{
	std::cout << (comboOp(operand) & 0b111) << ',';
}

// instruction 6
void bdv(int operand)
{
	B = A >> comboOp(operand);
}

// instruction 7
void cdv(int operand)
{
	C = A >> comboOp(operand);
}

horizontal_vector<int> ParseInput(bool isTest = false)
{
	std::ifstream file = FileUtil::OpenFile(__FILE__, isTest);
	size_t registerOffset = 12;
	std::string temp;
	std::getline(file, temp);
	A = std::stoi(temp.substr(registerOffset));
	std::getline(file, temp);
	B = std::stoi(temp.substr(registerOffset));
	std::getline(file, temp);
	C = std::stoi(temp.substr(registerOffset));
	size_t programOffset = 9;
	std::getline(file, temp);
	std::getline(file, temp);
	temp = temp.substr(programOffset);
	std::vector<std::string> throwaway;
	horizontal_vector<int> instructions = FileUtil::SplitInputLines<int>(std::vector<std::string>({ temp }), ',', throwaway)[0];
	file.close();
	return instructions;
}

int Day17::Solution1()
{
	horizontal_vector<int> program = ParseInput();
	InstructionPointer = 0;
	try
	{
		while (InstructionPointer < program.size() - 1)
		{
			int operand = program[InstructionPointer + 1];
			switch (program[InstructionPointer])
			{
			case 0:
				adv(operand);
				break;
			case 1:
				bxl(operand);
				break;
			case 2:
				bst(operand);
				break;
			case 3:
				jnz(operand);
				break;
			case 4:
				bxc(operand);
				break;
			case 5:
				out(operand);
				break;
			case 6:
				bdv(operand);
				break;
			case 7:
				cdv(operand);
				break;
			default:
				throw 0;
			}

			InstructionPointer += 2;
		}
	}
	catch (int e)
	{
		if (!e)
			std::cerr << '\n' << "Invalid program!" << '\n';
		return 1;
	}

	return 0;
}

static horizontal_vector<int> Output;

// instruction 5
void out2(int operand)
{
	Output.push_back(static_cast<int>(comboOp(operand)));
}

// instruction 6 (unused)
/*void bdv2(int operand)
{
	B = (A >> comboOp(operand)) & 0b111;
}*/

// instruction 7
void cdv2(int operand)
{
	C = (A >> comboOp(operand)) & 0b111;
}

int ExecuteProgram(const horizontal_vector<int>& program)
{
	try
	{
		Output.clear();
		B = 0;
		C = 0;
		InstructionPointer = 0;
		while (InstructionPointer < program.size() - 1)
		{
			int operand = program[InstructionPointer + 1];
			switch (program[InstructionPointer])
			{
			case 0:
				adv(operand);
				break;
			case 1:
				bxl(operand);
				break;
			case 2:
				bst(operand);
				break;
			case 3:
				jnz(operand);
				break;
			case 4:
				bxc(operand);
				break;
			case 5:
				out2(operand);
				break;
			case 7:
				cdv2(operand);
				break;
			default:
				throw 0;
			}

			InstructionPointer += 2;
		}
	}
	catch (int e)
	{
		if (!e)
			std::cerr << '\n' << "Invalid program!" << '\n';
		return 1;
	}
	return 0;
}

bool MatchOutput(const horizontal_vector<int>& program)
{
	size_t offset = program.size() - Output.size();
	for (unsigned int i = 0; i < Output.size(); ++i)
	{
		if (Output[i] != program[i + offset])
			return false;
	}

	return true;
}

bool TryNumbers(const horizontal_vector<int>& program, u64& currentResult, int executionDepth)
{
	if (executionDepth == program.size()) // We've added more numbers than we should need to add to produce a valid output
		return false;

	for (unsigned int i = 0; i < 8; ++i)
	{
		currentResult += i;
		A = currentResult;
		if (ExecuteProgram(program)) // ExecuteProgram returns a non-0 int if the program is invalid
			return false;
		if (MatchOutput(program))
		{
			if (Output.size() == program.size())
			{
				std::cout << Output << std::endl;
				std::cout << currentResult << std::endl;
				return true;
			}
			else
			{
				currentResult = currentResult << 3;
				if (TryNumbers(program, currentResult, executionDepth + 1))
					return true;
				else
					currentResult = currentResult >> 3;
			}
		}
		currentResult -= i;
	}

	return false;
}

int Day17::Solution2ver1()
{
	horizontal_vector<int> program = ParseInput();
	Output = horizontal_vector<int>();

	A = 0;
	unsigned long long resVal = 0;
	TryNumbers(program, resVal, 0);

	return 0;
}
