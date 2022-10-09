/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

using Coords = set<pair<int, int>>;
using GenPos = void (*)(int, int, map<int, Coords>&);

double vy(double t, int pos)
{
    return pos / t - (t + 1) / 2.0;
}

double vx(double t, int pos)
{
    return pos / t + (t - 1) / 2.0;
}

inline int dist(int v)
{
    return (v * (v + 1)) / 2;
}

void ty_pos(int t, int pos, map<int, Coords>& acc)
{
    for(double v = vy(t, pos); v > 0; t++, v = vy(t, pos))
    {
        int vi = int(v);
        if(v - vi == 0.0)
        {
            int key = 2 * vi + 1 + t;
            auto value = make_pair(vi, vi + t);
            auto it = acc.find(key);
            if(it == acc.end())
                acc[key] = Coords{value};
            else
                it->second.insert(value);
        }
    }
}

void tx_pos(int t, int pos, map<int, Coords>& acc)
{
    for(double v = vx(t, pos); v >= t - 1; t++, v = vx(t, pos))
    {
        int vi = int(v);
        if(v - vi == 0.0)
        {
            auto value = make_pair(vi, vi - t + 1);
            auto it = acc.find(t);
            if(it == acc.end())
                acc[t] = Coords{value};
            else
                it->second.insert(value);
        }
    }
}

void gen_pos(GenPos f, int start, int pos, map<int, Coords>& acc)
{
    for(; pos >= start; pos--)
        f(1, pos, acc);
}

size_t gen_all_pos(int sx1, int sx2, int sy1, int sy2, bool run_async = false)
{
    map<int, Coords> xts, yts, ts;

    if(run_async)
    {
        auto h1 = async(launch::async, gen_pos, tx_pos, -sy2, -sy1, ref(yts));
        auto h2 = async(launch::async, gen_pos, tx_pos, sx1, sx2, ref(xts));
        auto h3 = async(launch::async, gen_pos, ty_pos, -sy2, -sy1, ref(ts));

        h1.get();
        h2.get();
        h3.get();
    }
    else
    {
        gen_pos(tx_pos, -sy2, -sy1, yts);
        gen_pos(tx_pos, sx1, sx2, xts);
        gen_pos(ty_pos, -sy2, -sy1, ts);
    }

    Coords common;
    for(auto const& [t, lx]: xts)
    {
        auto it = yts.find(t);
        if(it != yts.end())
            for(auto const& lxi: lx)
                for(auto const& lyi: it->second)
                    common.insert(make_pair(lxi.first, -lyi.second));
    }
    for(auto const& [t, ly]: ts)
    {
        auto it = xts.find(t);
        if(it == xts.end())
        {
            for(auto const& [tx, lx0]: xts) if(tx < t)
                for(auto const& lx0i: lx0) if(lx0i.second == 0)
                    for(auto const& lyi: ly)
                        common.insert(make_pair(lx0i.first, lyi.first));
        }
        else
            for(auto const& lxi: it->second)
                for(auto const& lyi: ly)
                    common.insert(make_pair(lxi.first, lyi.first));
    }
    return common.size();
}

//******************************************************************************

int day17_p1(const vector<int>& l)
{
    if(l[3] < l[2])
        throw invalid_argument("unexpected task format");
    map<int, Coords> ts;
    gen_pos(ty_pos, -l[3], -l[2], ts);
    // map and set are implemented as RB-trees: the last element is the maximum
    return dist(ts.rbegin()->second.rbegin()->first);
}

size_t day17_p2(const vector<int>& l)
{
    if(l[1] < l[0] || l[3] < l[2])
        throw invalid_argument("unexpected task format");
    return gen_all_pos(l[0], l[1], l[2], l[3]);
}

}

void aoc2021::day17()
{
#if DEV
    auto sample = Utils{"data/day17/sample.txt"}.read_ints();
    assert(day17_p1(sample) == 45);
    assert(day17_p2(sample) == 112);
#endif

    auto input = Utils{"data/day17/input.txt"}.read_ints();
    assert(day17_p1(input) == 9180);
    assert(day17_p2(input) == 3767);
}

//******************************************************************************
