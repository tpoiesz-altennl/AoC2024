#include <iostream>

#include "Day1/Day1.h"
#include "Day2/Day2.h"
#include "Day3/Day3.h"
#include "Util.h"

#include <chrono>

int main()
{
    int iterations = 1000;
    Testing::TimeSolution(&Day2::Solution2ver1, iterations);
    Testing::TimeSolution(&Day2::Solution2ver2, iterations);
    return 0;
}
