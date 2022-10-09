/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

using Pts = tuple<char, size_t, size_t>;

class World
{
    const vector<string>& m_world;
    size_t m_rows, m_cols;

    char at(int r, int c) const;
    bool is_lowest(size_t r, size_t c) const;
    vector<Pts> higher(size_t r, size_t c) const;

public:
    World(const vector<string>& l) :
        m_world{l}, m_rows(l.size()), m_cols(l.size() > 0 ? l[0].size() : 0)
    {}
    vector<Pts> low_points() const;
    int eval_basin(const Pts& point) const;
};

char World::at(int r, int c) const
{
    return (r >= 0 && r < static_cast<int>(m_rows) &&
            c >= 0 && c < static_cast<int>(m_cols)) ? m_world[r][c] : '9';
}

bool World::is_lowest(size_t r, size_t c) const
{
    char self = at(r, c);
    return self < at(r - 1, c) && self < at(r + 1, c) &&
           self < at(r, c - 1) && self < at(r, c + 1);
}

vector<Pts> World::low_points() const
{
    vector<Pts> lst;
    for(size_t r = 0; r < m_world.size(); r++)
    {
        const string& str = m_world[r];
        for(size_t c = 0; c < str.size(); c++)
            if(is_lowest(r, c))
                lst.push_back(make_tuple(str[c], r, c));
    }
    return lst;
}

vector<Pts> World::higher(size_t r, size_t c) const
{
    vector<Pts> lst;
    pair<int, int> adjs[4] = {{r - 1, c}, {r + 1, c}, {r, c - 1}, {r, c + 1}};
    char self = at(r, c), neighbor;
    for(auto [dr, dc]: adjs)
    {
        neighbor = at(dr, dc);
        if(self < neighbor && neighbor < '9')
            lst.push_back(make_tuple(neighbor, dr, dc));
    }
    return lst;
}

int World::eval_basin(const Pts& point) const
{
    auto [x0, r0, c0] = point;
    map<pair<int, int>, char> check{{make_pair(r0, c0), x0}}, lock(check);
    while(check.size() > 0)
    {
        map<pair<int, int>, char> new_check;
        for(auto [coord, _]: check)
        {
            for(auto [hx, hr, hc]: higher(coord.first, coord.second))
            {
                pair<int, int> hpos = make_pair(hr, hc);
                if(!lock.contains(hpos))
                    new_check[hpos] = hx;
            }
        }
        check.clear();
        for(auto [new_pos, new_x]: new_check)
            if(new_x < '8')
                check[new_pos] = new_x;
        lock.merge(new_check);
    }
    return - lock.size();
}

//******************************************************************************

unsigned day09_p1(const vector<Pts>& lst)
{
    return accumulate(lst.cbegin(), lst.cend(), 0, [](unsigned acc, const Pts& point){
        return acc + get<0>(point) - '0' + 1;
    });
}

[[maybe_unused]]
unsigned day09_p10(const vector<string>& l)
{
    return day09_p1(World{l}.low_points());
}

unsigned day09_p2(const World& w, const vector<Pts>& lst)
{
    vector<int> marks;
    for(const Pts& point: lst)
        marks.push_back(w.eval_basin(point));
    sort(marks.begin(), marks.end());
    return marks.size() > 2 ? abs(marks[0] * marks[1] * marks[2]) : 0;
}

[[maybe_unused]]
unsigned day09_p20(const vector<string>& l)
{
    World w{l};
    vector<Pts> lst = w.low_points();
    return day09_p2(w, lst);
}

}

void aoc2021::day09()
{
#if DEV
    auto sample = Utils{"data/day09/sample.txt"}.read_lines();
    assert(day09_p10(sample) == 15);
    assert(day09_p20(sample) == 1134);
#endif

    auto input = Utils{"data/day09/input.txt"}.read_lines();
    World w{input};
    vector<Pts> lst = w.low_points();
    assert(day09_p1(lst) == 452);
    assert(day09_p2(w, lst) == 1263735);
}

//******************************************************************************
