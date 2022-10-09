/*
 *
 */

#include "utils.h"

using aoc2021::Utils;

namespace
{

using coord = Utils::coord;

constexpr int16_t MIN = 0, MAX = 1000;

unsigned day05_p2(const vector<coord>& coords)
{
    /*
    Longer version without size limitations using sets:
    *************************************************
    map<pair<int16_t, int16_t>, int8_t> field{};
    unsigned overlap = 0;
    auto check_overlap = [&field, &overlap](int16_t x, int16_t y) {
        auto xy = make_pair(x, y);
        auto it = field.find(xy);
        if(it == field.end())
            field[xy] = 0;
        else if(it->second == 0)
        {
            field[xy] = 1;
            overlap++;
            //cout << xy.first << ", " << xy.second << " -> " << overlap << endl;
        }
    };
    */

    int8_t field[MAX][MAX]{};
    unsigned overlap = 0;
    auto check_overlap = [&field, &overlap](int16_t x, int16_t y) {
        int8_t& value = field[x][y];
        switch(value)
        {
            case 0:
                value = 1;
                break;
            case 1:
                value = 2;
                overlap++;
                break;
        }
    };
    for(auto r: coords)
    {
        for(auto pos: r)
            if(pos < MIN || pos > MAX)
                throw invalid_argument("coordinates are out of range");

        if(r[0] == r[2])
            for(int16_t y: views::iota(min(r[1], r[3]), max(r[1], r[3]) + 1))
                check_overlap(r[0], y);
        else if(r[1] == r[3])
            for(int16_t x: views::iota(min(r[0], r[2]), max(r[0], r[2]) + 1))
                check_overlap(x, r[1]);
        else
        {
            int16_t dx = r[0] <= r[2] ? 1 : -1;
            int16_t dy = r[1] <= r[3] ? 1 : -1;
            for(int16_t x = r[0], y = r[1]; x != r[2] + dx; x += dx, y += dy)
                check_overlap(abs(x), abs(y));
        }
    }
    return overlap;
}

unsigned day05_p1(vector<coord> coords)
{
    coords.erase(
        remove_if(begin(coords), end(coords), [](coord r) {
            return r[0] != r[2] && r[1] != r[3];
        }),
        end(coords)
    );
    return day05_p2(coords);
}

}

void aoc2021::day05()
{
#if DEV
    auto sample = Utils{"data/day05/sample.txt"}.read_coords();
    assert(day05_p1(sample) == 5);
    assert(day05_p2(sample) == 12);
#endif

    auto input = Utils{"data/day05/input.txt"}.read_coords();
    assert(day05_p1(input) == 5585);
    assert(day05_p2(input) == 17193);
}

//******************************************************************************
