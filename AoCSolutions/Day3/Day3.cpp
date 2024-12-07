#include "Day3.h"
#include "../Util.h"

#include <string>
#include <regex>
#include <iostream>
#include <sstream>

bool MatchMulSyntax(const std::string& input, int& operand1, int& operand2, bool isRecursion = false)
{
    if ((!isRecursion && input.substr(0, 4) != "mul(") || (isRecursion && input[0] != ','))
        return false;

    std::smatch m3;
    std::regex threeDigit("([0-9][0-9][0-9])");
    bool threeDigitMatch = std::regex_search(input, m3, threeDigit);
    std::regex_match(input, m3, threeDigit);

    std::smatch m2;
    std::regex twoDigit("([0-9][0-9])");
    bool twoDigitMatch = std::regex_search(input, m2, twoDigit);

    std::smatch m1;
    std::regex oneDigit("([0-9])");
    bool oneDigitMatch = std::regex_search(input, m1, oneDigit);
    
    if (threeDigitMatch)
    {
        // If second mul number is three digits
        if (m3.prefix().str().back() == ',' && m3.suffix().length() && m3.suffix().str().front() == ')')
        {
            if (isRecursion) // Number before , already processed
            {
                operand2 = std::stoi(m3[0].str());
                return true;
            }
            else if (twoDigitMatch && m2.prefix().str() == "mul(")
            { // Two digit number before ,
                operand1 = std::stoi(m2[0].str());
                operand2 = std::stoi(m3[0].str());
                return true;
            }
            else if (oneDigitMatch && m1.prefix().str() == "mul(")
            { // One digit number before ,
                operand1 = std::stoi(m1[0].str());
                operand2 = std::stoi(m3[0].str());
                return true;
            }
            else // No number before ,
                return false;
        }
        // If first number of mul is three digits, we have to recurse to find second number of mul
        else if (m3.suffix().length() && m3.suffix().str().front() == ',')
        {
            operand1 = std::stoi(m3[0].str());
            return MatchMulSyntax(m3.suffix().str(), operand1, operand2, true);
        }
    }
    else if (twoDigitMatch)
    {
        // If second mul number is two digits
        if (m2.prefix().str().back() == ',' && m2.suffix().length() && m2.suffix().str().front() == ')')
        {
            if (isRecursion) // Number before , already processed
            {
                operand2 = std::stoi(m2[0].str());
                return true;
            }
            else if (oneDigitMatch && m1.prefix().str() == "mul(")
            { // One digit number before ,
                operand1 = std::stoi(m1[0].str());
                operand2 = std::stoi(m2[0].str());
                return true;
            }
            else // No number before ,
                return false;
        }
        // If first number of mul is two digits and second number of mul is two or fewer digits, we have to recurse to find it
        else if (m2.suffix().length() && m2.suffix().str().front() == ',')
        {
            operand1 = std::stoi(m2[0].str());
            return MatchMulSyntax(m2.suffix().str(), operand1, operand2, true);
        }
    }
    else if (oneDigitMatch)
    {
        // First number of mul is one digit; recurse to find second one-digit number (if any)
        if (m1.suffix().length() && m1.suffix().str().front() == ',')
        {
            operand1 = std::stoi(m1[0].str());
            return MatchMulSyntax(m1.suffix().str(), operand1, operand2, true);
        }
        // Second number of mul is one digit (found by recursion)
        else if (isRecursion && m1.suffix().length() && m1.suffix().str().front() == ')')
        {
            operand2 = std::stoi(m1[0].str());
            return true;
        }
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

    /*Testing::DebugFile dbg(__FILE__);
    int a = 0, b = 0;
    dbg.OutputMatches<std::string, int&, int&, bool>(mulStrings, &MatchMulSyntax, false, a, b, false);*/
    int total = 0;
    for (std::string funny : mulStrings)
    {
        int a = 0, b = 0;
        if (MatchMulSyntax(funny, a, b))
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
    dbg.OutputMatches<std::string, int&, int&, bool>(instructions, &MatchMulSyntax, false, a, b, false);*/

    int total = 0;
    for (std::string funny : instructions)
    {
        int a = 0, b = 0;
        if (MatchMulSyntax(funny, a, b))
            total += a * b;
    }

    std::cout << total;

    return 0;
}
