/*
 *
 */

#include <string_view>

#include "utils.h"

using namespace aoc2021;

namespace
{

class World
{
    set<pair<int, int>> m_points;
    vector<pair<bool, int>> m_commands;
    int m_x = 0, m_y = 0;

    void exec(bool is_x, int ln);
public:
    World(const vector<string>& l);
    size_t size() const;
    void fold();
    void one_fold();
    void print() const;
};

World::World(const vector<string>& l)
{
    size_t i = 0;
    int x, y;
    while(i < l.size())
    {
        const string& s = l[i++];
        if(s.empty())
            break;
        auto pos = s.find(",");
        m_points.insert(make_pair(x = stoi(s.substr(0, pos)),
                                  y = stoi(s.substr(pos + 1))));
        m_x = max(m_x, x);
        m_y = max(m_y, y);
    }
    while(i < l.size())
    {
        const string& s = l[i++];
        int no = stoi(s.substr(13));
        m_commands.push_back(make_pair(s[11] == 'x', no));
    }
}

size_t World::size() const
{
    return m_points.size();
}

void World::print() const
{
    for(int y = 0; y <= m_y; y++)
    {
        for(int x = 0; x <= m_x; x++)
            cout << (m_points.contains(make_pair(x, y)) ? "#" : " ");
        cout << endl;
    }
}

void World::fold()
{
    for(auto [is_x, ln]: m_commands)
        exec(is_x, ln);
}

void World::one_fold()
{
    for(auto [is_x, ln]: m_commands)
    {
        exec(is_x, ln);
        break;
    }
}

void World::exec(bool is_x, int ln)
{
    m_x = 0;
    m_y = 0;

    set<pair<int, int>> points;
    int x, y;
    for(auto it = m_points.begin(); it != m_points.end(); it++)
    {
        if(is_x && it->first >= ln)
        {
            x = ln - (it->first - ln);
            y = it->second;
        }
        else if(!is_x && it->second >= ln)
        {
            x = it->first;
            y = ln - (it->second - ln);
        }
        else
        {
            x = it->first;
            y = it->second;
        }
        points.insert(make_pair(x, y));
        m_x = max(m_x, x);
        m_y = max(m_y, y);
    }

    m_points = move(points);
}

//******************************************************************************

int day13_p1(const vector<string>& l)
{
    World w{l};
    w.one_fold();
    return w.size();
}

int day13_p2(const vector<string>& l, bool verbose = false)
{
    World w{l};
    w.fold();
    if(verbose)
        w.print();
    return w.size();
}

}

void aoc2021::day13()
{
#if DEV
    auto sample = Utils{"data/day13/sample.txt"}.read_lines();
    assert(day13_p1(sample) == 17);
    assert(day13_p2(sample) == 16);
#endif

    auto input = Utils{"data/day13/input.txt"}.read_lines();
    assert(day13_p1(input) == 788);
    assert(day13_p2(input) == 102);
}

//******************************************************************************
