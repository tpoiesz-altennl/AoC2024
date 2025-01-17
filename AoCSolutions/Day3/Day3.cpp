#include "Day3.h"
#include "../Util.h"

#include <string>
#include <regex>
#include <iostream>
#include <sstream>

bool MatchMulSyntax(const std::string& input, int& operand1, int& operand2)
{
    std::regex mulSyntax("mul\\(\\d+,\\d+\\)");
    std::smatch m;
    if (std::regex_search(input, m, mulSyntax))
    {
        std::regex digit("\\d+");
        std::regex_search(input, m, digit);
        operand1 = std::stoi(m[0].str());
        std::string otherHalf = m.suffix().str();
        std::regex_search(otherHalf, m, digit);
        operand2 = std::stoi(m[0].str());
        return true;
    }

    return false;
}

int Day3::Solution1()
{
    std::vector<std::string> inputStrings = FileUtil::ReadInputIntoVec<std::string>(__FILE__);
    //std::vector<std::string> inputStrings = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);

    std::vector<std::string> mulStrings;
    for (std::string str : inputStrings)
    {
        std::string sub = "mul(";
        size_t pos = str.find(sub, 0);
        while (pos != std::string::npos)
        {
            // A "mul(x,y)" substring is max 12 characters long (because x and y are both at most 3 digits)
            mulStrings.push_back(str.substr(pos, 12));
            pos = str.find(sub, pos + 1);
        }
    }

    Testing::DebugFile dbg(__FILE__);
    int a = 0, b = 0;
    dbg.OutputMatches<std::string, int&, int&>(mulStrings, &MatchMulSyntax, false, a, b);
    int total = 0;
    for (std::string mulInstruction : mulStrings)
    {
        int a = 0, b = 0;
        if (MatchMulSyntax(mulInstruction, a, b))
            total += a * b;
    }

    std::cout << total;

    return 0;
}

int Day3::Solution2()
{
    std::vector<std::string> inputStrings = FileUtil::ReadInputIntoVec<std::string>(__FILE__);
    //std::vector<std::string> inputStrings = FileUtil::ReadInputIntoVec<std::string>(__FILE__, true);

    std::vector<std::string> instructions;
    bool shouldExecute = true;
    for (std::string str : inputStrings)
    {
        std::string subMul = "mul(";
        size_t posMul = str.find(subMul, 0);
        std::string subDo = "do()";
        size_t posDo = str.find(subDo, 0);
        std::string subDont = "don't()";
        size_t posDont = str.find(subDont, 0);
        while (posMul != std::string::npos || posDo != std::string::npos || posDont != std::string::npos)
        {
            if (posMul < posDo && posMul < posDont)
            {
                // A "mul(x,y)" substring is max 12 characters long (because x and y are both at most 3 digits)
                if (shouldExecute)
                    instructions.push_back(str.substr(posMul, 12));
                posMul = str.find(subMul, posMul + 1);
            }
            else if (posDo < posDont)
            {
                shouldExecute = true;
                posDo = str.find(subDo, posDo + 1);
            }
            else
            {
                shouldExecute = false;
                posDont = str.find(subDont, posDont + 1);
            }
        }
    }

    /*FileUtil::DebugFile dbg(__FILE__);
    int a = 0, b = 0;
    dbg.OutputMatches<std::string, int&, int&>(instructions, &MatchMulSyntax, false, a, b);*/

    int total = 0;
    for (std::string mulInstruction : instructions)
    {
        int a = 0, b = 0;
        if (MatchMulSyntax(mulInstruction, a, b))
            total += a * b;
    }

    std::cout << total;

    return 0;
}
