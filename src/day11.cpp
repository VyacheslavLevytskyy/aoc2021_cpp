/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

class World
{
    constexpr static int EXPLODE = 10;
    constexpr static int TOTAL_SIZE = EXPLODE * EXPLODE;

    using Coord = pair<int, int>;
    using Storage = array<vector<Coord>, EXPLODE + 1>;

    Storage m_levels;

    void inc_energy();
    static void reserve(Storage& storage);

public:
    World(const vector<string>& l);
    int explode();
    bool is_zeroed() const;
    [[maybe_unused]] vector<string> present() const;
};

World::World(const vector<string>& l)
{
    int r = 0;
    for(auto const& s: l)
    {
        int c = 0;
        for(char ch: s)
            m_levels[ch - '0'].push_back(make_pair(r, c++));
        r++;
    }
}

vector<string> World::present() const
{
    vector<string> l(10, string(EXPLODE, ' '));
    for(int v = 0; v < EXPLODE; v++)
    {
        for(auto [r, c]: m_levels[v])
            l[r][c] = v + '0';
    }
    return l;
}

bool World::is_zeroed() const
{
    return m_levels[0].size() == TOTAL_SIZE;
}

void World::inc_energy()
{
    for(size_t i = EXPLODE; i > 0; i--)
        m_levels[i] = move(m_levels[i - 1]);
    m_levels[0] = vector<Coord>{};
}

int World::explode()
{
    inc_energy();
    int cb = 0;
    while(m_levels[EXPLODE].size() > 0)
    {
        vector<Coord> to_explode = move(m_levels[EXPLODE]);
        m_levels[EXPLODE].clear();
        for(auto focus: to_explode)
        {
            Storage levels;
            int i = 0, dr, dc, j;
            for(auto coords: m_levels)
            {
                for(auto coord: coords)
                {
                    dr = abs(coord.first - focus.first);
                    dc = abs(coord.second - focus.second);
                    j = (i > 0 && i < EXPLODE && dr < 2 && dc < 2) ? i + 1 : i;
                    levels[j].push_back(coord);
                }
                i++;
            }
            levels[0].push_back(focus);
            m_levels = move(levels);
            cb++;
        }
    }
    return cb;
}

//******************************************************************************

int day11_p1(const vector<string>& l, bool verbose = false)
{
    World w{l};
    int acc = 0;
    if(verbose)
        cout << w.present() << endl << endl;
    for(int i = 0; i < 100; i++)
    {
        acc += w.explode();
        if(verbose)
            cout << w.present() << endl << endl;
    }
    return acc;
}

int day11_p2(const vector<string>& l)
{
    World w{l};
    int acc = 0;
    for(; !w.is_zeroed(); acc++)
        w.explode();
    return acc;
}

}

void aoc2021::day11()
{
#if DEV
    auto sample = Utils{"data/day11/sample.txt"}.read_lines();
    assert(day11_p1(sample) == 1656);
    assert(day11_p2(sample) == 195);
#endif

    auto input = Utils{"data/day11/input.txt"}.read_lines();
    assert(day11_p1(input) == 1627);
    assert(day11_p2(input) == 329);
}

//******************************************************************************
