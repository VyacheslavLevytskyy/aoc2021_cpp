/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{


class World
{
    using XY = pair<size_t, size_t>;

    size_t m_dx, m_dy;
    vector<vector<uint8_t>> m_data;

    void safe_add(vector<XY>& around, size_t x, size_t y);

public:
    World(const vector<string>& l);
    void add_tiles(size_t tdx, size_t tdy);
    unsigned find();
};

World::World(const vector<string>& l)
{
    if(l.size() < 1 || l[0].size() < 1)
        throw invalid_argument("unexpected task format");

    m_dx = l[0].size();
    m_dy = l.size();
    m_data.resize(m_dx, vector<uint8_t>(m_dy, 0));

    for(size_t x = 0; x < m_dx; x++)
        for(size_t y = 0; y < m_dy; y++)
            m_data[x][y] = l[y][x] - '0';
}

void World::add_tiles(size_t tdx, size_t tdy)
{
    size_t dx = m_dx, dy = m_dy;
    m_dx *= tdx;
    m_dy *= tdy;
    for(size_t x = 0; x < dx; x++)
        m_data[x].resize(m_dy, 0);
    m_data.resize(m_dx, vector<uint8_t>(m_dy, 0));

    for(size_t x = 0; x < dx; x++)
        for(size_t y = 0; y < dy; y++)
        {
            uint8_t v = m_data[x][y];
            for(size_t tx = 0; tx < tdx; tx++)
                for(size_t ty = 0; ty < tdy; ty++)
                {
                    if(tx || ty)
                        m_data[x + tx * dx][y + ty * dy] = (v + tx + ty - 1) % 9 + 1;
                }
        }
}

void World::safe_add(vector<XY>& around, size_t x, size_t y)
{
    if(x >= 0 && y >= 0 && x < m_dx && y < m_dy)
        around.push_back(make_pair(x, y));
}

unsigned World::find()
{
    XY start{0, 0}, maxkey{m_dx - 1, m_dy - 1};
    map<XY, unsigned> shortest;
    set<pair<unsigned, XY>> dist{{0, start}};
    map<XY, unsigned> costs{{start, 0}};

    for(;;)
    {
        auto it = dist.begin();
        auto [cost, key] = *it;
        if(key == maxkey)
            return cost;

        dist.erase(it);

        auto [x, y] = key;
        vector<XY> around;
        safe_add(around, x - 1, y);
        safe_add(around, x + 1, y);
        safe_add(around, x, y - 1);
        safe_add(around, x, y + 1);

        for(auto const& newkey: around)
            if(!shortest.contains(newkey))
            {
                unsigned updcost = cost + m_data[newkey.first][newkey.second];
                if(!costs.contains(newkey) || costs[newkey] > updcost)
                {
                    dist.insert(make_pair(updcost, newkey));
                    costs[newkey] = updcost;
                }
            }

        shortest[key] = cost;
    }
    return numeric_limits<unsigned>::max();
}

//******************************************************************************

int day15_p1(const vector<string>& l)
{
    return World{l}.find();
}

int day15_p2(const vector<string>& l)
{
    World w{l};
    w.add_tiles(5, 5);
    return w.find();
}

}

void aoc2021::day15()
{
#if DEV
    auto sample = Utils{"data/day15/sample.txt"}.read_lines();
    assert(day15_p1(sample) == 40);
    assert(day15_p2(sample) == 315);
#endif

    auto input = Utils{"data/day15/input.txt"}.read_lines();
    assert(day15_p1(input) == 390);
    assert(day15_p2(input) == 2814);
}

//******************************************************************************
