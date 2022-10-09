/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

using num_score = long long;

char match(char ch)
{
    switch(ch)
    {
        case ')':
            return '(';
        case ']':
            return '[';
        case '}':
            return '{';
        case '>':
            return '<';
    }
    throw invalid_argument("in match()");
}

num_score score(char ch)
{
    switch(ch)
    {
        case '(':
            return 3;
        case '[':
            return 57;
        case '{':
            return 1197;
        case '<':
            return 25137;
    }
    throw invalid_argument("in score()");
}

num_score auto_score(stack<char>& mem)
{
    num_score acc = 0;
    while(!mem.empty())
    {
        switch(mem.top())
        {
            case '(':
                acc = 5 * acc + 1;
                break;
            case '[':
                acc = 5 * acc + 2;
                break;
            case '{':
                acc = 5 * acc + 3;
                break;
            case '<':
                acc = 5 * acc + 4;
                break;
        }
        mem.pop();
    }
    return acc;
}

pair<num_score, num_score> parse(const string& s)
{
    stack<char> mem;
    char elem;
    for(auto ch: s)
    {
        if(ch == '(' || ch == '[' || ch == '{' || ch == '<')
            mem.push(ch);
        else if(mem.empty())
            throw logic_error("unexpected end of line");
        else
        {
            elem = match(ch);
            if(elem != mem.top())
                return make_pair(1, score(elem));
            else
                mem.pop();
        }
    }
    return make_pair(2, auto_score(mem));
}

vector<pair<num_score, num_score>> map_parse(const vector<string>& lines)
{
    vector<pair<num_score, num_score>> res(lines.size());
    size_t i = 0;
    for(auto const& line: lines)
        res[i++] = parse(line);
    return res;
}

num_score day10_p1(const vector<pair<num_score, num_score>>& p)
{
    return accumulate(cbegin(p), cend(p), 0, [](num_score acc, auto const& res){
        return res.first == 1 ? acc + res.second : acc;
    });
}

num_score day10_p2(const vector<pair<num_score, num_score>>& p)
{
    vector<num_score> marks;
    for(auto const& res: p)
        if(res.first == 2)
            marks.push_back(res.second);
    sort(begin(marks), end(marks));
    return marks[marks.size() >> 1];
}

}

void aoc2021::day10()
{
#if DEV
    auto sample = Utils{"data/day10/sample.txt"}.read_lines();
    vector<pair<num_score, num_score>> parsed_sample = map_parse(sample);
    assert(day10_p1(parsed_sample) == 26397);
    assert(day10_p2(parsed_sample) == 288957);
#endif

    auto input = Utils{"data/day10/input.txt"}.read_lines();
    vector<pair<num_score, num_score>> parsed_input = map_parse(input);
    assert(day10_p1(parsed_input) == 268845);
    assert(day10_p2(parsed_input) == 4038824534);
}

//******************************************************************************
