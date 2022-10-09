/*
 *
 */

#include "utils.h"

using namespace aoc2021;

template<typename K>
struct std::hash<tuple<K, K, K, K>>
{
    size_t operator()(tuple<K, K, K, K> const& k) const noexcept
    {
        size_t seed = 0;
        hash_combine(seed, get<0>(k));
        hash_combine(seed, get<1>(k));
        hash_combine(seed, get<2>(k));
        hash_combine(seed, get<3>(k));
        return seed;
    }
};

namespace
{

void parse_pos(const vector<string>& l, unsigned& pos1, unsigned& pos2)
{
    static unsigned i = 28;
    pos1 = stoi(l[0].substr(i));
    pos2 = stoi(l[1].substr(i));
}

template <typename K, typename V>
void inc_map(K key, V inc, unordered_map<K, V>& acc)
{
    auto it = acc.find(key);
    acc[key] = inc + (it == acc.end() ? 0 : it->second);
}

inline
unsigned mod(unsigned x1, unsigned x2, unsigned lim)
{
    return (x1 + x2 - 1) % lim + 1;
}

unsigned use_dice(unsigned dice, unsigned n, unsigned dice_mod)
{
    unsigned s = 0;
    for(unsigned i = 0; i < n; i++)
        s += mod(dice, i, dice_mod);
    return s;
}

unsigned do_p1(unsigned a, unsigned b, unsigned board_mod, unsigned dice_mod, unsigned n, unsigned win_sum)
{
    for(unsigned va = 0, vb = 0, dice = 1, step = 0, d;; step += 2 * n)
    {
        d = use_dice(dice, n, dice_mod);
        dice = mod(dice, n, dice_mod);
        va += a = mod(a, d, board_mod);
        if(va >= win_sum)
            return vb * (step + n);
        else
        {
            d = use_dice(dice, n, dice_mod);
            dice = mod(dice, n, dice_mod);
            vb += b = mod(b, d, board_mod);
            if(vb >= win_sum)
                return va * (step + 2 * n);
        }
    }
}

//******************************************************************************

using num_score = unsigned long long;
using Map = unordered_map<tuple<unsigned, unsigned, unsigned, unsigned>, num_score>;

num_score do_p2(const vector<string>& l, unsigned board_mod, unsigned win_sum)
{
    num_score win1 = 0, win2 = 0;
    unsigned pos1, pos2;
    num_score delta;
    parse_pos(l, pos1, pos2);

    static map<unsigned, num_score> cases{
        {3,1}, {4,3}, {5,6}, {6,7}, {7,6}, {8,3}, {9,1}
    };

    for(Map states = {{{pos1, 0U, pos2, 0U}, 1}}; states.size() > 0;)
    {
        Map new_states;
        for(auto [d, dn]: cases)
        {
            for(auto [key, sn]: states)
            {
                auto [a, va, b, vb] = key;
                delta = dn * sn;
                va += a = mod(a, d, board_mod);
                if(va >= win_sum)
                    win1 += delta;
                else
                    inc_map(make_tuple(a, va, b, vb), delta, new_states);
            }
        }
        states = move(new_states);
        new_states.clear();

        for(auto [d, dn]: cases)
        {
            for(auto [key, sn]: states)
            {
                auto [a, va, b, vb] = key;
                delta = dn * sn;
                vb += b = mod(b, d, board_mod);
                if(vb >= win_sum)
                    win2 += delta;
                else
                    inc_map(make_tuple(a, va, b, vb), delta, new_states);
            }
        }
        states = move(new_states);
        new_states.clear();
    }

    return max(win1, win2);
}

//******************************************************************************

unsigned day21_p1(const vector<string>& l)
{
    unsigned pos1, pos2;
    parse_pos(l, pos1, pos2);
    return do_p1(pos1, pos2, 10, 100, 3, 1000);
}

num_score day21_p2(const vector<string>& l)
{
    return do_p2(l, 10, 21);
}

}

void aoc2021::day21()
{
#if DEV
    auto sample = Utils{"data/day21/sample.txt"}.read_lines();
    assert(day21_p1(sample) == 739785ULL);
    assert(day21_p2(sample) == 444356092776315ULL);
#endif

    auto input = Utils{"data/day21/input.txt"}.read_lines();
    assert(day21_p1(input) == 432450ULL);
    assert(day21_p2(input) == 138508043837521ULL);
}

//******************************************************************************
