/*
 *
 */

#include "utils.h"

using aoc2021::Utils;

namespace
{

int day01_p1(const vector<int>& ints)
{
    return ints.size() < 2 ? 0 : get<0>(
        accumulate(cbegin(ints), cend(ints), make_tuple(0, ints[0]),
            [](tuple<int, int> acc, int elem) {
                return make_tuple(get<0>(acc) + (elem > get<1>(acc)? 1 : 0), elem);
            })
    );
}

int day01_p2(const vector<int>& ints, int window_size = 3)
{
    if(ints.size() < 4)
        return 0;
    int cnt = 0, s = accumulate(cbegin(ints), cbegin(ints) + window_size, 0), s2;
    for(size_t i = 1; i < ints.size() - window_size + 1; i++, s = s2)
    {
        s2 = s - ints[i - 1] + ints[i + window_size - 1];
        if(s2 > s)
            ++cnt;
    }
    return cnt;
}

}

void aoc2021::day01()
{
    vector<int> ints = Utils{"data/day01/input.txt"}.read_ints();
    assert(day01_p1(ints) == 1759);
    assert(day01_p2(ints) == 1805);
}

//******************************************************************************

