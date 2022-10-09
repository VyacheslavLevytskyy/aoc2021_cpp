/*
 *
 */

#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <iostream>
#include <functional>
#include <future>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <chrono>

#define DEV 0

using namespace std;

namespace aoc2021
{

[[maybe_unused]] inline
ostream& operator<<(ostream& o, const tuple<int, int, int>& elem)
{
    return o << "[" << get<0>(elem) << ", " << get<1>(elem) << ", " << get<2>(elem) << "]";
}

template <typename T>
ostream& operator<<(ostream& o, const pair<T, T>& elem)
{
    return o << "[" << elem.first << ", " << elem.second << "]";
}

[[maybe_unused]] inline
ostream& operator<<(ostream& o, const tuple<char, size_t, size_t>& elem)
{
    auto [x, r, c] = elem;
    return o << make_pair(static_cast<int>(r), static_cast<int>(c)) << ": " << x;
}

template<typename T>
ostream& operator<<(ostream& o, const vector<T>& vec)
{
    for(auto const& elem: vec)
        o << "    " << elem << endl;
    return o;
}

template<typename K, typename V>
ostream& operator<<(ostream& o, const map<K, V>& data)
{
    for(auto const& [k, v]: data)
        o << k << ": " << v << endl;
    return o;
}

template<typename K>
ostream& operator<<(ostream& o, const set<K>& data)
{
    for(auto const& k: data)
        o << k << endl;
    return o;
}

class Utils
{
    constexpr static unsigned m_sz = 1024;
    char m_buffer[m_sz];

protected:
    string m_path;

public:
    Utils(string_view path) : m_path(path) {}

    vector<string> read_lines();
    vector<int> read_ints();

    using coord = array<int16_t, 4>;
    vector<coord> read_coords();

    using nav_t = pair<int, int>;
    vector<nav_t> read_nav_cmds();

    using bigint = long long;
    using cube = array<bigint, 6>;
    vector<pair<int, cube>> read_cubes();

    using display = pair<array<string,10>,array<string,4>>;
    vector<display> read_displays();

    template <typename T> requires integral<T>
    static vector<T> read_int_line(const string& s);
};

// hash_combine() is from #include <boost/functional/hash/hash.hpp>
template <typename T>
void hash_combine(size_t& seed, T const& v)
{
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T> requires integral<T>
vector<T> aoc2021::Utils::read_int_line(const string& s)
{
    vector<T> numbers;
    size_t pos1 = 0;
    for(size_t pos2; (pos2 = s.find(",", pos1)) != string::npos; pos1 = pos2 + 1)
        numbers.push_back(stoi(s.substr(pos1, pos2)));
    numbers.push_back(static_cast<T>(stoi(s.substr(pos1))));
    return numbers;
}

void day01();
void day02();
void day03();
void day04();
void day05();
void day06();
void day07();
void day08();
void day09();
void day10();
void day11();
void day12();
void day13();
void day14();
void day15();
void day16();
void day17();
void day18();
void day19();
void day20();
void day21();
void day22();
void day23();
void day24();
void day25();

} // namespace aoc2021

//******************************************************************************
