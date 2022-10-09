/*
 *
 */

#include <execution>
#include <fstream>
#include <regex>
#include <cstring>

#include "utils.h"

using namespace aoc2021;

vector<string> aoc2021::Utils::read_lines()
{
    std::ifstream input{m_path};
    if (!input.is_open())
    {
        snprintf(m_buffer, m_sz, R"(cannot open file: "%s")", m_path.c_str());
        throw invalid_argument(m_buffer);
    }

    vector<string> lines;
    for(string line; getline(input, line); )
        lines.push_back(move(line));

    return lines;
}

vector<int> aoc2021::Utils::read_ints()
{
    vector<string> lines = read_lines();
    vector<int> ints(lines.size());
    transform(execution::par, lines.cbegin(), lines.cend(), ints.begin(),
        [](string s) { return stoi(s); });
    return ints;
}

vector<Utils::nav_t> aoc2021::Utils::read_nav_cmds()
{
    vector<string> lines = read_lines();
    vector<Utils::nav_t> cmds(lines.size());
    transform(execution::par, lines.cbegin(), lines.cend(), cmds.begin(), [](string line) {
        auto delimiterPos = line.find(" ");
        string_view cmd = line.substr(0, delimiterPos);
        int code = cmd == "forward" ? 1 : (cmd == "down" ? 2 : 3);
        return make_pair(
            code,
            stoi(line.substr(delimiterPos + 1)));
    });
    return cmds;
}

vector<Utils::coord> aoc2021::Utils::read_coords()
{
    vector<string> lines = read_lines();
    vector<Utils::coord> coords(lines.size());
    regex re("(\\d+),(\\d+) -> (\\d+),(\\d+)");
    transform(execution::par, lines.cbegin(), lines.cend(), coords.begin(), [re](string line) {
        smatch match;
        if(regex_search(line, match, re) && match.size() == 5)
        {
            Utils::coord rec;
            for(size_t i = 1; i <= 4; i++)
                rec[i - 1] = static_cast<int16_t>(stoi(match.str(i)));
            return rec;
        }
        else
            throw invalid_argument("unexpected coordinate format");
    });
    return coords;
}

vector<pair<int, Utils::cube>> Utils::read_cubes()
{
    vector<string> lines = read_lines();
    vector<pair<int, Utils::cube>> cubes(lines.size());
    regex re("(on|off) x=([-\\d]+)..([-\\d]+),y=([-\\d]+)..([-\\d]+),z=([-\\d]+)..([-\\d]+)");
    transform(execution::par, lines.cbegin(), lines.cend(), cubes.begin(), [re](string line) {
        smatch match;
        if(regex_search(line, match, re) && match.size() == 8)
        {
            int onoff = match.str(1) == "on" ? 1 : 0;
            Utils::cube rec;
            for(size_t i = 2; i <= 7; i++)
                rec[i - 2] = static_cast<bigint>(stoi(match.str(i)));
            return make_pair(onoff, rec);
        }
        else
            throw invalid_argument("unexpected coordinate format");
    });
    return cubes;
}

vector<Utils::display> aoc2021::Utils::read_displays()
{
    vector<string> lines = read_lines();
    vector<Utils::display> displays(lines.size());
    regex re(R"((\w+)\s(\w+)\s(\w+)\s(\w+)\s(\w+)\s(\w+)\s(\w+)\s(\w+)\s(\w+)\s(\w+)\s)"
             R"(\|\s(\w+)\s(\w+)\s(\w+)\s(\w+))");
    transform(execution::par, lines.cbegin(), lines.cend(), displays.begin(), [this, re](string line) {
        smatch match;
        if(regex_search(line, match, re) && match.size() == 15)
        {
            array<string,10> l;
            array<string,4> r;
            copy(cbegin(match) + 1, cbegin(match) + 11, begin(l));
            copy(cbegin(match) + 11, cbegin(match) + 15, begin(r));
            return make_pair(l, r);
        }
        else
        {
            snprintf(m_buffer, m_sz, R"(unexpected number of captured groups: %lu)", match.size());
            throw invalid_argument(m_buffer);
        }
    });
    return displays;
}

//******************************************************************************

void test_utils()
{
    try
    {
        Utils utils("input.txt");
        utils.read_lines();
    }
    catch (const exception& e)
    {
        assert(!strcmp(e.what(), "cannot open file: \"input.txt\""));

        Utils utils("data/day01/input.txt");
        vector<string> lines = utils.read_lines();
        assert(equal(lines.begin(), lines.begin() + 3, vector<string>{"123", "126", "130"}.begin()));
        vector<int> ints = utils.read_ints();
        assert(equal(ints.begin(), ints.begin() + 3, vector<int>{123, 126, 130}.begin()));

        cout << "Utils tests OK..." << endl;

        return;
    }

    throw logic_error("Utils tests fail");
}

//******************************************************************************
