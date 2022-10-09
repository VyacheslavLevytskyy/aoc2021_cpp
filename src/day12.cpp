/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

constexpr int START = 0;
constexpr int RESERVED = -1;
constexpr int END = -2;

int encode(const string& s, map<string, int>& dict, int& hi, int& lo)
{
    int res;
    auto it = dict.find(s);
    if(it == dict.end())
    {
        res = (s[0] >= 'A' && s[0] <= 'Z') ? hi++ : lo--;
        dict[s] = res;
    }
    else
        res = it->second;
    return res;
}

void connect(map<int, set<int>>& connected, int from, int to)
{
    auto it = connected.find(from);
    if(it == connected.end())
        connected[from] = set{to};
    else
        it->second.insert(to);
}

map<int, set<int>> create_map(const vector<string>& l)
{
    map<int, set<int>> connected;
    map<string, int> dict{
        {"start", START},
        {"end", END}
    };
    int hi = START + 1, lo = END - 1;
    for(auto const& s: l)
    {
        auto pos = s.find("-");
        int ki = encode(s.substr(0, pos), dict, hi, lo);
        int vi = encode(s.substr(pos + 1), dict, hi, lo);
        connect(connected, ki, vi);
        connect(connected, vi, ki);
    }
    return connected;
}

int branch(const map<int, set<int>>& m, int current, const set<int>& visited, int visited_twice);

int aggregate(const map<int, set<int>>& m, const set<int>& tos, const set<int>& visited, int visited_twice)
{
    int cb = 0;
    for(int to: tos)
    {
        if(to == END)
            cb++;
        else if(to > 0)
            cb += branch(m, to, visited, visited_twice);
        else if(!visited.contains(to))
        {
            set<int> visited_copy(visited);
            visited_copy.insert(to);
            cb += branch(m, to, visited_copy, visited_twice);
        }
        else if(visited_twice == RESERVED && to != START)
            cb += branch(m, to, visited, to);
    }
    return cb;
}

int branch(const map<int, set<int>>& m, int current, const set<int>& visited, int visited_twice)
{
    auto it = m.find(current);
    return it == m.end() ? 0 : aggregate(m, it->second, visited, visited_twice);
}

int day12_p1(const map<int, set<int>>& m)
{
    set<int> tos{START};
    return branch(m, START, tos, START);
}

int day12_p2(const map<int, set<int>>& m)
{
    set<int> tos{START};
    return branch(m, START, tos, RESERVED);
}

}

void aoc2021::day12()
{
#if DEV
    auto sample = Utils{"data/day12/sample.txt"}.read_lines();
    auto sample_map = create_map(sample);
    assert(day12_p1(sample_map) == 10);
    assert(day12_p2(sample_map) == 36);
#endif

    auto input = Utils{"data/day12/input.txt"}.read_lines();
    auto input_map = create_map(input);
    assert(day12_p1(input_map) == 3450);
    assert(day12_p2(input_map) == 96528);
}

//******************************************************************************
