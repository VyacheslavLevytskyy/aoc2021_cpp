/*
 *
 */

#include "utils.h"

using aoc2021::Utils;

namespace
{

unsigned most_common(const vector<string>& lines, unsigned sz, size_t pos, unsigned on_draw = 0)
{
    unsigned n = 0, lim = sz / 2;
    for(size_t i = 0; i < sz; i++)
    {
        if(lines[i][pos] == '1' and ++n > lim)
            return 1;
    }
    return (2 * n == sz) ? on_draw : 0;
}

unsigned day03_p1(const vector<string>& lines)
{
    unsigned sz = lines.size();
    unsigned n = lines[0].size(), most = 0;
    for(size_t pos = 0; pos < n; pos++)
        most = (most << 1) + most_common(lines, sz, pos);
    return most * (((1 << n) - 1) ^ most);
}

unsigned filter(vector<string> lines, bool keep_most)
{
    auto it1 = begin(lines), it2 = end(lines);
    unsigned sz = it2 - it1, n = lines[0].size(), bit;
    for(size_t pos = 0; pos < n; pos++)
    {
        bit = most_common(lines, sz, pos, 1);
        it2 = remove_if(it1, it2, [pos, keep_most, bit](const string& s) {
            return (static_cast<unsigned>(s[pos] - '0') == bit) != keep_most;
        });
        sz = it2 - it1;
        if(sz == 1)
            return static_cast<unsigned>(stoul(*it1, nullptr, 2));
    }
    throw invalid_argument("unexpected input");
}

unsigned day03_p2(const vector<string>& lines)
{
    unsigned a = filter(lines, true);
    unsigned b = filter(lines, false);
    return a * b;
}

}

void aoc2021::day03()
{
#if DEV
    assert(day03_p2(Utils{"data/day03/sample.txt"}.read_lines()) == 230);
#endif
    auto lines = Utils{"data/day03/input.txt"}.read_lines();
    assert(day03_p1(lines) == 3912944);
    assert(day03_p2(lines) == 4996233);
}

//******************************************************************************
