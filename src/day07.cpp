/*
 *
 */

#include "utils.h"

using aoc2021::Utils;

namespace
{

int day07_p1(const vector<int>& sorted_ints)
{
    int pos = sorted_ints.at(sorted_ints.size() >> 1);
    return accumulate(cbegin(sorted_ints), cend(sorted_ints), 0, [pos](int acc, int x){
        return acc + abs(x - pos);
    });
}

int fuel(const vector<int>& vec, int pos)
{
    return accumulate(cbegin(vec), cend(vec), 0, [pos](int acc, int x){
        return acc + abs(x - pos) * (1 + abs(x - pos));
    });
}

int day07_p2(const vector<int>& sorted_ints)
{
    int pos1 = sorted_ints[0], w1 = fuel(sorted_ints, pos1), w2;
    for(int pos2: views::iota(pos1 + 1, sorted_ints[sorted_ints.size() - 1] + 1))
    {
        w2 = fuel(sorted_ints, pos2);
        if(w2 < w1)
        {
            pos1 = pos2;
            w1 = w2;
        }
        else
            break;
    }
    return w1 >> 1;
}

}

void aoc2021::day07()
{
#if DEV
    vector<string> sample_lines = Utils{"data/day07/sample.txt"}.read_lines();
    auto sample = Utils::read_int_line<int>(sample_lines[0]);
    assert(sample.size() > 0);
    sort(sample.begin(), sample.end());
    assert(day07_p1(sample) == 37);
    assert(day07_p2(sample) == 168);
#endif

    vector<string> input_lines = Utils{"data/day07/input.txt"}.read_lines();
    auto input = Utils::read_int_line<int>(input_lines[0]);
    assert(input.size() > 0);
    sort(input.begin(), input.end());
    assert(day07_p1(input) == 337488);
    assert(day07_p2(input) == 89647695);
}

//******************************************************************************
