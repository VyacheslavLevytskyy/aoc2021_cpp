/*
 *
 */

#include "utils.h"

using namespace aoc2021;

using num_score = unsigned long long;

namespace
{

inline void inc(map<uint16_t, num_score>& m, uint16_t key, num_score n)
{
    auto it = m.find(key);
    m[key] = n + (it == m.end() ? 0 : it->second);
}

class World
{
    map<uint16_t, num_score> m_pairs;
    map<uint16_t, num_score> m_freq;
    map<uint16_t, uint16_t> m_rules;

public:
    World(const vector<string>& l);
    num_score weight() const;
    void transform(size_t steps);
};

World::World(const vector<string>& l)
{
    if(l.size() < 3)
        throw invalid_argument("unexpected task format");

    const string& s = l[0];
    if(s.size() < 1)
        throw invalid_argument("unexpected task format");

    uint16_t a, b = s[0];
    inc(m_freq, b, 1);
    for(size_t i = 1; i < s.size(); i++)
    {
        a = b;
        b = s[i];
        inc(m_pairs, (a << 8) | b, 1);
        inc(m_freq, b, 1);
    }

    for(auto it = l.begin() + 2; it != l.end(); it++)
    {
        auto pos = it->find(" -> ");
        string k = it->substr(0, pos), v = it->substr(pos + 4);
        m_rules[(static_cast<uint16_t>(k[0]) << 8) | k[1]] = v[0];
    }
}

num_score World::weight() const
{
    auto it = m_freq.cbegin();
    num_score mx = it->second, mn = it->second;
    for(it++; it != m_freq.end(); it++)
    {
        mx = max(mx, it->second);
        mn = min(mn, it->second);
    }
    return mx - mn;
}

void World::transform(size_t steps)
{
    for(size_t i = 0; i < steps; i++)
    {
        map<uint16_t, num_score> pairs;
        for(auto [key, cb]: m_pairs)
        {
            auto it = m_rules.find(key);
            if(it == m_rules.end())
                inc(pairs, key, cb);
            else
            {
                uint16_t to_insert = it->second, hi = key & 0xFF00, lo = key & 0xFF;
                inc(pairs, hi | to_insert, cb);
                inc(pairs, (to_insert << 8) | lo, cb);
                inc(m_freq, to_insert, cb);
            }
        }
        m_pairs = move(pairs);
    }
}

//******************************************************************************

num_score day14_p1(const vector<string>& l)
{
    World w{l};
    w.transform(10);
    return w.weight();
}

num_score day14_p2(const vector<string>& l)
{
    World w{l};
    w.transform(40);
    return w.weight();
}

}

void aoc2021::day14()
{
#if DEV
    auto sample = Utils{"data/day14/sample.txt"}.read_lines();
    assert(day14_p1(sample) == 1588);
    assert(day14_p2(sample) == 2188189693529ULL);
#endif

    auto input = Utils{"data/day14/input.txt"}.read_lines();
    assert(day14_p1(input) == 2112);
    assert(day14_p2(input) == 3243771149914ULL);
}

//******************************************************************************
