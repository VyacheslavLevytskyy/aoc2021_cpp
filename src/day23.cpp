/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

using Bigint = unsigned long long;

enum Token {A = 1, B = 10, C = 100, D = 1000};
using State = unordered_map<int, Token>;
using Map = unordered_map<int, vector<int>>;

#define BITSET_HASH 1
#if BITSET_HASH
using HashKey = bitset<88>;
#else
using HashKey = string;
#endif

static State START_P1{
    {100, C}, {101, B}, {200, B}, {201, C},
    {300, A}, {301, D}, {400, D}, {401, A}
};

static State START_P2{
    {100, C}, {101, D}, {102, D}, {103, B},
    {200, B}, {201, C}, {202, B}, {203, C},
    {300, A}, {301, B}, {302, A}, {303, D},
    {400, D}, {401, A}, {402, C}, {403, A}
};

static State START_S1{
    {100, B}, {101, A}, {200, C}, {201, D},
    {300, B}, {301, C}, {400, D}, {401, A}
};

static State START_S2{
    {100, B}, {101, D}, {102, D}, {103, A},
    {200, C}, {201, C}, {202, B}, {203, D},
    {300, B}, {301, B}, {302, A}, {303, C},
    {400, D}, {401, A}, {402, C}, {403, A}
};

static State START_G1{
    {100, A}, {101, A},
    {200, B}, {201, B},
    {300, C}, {301, C},
    {400, D}, {401, D}
};

static State START_G2{
    {100, A}, {101, A}, {102, A}, {103, A},
    {200, B}, {201, B}, {202, B}, {203, B},
    {300, C}, {301, C}, {302, C}, {303, C},
    {400, D}, {401, D}, {402, D}, {403, D}
};

static Map graph1{
    {101, {100}},
    {100, {3, 101}},
    {201, {200}},
    {200, {5, 201}},
    {301, {300}},
    {300, {7, 301}},
    {401, {400}},
    {400, {9, 401}},

    {1, {2}},
    {2, {1, 3}},
    {3, {2, 4, 100}},
    {4, {3, 5}},
    {5, {4, 6, 200}},
    {6, {5, 7}},
    {7, {6, 8, 300}},
    {8, {7, 9}},
    {9, {8, 10, 400}},
    {10, {9, 11}},
    {11, {10}}
};

static Map graph2{
    {103, {102}},
    {102, {101, 103}},
    {101, {100, 102}},
    {100, {3, 101}},

    {203, {202}},
    {202, {201, 203}},
    {201, {200, 202}},
    {200, {5, 201}},

    {303, {302}},
    {302, {301, 303}},
    {301, {300, 302}},
    {300, {7, 301}},

    {403, {402}},
    {402, {401, 403}},
    {401, {400, 402}},
    {400, {9, 401}},

    {1, {2}},
    {2, {1, 3}},
    {3, {2, 4, 100}},
    {4, {3, 5}},
    {5, {4, 6, 200}},
    {6, {5, 7}},
    {7, {6, 8, 300}},
    {8, {7, 9}},
    {9, {8, 10, 400}},
    {10, {9, 11}},
    {11, {10}}
};

//******************************************************************************

constexpr int ROOM = 100;

inline int room(int pos)
{
    return pos / ROOM;
}

inline int get_idx(int pos)
{
    return pos % ROOM;
}

inline int get_pos(int room, int idx)
{
    return room * ROOM + idx;
}

bool at_home(int pos, Token token)
{
    switch(room(pos))
    {
        case 1:
            return token == A;
        case 2:
            return token == B;
        case 3:
            return token == C;
        case 4:
            return token == D;
        default:
            return false;
    }
}

bool is_solution(const State& s, int max_idx)
{
    for(int room_no = 1; room_no <= 4; room_no++)
    {
        for(int idx = 0; idx <= max_idx; idx++)
        {
            int pos = get_pos(room_no, idx);
            auto it = s.find(pos);
            if(it == s.end() || !at_home(pos, it->second))
                return false;
        }
    }
    return true;
}

inline int out(int from)
{
    return room(from) * 2 + 1;
}

inline bool is_exit(int pos)
{
    return pos == 3 || pos == 5 || pos == 7 || pos == 9;
}

inline bool is_same_room(int pos1, int pos2)
{
    return room(pos1) == room(pos2);
}

inline int get_cost(Token token, int from, int to)
{
    if(from >= ROOM && to >= ROOM)
        return token * (abs(out(from) - out(to)) + 2 + get_idx(from) + get_idx(to));
    else if(from >= ROOM)
        return token * (abs(to - out(from)) + 1 + get_idx(from));
    else
        return token * (abs(from - out(to)) + 1 + get_idx(to));
}

int estimate(const State& s0, const State& goal0, int max_idx)
{
    int c = 0;
    Token token;
    for(int room_no = 1; room_no <= 4; room_no++)
    {
        int stranger = 0;
        for(int idx = max_idx; idx >= 0; idx--)
        {
            int pos = get_pos(room_no, idx);
            auto it = s0.find(pos);
            if(it != s0.end())
            {
                token = it->second;
                if(!at_home(pos, token))
                    stranger++;
                else if(stranger > 0)
                    c += 2 * get_cost(token, pos, 1 + out(pos));
            }
        }
    }

    State goal, s(s0);
    for(auto [k, v]: goal0)
    {
        auto it = s.find(k);
        if(it != s.end() && it->second == v)
            s.erase(it);
        else
            goal[k] = v;
    }
    for(auto [k, v]: goal)
    {
        for(auto it = s.begin(); it != s.end(); it++)
        {
            if(it->second == v)
            {
                c += get_cost(v, it->first, k);
                s.erase(it);
                break;
            }
        }
    }
    return c;
}

