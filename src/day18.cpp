/*
 *
 */

#define PARALLEL 1

#if PARALLEL
#include <thread>
#include <mutex>
#include <execution>
#endif

#include "utils.h"

using namespace aoc2021;

namespace
{

using FNumber = list<uint32_t>;

inline uint32_t get_depth(uint32_t v)
{
    return v & 0x000000FF;
}

inline uint32_t get_value(uint32_t v)
{
    return v >> 8;
}

inline uint32_t create_number(uint32_t value, uint32_t depth)
{
    return (value << 8) | depth;
}

inline void inc_depth(uint32_t& v)
{
    v++;
}

unsigned magnitude(FNumber& tree)
{
    while(tree.size() > 1)
    {
        auto it1 = tree.begin(), it2 = next(it1);
        uint32_t v1 = *it1, v2 = *it2;
        while(get_depth(v1) != get_depth(v2))
        {
            v1 = v2;
            v2 = *++it2;
        }
        *prev(it2) = create_number(3 * get_value(v1) + 2 * get_value(v2), get_depth(v1) - 1);
        tree.erase(it2);
    }
    return get_value(*tree.begin());
}

bool explode(FNumber& tree)
{
    for(auto it1 = tree.begin(), it2 = next(it1); it2 != tree.end(); it1 = it2, it2++)
    {
        uint32_t v1 = *it1, v2 = *it2;
        if(get_depth(v1) == 5 && get_depth(v2) == 5)
        {
            if(it1 != tree.begin())
            {
                uint32_t& p = *prev(it1);
                p = create_number(get_value(p) + get_value(v1), get_depth(p));
            }
            auto it3 = next(it2);
            if(it3 != tree.end())
            {
                uint32_t& p = *it3;
                p = create_number(get_value(p) + get_value(v2), get_depth(p));
            }
            *it1 = create_number(0, 4);
            tree.erase(it2);
            return true;
        }
    }
    return false;
}

bool split(FNumber& tree)
{
    for(auto it = tree.begin(); it != tree.end(); it++)
    {
        uint32_t num = *it, val = get_value(num);
        if(val >= 10)
        {
            uint32_t depth = get_depth(num) + 1;
            uint32_t l = val / 2;
            *it = create_number(l, depth);
            tree.insert(next(it), create_number(val - l, depth));
            return true;
        }
    }
    return false;
}

void sum(FNumber& tree, const string& s)
{
    for(uint32_t& v: tree)
        inc_depth(v);
    uint32_t depth = tree.size() > 0 ? 1 : 0;
    for(auto ch: s)
    {
        if(ch == '[')
            depth++;
        else if(ch == ']')
            depth--;
        else if(ch >= '0' && ch <= '9')
            tree.push_back(create_number(ch - '0', depth));
    }
    while(explode(tree) || split(tree));
}

unsigned day18_p1(const vector<string>& l)
{
    FNumber tree;
    for(auto s: l)
        sum(tree, s);
    return magnitude(tree);
}

[[maybe_unused]]
unsigned day18_p2_seq(const vector<string>& l)
{
    uint32_t max_mag = 0;
    size_t i, j;
    for(i = 0; i < l.size(); i++)
        for(j = 0; j < l.size(); j++)
            if(i != j)
            {
                FNumber tree;
                sum(tree, l[i]);
                sum(tree, l[j]);
                max_mag = max(max_mag, magnitude(tree));
            }
    return max_mag;
}

[[maybe_unused]]
unsigned day18_p2_par(const vector<string>& l, bool verbose = false)
{
    vector<pair<size_t, size_t>> pairs;
    pairs.reserve(l.size() * l.size());
    for(size_t i = 0; i < l.size(); i++)
        for(size_t j = 0; j < l.size(); j++)
            if(i != j)
                pairs.push_back(make_pair(i, j));

    set<thread::id> ids;
    mutex mtx;

    unsigned max_mag = transform_reduce(execution::par, pairs.cbegin(), pairs.cend(), 0U,
        [](unsigned a, unsigned b){
            return max(a, b);
        },
        [verbose, &l, &mtx, &ids](const pair<size_t, size_t>& ij){
            FNumber tree;
            sum(tree, l[ij.first]);
            sum(tree, l[ij.second]);
            unsigned mag = magnitude(tree);
            if(verbose)
            {
                // enumerate threads
                auto this_id = this_thread::get_id();
                lock_guard lock(mtx);
                if(!ids.contains(this_id))
                    ids.insert(this_id);
            }
            return mag;
        });

    if(verbose)
    {
        cout << "Used threads: ";
        copy(ids.begin(), ids.end(), ostream_iterator<thread::id>(cout, " "));
        cout << endl;
    }

    return max_mag;
}

#if PARALLEL
#define day18_p2 day18_p2_par
#else
#define day18_p2 day18_p2_seq
#endif

}

void aoc2021::day18()
{
#if DEV
    auto sample1 = Utils{"data/day18/sample1.txt"}.read_lines();
    assert(day18_p1(sample1) == 4140);
    assert(day18_p2(sample1) == 3993);

    auto sample = Utils{"data/day18/sample.txt"}.read_lines();
    assert(day18_p1(sample) == 3488);
    assert(day18_p2(sample) == 3805);
#endif

    auto input = Utils{"data/day18/input.txt"}.read_lines();
    assert(day18_p1(input) == 3524);
    assert(day18_p2(input) == 4656);
}

//******************************************************************************
