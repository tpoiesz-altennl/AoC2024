#include <iostream>

#include "Day1/Day1.h"
#include "Day2/Day2.h"
#include "Day3/Day3.h"
#include "Day4/Day4.h"
#include "Day5/Day5.h"
#include "Day6/Day6.h"
#include "Day7/Day7.h"
#include "Day8/Day8.h"
#include "Day9/Day9.h"
#include "Day10/Day10.h"
#include "Day11/Day11.h"
#include "Day12/Day12.h"
#include "Day13/Day13.h"
#include "Day14/Day14.h"
#include "Day15/Day15.h"
#include "Day16/Day16.h"
#include "Util.h"

#include <chrono>

int main()
{
    u64 iter = 1000;
    //Testing::TimeSolution(&Day16::Solution1ver2, iter);
    //Testing::TimeSolution(&Day16::Solution1ver3, iter);
    return Day16::Solution2();
    return 0;
}
