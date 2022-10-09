/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

using Bigint = Utils::bigint;

constexpr size_t N = 14;
using Pair = pair<int, int>;

array<Pair, N> link_positions(int pos, auto const& cmds)
{
    array<Pair, N> rules;
    stack<Pair> acc;
    for(auto [no, v2, v3]: cmds)
    {
        if(no == 1)
            acc.push({pos, v3});
        else
        {
            auto [pos0, v30] = acc.top();
            rules[pos] = make_pair(pos0, v30 + v2);
            rules[pos0] = make_pair(pos, -v30 - v2);
            acc.pop();
        }
        pos++;
    }
    return rules;
}

Bigint gen_minmax(auto choice)
{
    static vector<tuple<int, int, int>> program {
        {1, 10, 0},
        {1, 12, 6},
        {1, 13, 4},
        {1, 13, 2},
        {1, 14, 9},
        {26, -2, 1},
        {1, 11, 10},
        {26, -15, 6},
        {26, -10, 4},
        {1, 10, 6},
        {26, -10, 3},
        {26, -4, 9},
        {26, -1, 15},
        {26, -1, 5}
    };

    auto rules = link_positions(0, program);
    vector<int8_t> l(14, -1);
    for(size_t pos = 13, iter = 1; iter < 8; pos--)
    {
        if(l[pos] == -1)
        {
            auto [linked_pos, add] = rules[pos];
            auto pos_val = choice(-add);
            l[pos] = pos_val;
            l[linked_pos] = pos_val - add;
            iter++;
        }
    }
    return accumulate(cbegin(l), cend(l), 0ULL, [](Bigint acc, int i){return acc * 10 + i;});
}

//******************************************************************************

Bigint day24_p1(const vector<string>& l)
{
    return gen_minmax([](int v){return v < 0 ? 9 : 9 - v;});
}

Bigint day24_p2(const vector<string>& l)
{
    return gen_minmax([](int v){return v > 0 ? 1 : 1 - v;});
}

}

void aoc2021::day24()
{
    auto input = Utils{"data/day24/input.txt"}.read_lines();
    assert(day24_p1(input) == 94992994195998);
    assert(day24_p2(input) == 21191861151161);
}

//******************************************************************************
