/*
 *
 */

#include "utils.h"

using aoc2021::Utils;

namespace
{

using Display = Utils::display;

unsigned digit(unsigned bitmask)
{
    switch(bitmask)
    {
        case 36:
            return 1U;
        case 37:
            return 7U;
        case 46:
            return 4U;

        case 93:
            return 2U;
        case 109:
            return 3U;
        case 107:
            return 5U;

        case 119:
            return 0U;
        case 123:
            return 6U;
        case 111:
            return 9U;

        case 127:
            return 8U;

        default:
            throw invalid_argument("no such digit encoding");
    }
}

unsigned get_bit(char ch)
{
    switch(ch)
    {
        case 'a':
            return 0b1;
        case 'b':
            return 0b10;
        case 'c':
            return 0b100;
        case 'd':
            return 0b1000;
        case 'e':
            return 0b10000;
        case 'f':
            return 0b100000;
        case 'g':
            return 0b1000000;
        default:
            throw invalid_argument("no such character in the digit encoding");
    }
}

unsigned n_bits(unsigned v)
{
    unsigned s = 0;
    while(v > 0)
    {
        s += v & 1;
        v >>= 1;
    }
    return s;
}

map<unsigned, unsigned> wire(array<string, 10>& in)
{
    sort(begin(in), end(in), [](const string& a, const string& b){
        return a.size() < b.size();
    });
    array<unsigned, 10> in_bin;
    transform(cbegin(in), cend(in), begin(in_bin), [](auto const& str){
        unsigned mask = 0;
        for(auto ch: str)
            mask |= get_bit(ch);
        return mask;
    });

    auto [v1, v7, v4, x5, y5, z5, x6, y6, z6, v8] = in_bin;
    unsigned p0 = v7 ^ v1;
    unsigned p13 = v4 ^ v1;
    array<unsigned, 3> xy{x5 ^ y5, y5 ^ z5, x5 ^ z5};
    sort(begin(xy), end(xy), [](unsigned a, unsigned b){
        return n_bits(a) < n_bits(b);
    });
    auto [x1, x2, p1245] = xy;
    unsigned p12, p45;
    if((p13 & x1) == 0)
    {
        p12 = x2;
        p45 = x1;
    }
    else
    {
        p12 = x1;
        p45 = x2;
    }
    unsigned p1 = p13 & p12;
    unsigned p2 = p12 ^ p1;
    unsigned p3 = p13 ^ p1;
    unsigned p23 = p2 | p3;
    unsigned y1 = x6 ^ y6;
    unsigned y2 = y6 ^ z6;
    unsigned y3 = x6 ^ z6;
    unsigned p4;
    if(p23 == y1)
        p4 = y2 & y3;
    else if(p23 == y2)
        p4 = y1 & y3;
    else if(p23 == y3)
        p4 = y1 & y2;
    else
        throw invalid_argument("unexpected display encoding");
    unsigned p5 = p45 ^ p4;
    unsigned p6 = v8 ^ (p0 | p3 | p1245);
    return map<unsigned, unsigned>{
        {p0, 0b1},
        {p1, 0b10},
        {p2, 0b100},
        {p3, 0b1000},
        {p4, 0b10000},
        {p5, 0b100000},
        {p6, 0b1000000}
    };
}

array<unsigned, 4> decode(auto const& out, map<unsigned, unsigned> wire)
{
    array<unsigned, 4> out_bin;
    size_t i = 0;
    unsigned num;
    for(auto const& str: out)
    {
        size_t len = str.size();
        switch(len)
        {
            case 2:
                num = 1;
                break;
            case 3:
                num = 7;
                break;
            case 4:
                num = 4;
                break;
            case 7:
                num = 8;
                break;
            default:
                unsigned mask = 0;
                for(auto ch: str)
                    mask |= wire[get_bit(ch)];
                num = digit(mask);
        }
        out_bin[i++] = num;
    }
    return out_bin;
}

unsigned day08_p1(const vector<Display>& displays)
{
    unsigned acc = 0;
    for(auto line: displays)
        for(auto digit: line.second)
        {
            size_t len = digit.size();
            if(len == 2 || len == 3 || len == 4 || len == 7)
                acc++;
        }
    return acc;
}

unsigned day08_p2(vector<Display> displays)
{
    return accumulate(begin(displays), end(displays), 0U, [](unsigned acc, Display& elem){
        auto const& [a, b, c, d] = decode(elem.second, wire(elem.first));
        return acc + (d + 10*(c + 10*(b + 10*a)));
    });
}

}

void aoc2021::day08()
{
#if DEV
    auto sample = Utils{"data/day08/sample.txt"}.read_displays();
    assert(day08_p1(sample) == 26);
    assert(day08_p2(sample) == 61229);
#endif

    auto input = Utils{"data/day08/input.txt"}.read_displays();
    assert(day08_p1(input) == 493);
    assert(day08_p2(input) == 1010460);
}

//******************************************************************************
