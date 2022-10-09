/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

unsigned moves(vector<string>& l)
{
    size_t dx = l[0].size(), dy = l.size(), x, y;
    vector<string> m(dy, string(dx + 1, '.'));
    char ch;
    for(unsigned cb = 0, step = 1;; cb = 0, step++)
    {
        for(x = 0; x < dx; x++)
            for(y = 0; y < dy; y++)
                if((ch = l[y][x]) == '>' && l[y][(x + 1) % dx] == '.')
                {
                    m[y][(x + 1) % dx] = ch;
                    cb++;
                }
                else if(ch != '.')
                    m[y][x] = ch;

        l = move(m);
        m.resize(dy, string(dx + 1, '.'));

        for(x = 0; x < dx; x++)
            for(y = 0; y < dy; y++)
                if((ch = l[y][x]) == 'v' && l[(y + 1) % dy][x] == '.')
                {
                    m[(y + 1) % dy][x] = ch;
                    cb++;
                }
                else if(ch != '.')
                    m[y][x] = ch;

        if(cb == 0)
            return step;

        l = move(m);
        m.resize(dy, string(dx + 1, '.'));
    }
}

//******************************************************************************

unsigned day25_p1(vector<string>& l)
{
    return moves(l);
}

}

void aoc2021::day25()
{
#if DEV
    auto sample = Utils{"data/day25/sample.txt"}.read_lines();
    assert(day25_p1(sample) == 58);
#endif

    auto input = Utils{"data/day25/input.txt"}.read_lines();
    assert(day25_p1(input) == 414);
}

//******************************************************************************
