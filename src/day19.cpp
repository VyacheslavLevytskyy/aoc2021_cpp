/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

using Pos = tuple<int, int, int>;
using Pair = pair<Pos, Pos>;
using TransId = pair<int, int>;
using TransIds = list<TransId>;
using DistFun = int (*) (const Pos&, const Pos&);

template <typename T>
vector<pair<T, T>> pairs(const vector<T>& poss)
{
    vector<pair<T, T>> pairs;
    for(size_t i = 0; i < poss.size() - 1; i++)
        for(size_t j = i + 1; j < poss.size(); j++)
            pairs.push_back(make_pair(poss[i], poss[j]));
    return pairs;
}

int dist_e(const Pos& p1, const Pos& p2)
{
    auto dx = get<0>(p1) - get<0>(p2), dy = get<1>(p1) - get<1>(p2), dz = get<2>(p1) - get<2>(p2);
    return static_cast<int>(sqrt(dx*dx + dy*dy + dz*dz) * 10000);
}

int dist_m(const Pos& p1, const Pos& p2)
{
    auto dx = abs(get<0>(p1) - get<0>(p2)),
         dy = abs(get<1>(p1) - get<1>(p2)),
         dz = abs(get<2>(p1) - get<2>(p2));
    return dx + dy + dz;
}

vector<map<int, Pair>> to_dist(const vector<vector<Pos>>& s, DistFun f)
{
    vector<map<int, Pair>> d;
    for(auto const& poss: s)
    {
        map<int, Pair> distances;
        vector<Pair> scanner_pairs = pairs(poss);
        for(auto const& pair: scanner_pairs)
            distances[f(pair.first, pair.second)] = pair;
        d.push_back(move(distances));
    }
    return d;
}

template <typename K, typename V>
map<K, V> map_inter(const map<K, V>& m1, const map<K, V>& m2)
{
    map<K, V> res;
    auto b1 = m1.cbegin(), e1 = m1.cend();
    auto b2 = m2.cbegin(), e2 = m2.cend();
    while(b1 != e1 && b2 != e2)
    {
        if(b1->first < b2->first)
            b1++;
        else
        {
            if(!(b2->first < b1->first))
                res.insert(*b1++);
            b2++;
        }
    }
    return res;
}

template <typename K, typename V>
map<K, V> map_with_keys(const map<K, V>& mkeys, const map<K, V>& mvals)
{
    map<K, V> r;
    for(auto i: mvals)
        if(mkeys.contains(i.first))
            r.insert(i);
    return r;
}

template <typename K, typename V>
void add_to_map(K key, V elem, map<K, list<V>>& m)
{
    auto it = m.find(key);
    if(it == m.end())
        m[key] = list<V>{elem};
    else
        it->second.push_front(elem);
}

Pos flip(int sign, Pos p)
{
    auto [x, y, z] = p;
    switch(sign)
    {
        case 1:
            return make_tuple(x, y, z);
        case 2:
            return make_tuple(x, y, -z);
        case 3:
            return make_tuple(x, -y, z);
        case 4:
            return make_tuple(x, -y, -z);
        case 5:
            return make_tuple(-x, y, z);
        case 6:
            return make_tuple(-x, y, -z);
        case 7:
            return make_tuple(-x, -y, z);
        case 8:
            return make_tuple(-x, -y, -z);
    }
    throw logic_error("unexpected sign value");
}

Pos perm(int no, Pos p)
{
    auto [x, y, z] = p;
    switch(no)
    {
        case 1:
            return make_tuple(x, y, z);
        case 2:
            return make_tuple(x, z, y);
        case 3:
            return make_tuple(y, x, z);
        case 4:
            return make_tuple(y, z, x);
        case 5:
            return make_tuple(z, x, y);
        case 6:
            return make_tuple(z, y, x);
    }
    throw logic_error("unexpected permutation value");
}

