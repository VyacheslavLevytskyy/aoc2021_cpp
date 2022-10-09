/*
 *
 */

#include <iomanip>

#include "utils.h"

using namespace aoc2021;

int main(int argc, const char *argv[])
{
    auto days = {
        day01, day02, day03, day04, day05,
        day06, day07, day08, day09, day10,
        day11, day12, day13, day14, day15,
        day16, day17, day18, day19, day20,
        day21, day22, day23, day24, day25
    };

    cout << days.size() << " C++ days:" << endl;
    vector<chrono::time_point<chrono::steady_clock>> ts{1, chrono::steady_clock::now()};
    for(auto day: days)
    {
        day();
        ts.push_back(chrono::steady_clock::now());
    }

    cout << "Time elapsed:" << endl;
    ios cout_state(nullptr);
    cout_state.copyfmt(cout);
    cout << std::setprecision(1) << std::fixed;
    chrono::duration<double, milli>::rep s = 0;
    for(size_t i = 1; i < ts.size(); i++)
    {
        chrono::duration<double, milli> ms = ts[i] - ts[i - 1];
        s += ms.count();
        cout << "    day " << i << ": " << ms.count() << "ms" << endl;
    }
    cout << "Average: " << (s / days.size()) << "ms" << endl;
    cout.copyfmt(cout_state);

    return 0;
}

//******************************************************************************
