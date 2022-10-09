/*
 *
 */

#include "utils.h"

using aoc2021::Utils;

namespace
{

using nav_t = Utils::nav_t;

int day02_p1(const vector<nav_t>& cmds)
{
    int hor = 0, depth = 0;
    for(auto [cmd, param]: cmds)
    {
        switch(cmd)
        {
            case 1:
                hor += param;
                break;
            case 2:
                depth += param;
                break;
            case 3:
                depth -= param;
                break;
        }
    }
    return hor * depth;
}

int day02_p2(const vector<nav_t>& cmds)
{
    int hor = 0, depth = 0, aim = 0;
    for(auto [cmd, param]: cmds)
    {
        switch(cmd)
        {
            case 1:
                hor += param;
                depth += aim * param;
                break;
            case 2:
                aim += param;
                break;
            case 3:
                aim -= param;
                break;
        }
    }
    return hor * depth;
}

}

void aoc2021::day02()
{
    auto cmds = Utils{"data/day02/input.txt"}.read_nav_cmds();
    assert(day02_p1(cmds) == 2322630);
    assert(day02_p2(cmds) == 2105273490);
}

//******************************************************************************
