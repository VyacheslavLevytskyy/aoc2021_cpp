/*
 *
 */

#include "utils.h"

using aoc2021::Utils;

namespace
{

unsigned long long do_spawn(const vector<int8_t>& ints, unsigned steps)
{
    array<unsigned long long, 9> pool{};
    for(int8_t k: ints)
        pool[k]++;

    unsigned long long pro;
    for(unsigned step = 0; step < steps; step++)
    {
        pro = pool[0];
        for(unsigned i = 0; i < 8; i++)
            pool[i] = pool[i + 1];
        pool[6] += pool[8] = pro;
    }
    return accumulate(cbegin(pool), cend(pool), 0ULL);
}

unsigned long long day06_p1(const vector<int8_t>& ints)
{
    return do_spawn(ints, 80);
}

unsigned long long day06_p2(const vector<int8_t>& ints)
{
    return do_spawn(ints, 256);
}

}

void aoc2021::day06()
{
#if DEV
    vector<string> sample_lines = Utils{"data/day06/sample.txt"}.read_lines();
    auto sample = Utils::read_int_line<int8_t>(sample_lines[0]);
    assert(day06_p1(sample) == 5934ULL);
    assert(day06_p2(sample) == 26984457539ULL);
#endif

    vector<string> input_lines = Utils{"data/day06/input.txt"}.read_lines();
    auto input = Utils::read_int_line<int8_t>(input_lines[0]);
    assert(day06_p1(input) == 390011ULL);
    assert(day06_p2(input) == 1746710169834ULL);
}

//******************************************************************************