#if BITSET_HASH

inline uint8_t token_view(Token token)
{
    switch(token)
    {
        case A:
            return 0b00000001;
        case B:
            return 0b00000010;
        case C:
            return 0b00000011;
        case D:
            return 0b00000100;
    }
    return 0;
}

inline void add_token_to_hash(HashKey& b, const State& s, int k)
{
    b <<= 3;
    auto it = s.find(k);
    if(it != s.end())
        b |= token_view(it->second);
}

#else

inline char token_view(Token token)
{
    switch(token)
    {
        case A:
            return 'A';
        case B:
            return 'B';
        case C:
            return 'C';
        case D:
            return 'D';
    }
    return '.';
}

inline void add_token_to_hash(HashKey& b, const State& s, int k)
{
    auto it = s.find(k);
    b += it != s.end() ? token_view(s.at(k)) : '.';
}

#endif

HashKey hash_state(const State& s, int max_idx)
{
    HashKey b;
    for(int i = 1; i <= 11; i++)
        add_token_to_hash(b, s, i);
    for(int room_no = 1; room_no <= 4; room_no++)
        for(int idx = 0; idx <= max_idx; idx++)
            add_token_to_hash(b, s, get_pos(room_no, idx));
    return b;
}

//******************************************************************************

void paths(const Map& graph, int max_idx, const State& s, Token token,
           int pos0, int pos1, const unordered_set<int>& visited, vector<int>& stops)
{
    const vector<int>& tos = graph.at(pos1);
    for(auto pos2: tos)
    {
        if(!visited.contains(pos2) && !s.contains(pos2))
        {
            unordered_set<int> visited2(visited);
            visited2.insert(pos1);

            bool proceed = false;
            if(at_home(pos2, token))
            {
                proceed = true;
                if(pos0 < ROOM || !is_same_room(pos0, pos2))
                {
                    bool eor = true; // end of room
                    for(int idx = get_idx(pos2) + 1; idx <= max_idx; idx++)
                    {
                        auto it = s.find(get_pos(room(pos2), idx));
                        if(it == s.end())
                            eor = false;
                        else if(it->second != token)
                        {
                            proceed = false;
                            break;
                        }
                    }
                    if(proceed && eor)
                        stops.push_back(pos2);
                }
            }
            else if(pos0 < ROOM)
            {
                if(pos2 < ROOM)
                    proceed = true;
            }
            else
            {
                if(pos2 < ROOM)
                {
                    if(!is_exit(pos2))
                        stops.push_back(pos2);
                    proceed = true;
                }
                else
                    proceed = is_same_room(pos0, pos2);
            }

            if(proceed)
                paths(graph, max_idx, s, token, pos0, pos2, visited2, stops);
        }
    }
}

list<tuple<int, int, State>> branches(const Map& graph, int max_idx, const State& s, int cost, int best)
{
    const State& goal = max_idx == 1 ? START_G1 : START_G2;
    list<tuple<int, int, State>> q;
    for(auto [pos0, token]: s)
    {
        if(at_home(pos0, token))
        {
            int idx;
            for(idx = get_idx(pos0) + 1; idx <= max_idx; idx++)
            {
                auto it = s.find(get_pos(room(pos0), idx));
                if(it == s.end() || it->second != token)
                    break;
            }
            if(idx == max_idx + 1)
                continue;
        }

        unordered_set<int> visited;
        vector<int> stops;
        paths(graph, max_idx, s, token, pos0, pos0, visited, stops);

        for(auto pos2: stops)
        {
            if(at_home(pos2, token))
            {
                stops.clear();
                stops.push_back(pos2);
                break;
            }
        }

        for(auto pos2: stops)
        {
            State s2(s);
            s2.erase(pos0);
            s2[pos2] = token;
            int c = cost + get_cost(token, pos0, pos2);
            int ecost = c + estimate(s2, goal, max_idx);
            if(ecost < best)
                q.push_front({c, ecost, s2});
        }
    }
    return q;
}

int play(const Map& graph, int max_idx, const State& init, bool verbose = false)
{
    int best = INT_MAX;
    unordered_map<HashKey, int> logs;
    list<tuple<int, int, State>> q{
        {0, estimate(init, max_idx == 1 ? START_G1 : START_G2, max_idx), init}
    };

    while(!q.empty())
    {
        auto& [cost, ecost, s] = q.front();
        if(ecost < best)
        {
            auto skey = hash_state(s, max_idx);
            auto logs_it = logs.find(skey);
            if(logs_it == logs.end() || logs_it->second > cost)
            {
                logs[skey] = cost;
                if(is_solution(s, max_idx))
                {
                    if(cost < best)
                    {
                        best = cost;
                        if(verbose)
                            cout << "    new best solution: " << cost << endl;
                    }
                }
                else
                {
                    auto newq = branches(graph, max_idx, s, cost, best);
                    q.pop_front();
                    q.splice(q.begin(), newq);
                    q.sort([](auto const& i1, auto const& i2){
                        return -get<1>(i1) < -get<1>(i2);
                    });
                    continue;
                }
            }
        }
        q.pop_front();
    }
    return best;
}

//******************************************************************************

int day23_p1(const State& s)
{
    return play(graph1, 1, s);
}

int day23_p2(const State& s)
{
    return play(graph2, 3, s);
}

}

void aoc2021::day23()
{
#if DEV
    assert(day23_p1(START_S1) == 12521);
    assert(day23_p2(START_S2) == 44169);
#endif

    assert(day23_p1(START_P1) == 11320);
    assert(day23_p2(START_P2) == 49532);
}

//******************************************************************************
