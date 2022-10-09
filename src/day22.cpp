/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

using num_score = Utils::bigint;
using Cube = Utils::cube;

//******************************************************************************

bool is_empty(const Cube& s)
{
    return s[0] > s[1] || s[2] > s[3] || s[4] > s[5];
}

num_score sz(const Cube& s)
{
    return (s[1] - s[0] + 1) * (s[3] - s[2] + 1) * (s[5] - s[4] + 1);
}

Cube intersect(const Cube& p, const Cube& q)
{
    Cube s;
    for(size_t i = 0; i < s.size(); i += 2)
    {
        s[i] = max(p[i], q[i]);
        s[i + 1] = min(p[i + 1], q[i + 1]);
    }
    return s;
}

num_score reboot(const vector<pair<int, Cube>>& cmds)
{
    list<pair<int, Cube>> cubes;
    num_score total = 0;
    for(auto [onoff, p]: cmds)
    {
        list<pair<int, Cube>> new_cubes;
        if(onoff == 1)
        {
            new_cubes.push_front(make_pair(1, p));
            total += sz(p);
        }
        for(auto const& [sign, q]: cubes)
        {
            Cube r = intersect(p, q);
            if(!is_empty(r))
            {
                new_cubes.push_front(make_pair(-sign, r));
                if(sign == 1)
                    total -= sz(r);
                else
                    total += sz(r);
            }
        }
        cubes.splice(cubes.begin(), new_cubes);
    }
    return total;
}

//******************************************************************************

num_score day22_p1(const vector<pair<int, Cube>>& cmds0)
{
    static const Cube center{-50, 50, -50, 50, -50, 50};
    vector<pair<int, Cube>> cmds;
    cmds.reserve(cmds0.size());
    for(auto [onoff, r0]: cmds0)
    {
        Cube r = intersect(center, r0);
        if(!is_empty(r))
            cmds.push_back(make_pair(onoff, r));
    }
    return reboot(cmds);
}

num_score day22_p2(const vector<pair<int, Cube>>& cmds)
{
    return reboot(cmds);
}

}

void aoc2021::day22()
{
#if DEV
    auto sample = Utils{"data/day22/sample3.txt"}.read_cubes();
    assert(day22_p1(sample) == 474140ULL);
    assert(day22_p2(sample) == 2758514936282235ULL);
#endif

    auto input = Utils{"data/day22/input.txt"}.read_cubes();
    assert(day22_p1(input) == 580098ULL);
    assert(day22_p2(input) == 1134725012490723ULL);
}

//******************************************************************************
