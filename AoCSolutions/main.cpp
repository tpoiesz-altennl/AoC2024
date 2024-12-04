#include <iostream>

#include "Day1/Day1.h"
#include "Day2/Day2.h"
#include "Day3/Day3.h"
#include "Day4/Day4.h"
#include "Util.h"

#include <chrono>

int main()
{
    unsigned long long iter = 10;
    Testing::TimeSolution(&Day4::Solution1ver1, iter);
    Testing::TimeSolution(&Day4::Solution1ver2, iter);
    return 0;
}