Pos diff(Pos p, Pos q)
{
    auto [x1, y1, z1] = p;
    auto [x2, y2, z2] = q;
    return make_tuple(x1 - x2, y1 - y2, z1 - z2);
}

Pos add_diff(Pos p, Pos q)
{
    auto [x1, y1, z1] = p;
    auto [x2, y2, z2] = q;
    return make_tuple(x1 + x2, y1 + y2, z1 + z2);
}

const TransIds& trans_ids()
{
    static TransIds r = {
        {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
        {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
        {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6},
        {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6},
        {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
        {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6},
        {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6},
        {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}
    };
    return r;
}

Pos trans(TransId cmd, Pos p)
{
    return flip(cmd.first, perm(cmd.second, p));
}

Pos trans_seq(const TransIds& cmds, Pos p)
{
    for(auto cmd: cmds)
        p = trans(cmd, p);
    return p;
}

bool match(Pair pq1, Pair pq2)
{
    auto d1 = diff(pq1.first, pq1.second);
    auto d2 = diff(pq2.first, pq2.second);
    return d1 == d2;
}

bool check_match(const vector<Pair>& pqs, TransId& good_cmd)
{
    TransIds src{trans_ids()};
    for(size_t i = 0; src.size() > 0 && i < pqs.size() - 1; i++)
    {
        for(auto it = src.begin(); it != src.end();)
        {
            Pair pq1 = make_pair(pqs[i].first, trans(*it, pqs[i].second));
            Pair pq2 = make_pair(pqs[i + 1].first, trans(*it, pqs[i + 1].second));
            if(match(pq1, pq2))
                it++;
            else
                it = src.erase(it);
        }
    }
    assert(src.size() < 2);
    if(src.size() == 1)
    {
        good_cmd = *begin(src);
        return true;
    }
    return false;
}

void add_to_set(Pos key, const Pair& elem, map<Pos, list<Pos>>& m)
{
    auto [q1, q2] = elem;
    auto it = m.find(key);
    if(it == m.end())
        m[key] = list<Pos>{q1, q2};
    else
    {
        auto itset = it->second.begin();
        while(itset != it->second.end())
        {
            if(*itset != q1 && *itset != q2)
                itset = it->second.erase(itset);
            else
                itset++;

        }
    }
}

bool get_translation(const map<int, Pair>& dists1, const map<int, Pair>& dists2,
                     Pos& delta, TransId& good_cmd)
{
    map<Pos, list<Pos>> candidates;
    for(auto const& [k, v]: dists1)
    {
        const Pair& elem = dists2.at(k);
        auto [p1, p2] = v;
        add_to_set(p1, elem, candidates);
        add_to_set(p2, elem, candidates);
    }
    vector<Pair> pqs;
    for(auto [p, qs]: candidates)
    {
        if(qs.size() == 1)
            pqs.push_back(make_pair(p, *qs.begin()));
        else
            return false;
    }
    if(pqs.size() > 0 && check_match(pqs, good_cmd))
    {
        auto [p, q] = pqs[0];
        delta = diff(p, trans(good_cmd, q));
        return true;
    }
    return false;
}

//******************************************************************************

pair<size_t, vector<Pos>> intersect(const vector<vector<Pos>>& smap)
{
    vector<map<int, Pair>> dmap = to_dist(smap, dist_e);
    map<size_t, list<tuple<size_t, TransId, Pos>>> turns;
    size_t id0 = SIZE_MAX;
    for(size_t id1 = 0; id1 < dmap.size() - 1; id1++)
    {
        for(size_t id2 = id1 + 1; id2 < dmap.size(); id2++)
        {
            auto common = map_inter(dmap[id1], dmap[id2]);
            if(common.size() >= 66)
            {
                auto cdist1 = map_with_keys(common, dmap[id1]),
                     cdist2 = map_with_keys(common, dmap[id2]);
                Pos delta1, delta2;
                TransId good_cmd1, good_cmd2;
                bool status1 = get_translation(cdist1, cdist2, delta1, good_cmd1);
                bool status2 = get_translation(cdist2, cdist1, delta2, good_cmd2);
                if(status1 && status2)
                {
                    if(id0 == SIZE_MAX)
                        id0 = id1;
                    add_to_map(id1, make_tuple(id2, good_cmd1, delta1), turns);
                    add_to_map(id2, make_tuple(id1, good_cmd2, delta2), turns);
                }
            }
        }
    }
    if(id0 == SIZE_MAX)
        return make_pair(0, vector<Pos>{});

    map<size_t, list<tuple<size_t, Pos, set<Pos>>>> visited;
    list<tuple<size_t, TransIds, Pos>> steps = {{id0, {{1, 1}}, {0, 0, 0}}};
    for(auto itstep = steps.begin(); itstep != steps.end(); itstep++)
    {
        auto [id, cmds, diff] = *itstep;
        if(visited.contains(id))
        {
            itstep++;
            continue;
        }
        auto id_turns = turns.at(id);
        for(auto [id_to, cmd_id_to, diff_to]: id_turns)
        {
            if(!visited.contains(id_to))
            {
                diff_to = add_diff(diff, trans_seq(cmds, diff_to));
                TransIds new_cmds{cmds};
                new_cmds.push_front(cmd_id_to);
                set<Pos> pts_to;
                auto const& id_poss = smap[id_to];
                for(auto id_pos: id_poss)
                    pts_to.insert(add_diff(diff_to, trans_seq(new_cmds, id_pos)));
                steps.emplace_back(id_to, new_cmds, diff_to);
                add_to_map(id, make_tuple(id_to, diff_to, pts_to), visited);
            }
        }
    }
    list<tuple<size_t, Pos, set<Pos>>> all_shifts;
    set<Pos> all_points{cbegin(smap[id0]), cend(smap[id0])};
    vector<Pos> all_diffs{{0, 0, 0}};
    for(auto [_, shifts]: visited)
        all_shifts.insert(all_shifts.end(), shifts.begin(), shifts.end());
    for(auto [id_to, diff_to, pts_to]: all_shifts)
    {
        all_points.insert(cbegin(pts_to), cend(pts_to));
        all_diffs.push_back(diff_to);
    }
    return make_pair(all_points.size(), move(all_diffs));
}

//******************************************************************************

vector<vector<Pos>> parse(const vector<string>& l)
{
    vector<vector<Pos>> r{1, vector<Pos>{}};
    size_t no = 0;
    for(auto s: l)
    {
        if(s.size() == 0)
        {
            r.push_back(vector<Pos>{});
            no++;
        }
        else if(!s.starts_with("---"))
        {
            auto i = s.find(",");
            int x = stoi(s.substr(0, i));
            string rest = s.substr(i + 1);
            i = rest.find(",");
            int y = stoi(rest.substr(0, i));
            int z = stoi(rest.substr(i + 1));
            r[no].push_back(make_tuple(x, y, z));
        }
    }
    return r;
}

//******************************************************************************

size_t day19_p1(const vector<vector<Pos>>& smap)
{
    auto [sz, _] = intersect(smap);
    return sz;
}

int day19_p2(const vector<vector<Pos>>& smap)
{
    auto [_, diffs] = intersect(smap);
    vector<Pair> diff_pairs = pairs(diffs);
    int max_dist = 0;
    for(auto [p1, p2]: diff_pairs)
        max_dist = max(max_dist, dist_m(p1, p2));
    return max_dist;
}

}

void aoc2021::day19()
{
#if DEV
    auto sample = Utils{"data/day19/sample.txt"}.read_lines();
    auto smap_sample = parse(sample);
    assert(day19_p1(smap_sample) == 79);
    assert(day19_p2(smap_sample) == 3621);
#endif

    auto input = Utils{"data/day19/input.txt"}.read_lines();
    auto smap_input = parse(input);
    assert(day19_p1(smap_input) == 449);
    assert(day19_p2(smap_input) == 13128);
}

//******************************************************************************
