/*
 *
 */

#include "utils.h"

using aoc2021::Utils;

namespace
{

constexpr static size_t NUM = 100, N = 5;

class World
{
private:
    vector<int8_t> m_numbers;
    size_t m_boards_n = 0;
    array<vector<vector<int8_t>>, NUM> m_values;
    vector<array<int8_t, 2*N>> m_marked;
    vector<bool> m_winners;

    bool mark(size_t board_no, int8_t pos);

public:
    World(const vector<string>& lines);
    const vector<int8_t>& get_numbers() const;
    size_t get_boards_number() const;

    vector<size_t> draw(unsigned v);
    unsigned score(size_t board_no);
};

World::World(const vector<string>& lines)
{
    m_numbers = Utils::read_int_line<int8_t>(lines[0]);

    size_t i, j, pos;
    for(i = 2; i < lines.size(); i += N + 1)
    {
        vector<int8_t> ns;
        for(j = 0; j < N; j++)
            for(pos = 0; pos <= (N - 1) * 3; pos += 3)
                ns.push_back(stoi(lines[i + j].substr(pos)));

        int8_t value;
        for(j = 0; j < N*N; j++)
        {
            value = ns[j];
            if(m_values[value].size() > m_boards_n)
                m_values[value][m_boards_n].push_back(j);
            else
            {
                m_values[value].resize(1 + m_boards_n);
                m_values[value][m_boards_n].push_back(j);
            }
        }
        m_boards_n++;
    }

    m_marked.resize(m_boards_n, array<int8_t, 2*N>{});
    m_winners.resize(m_boards_n, false);
}

inline
const vector<int8_t>& World::get_numbers() const
{
    return m_numbers;
}

inline
size_t World::get_boards_number() const
{
    return m_boards_n;
}

bool World::mark(size_t board_no, int8_t pos)
{
    size_t c = pos % N, r = N + pos / N;
    auto& m = m_marked[board_no];
    if(m[c] == N - 1 || m[r] == N - 1)
        return true;
    else
    {
        m[c]++;
        m[r]++;
        return false;
    }
}

vector<size_t> World::draw(unsigned v)
{
    vector<vector<int8_t>>& boards = m_values[v];
    size_t board_no = 0;
    vector<size_t> won;
    for(const vector<int8_t>& board: boards)
    {
        if(!m_winners[board_no])
        {
            for(auto pos: board)
            {
                if(mark(board_no, pos))
                {
                    m_winners[board_no] = true;
                    won.push_back(board_no);
                    break;
                }
            }
        }
        board_no++;
    }
    boards.clear();
    return won;
}

unsigned World::score(size_t board_no)
{
    unsigned sum = 0, value = 0;
    for(auto boards: m_values)
    {
        if(board_no < boards.size())
            sum += (value * boards[board_no].size());
        value++;
    }
    return sum;
}

//******************************************************************************

unsigned day04_p1(World w)
{
    for(auto v: w.get_numbers())
    {
        vector<size_t> won = w.draw(v);
        if(won.size() > 0)
            return v * w.score(won[0]);
    }
    return 0;
}

unsigned day04_p2(World w)
{
    unsigned n_winners = 0;
    for(auto v: w.get_numbers())
    {
        vector<size_t> won = w.draw(v);
        n_winners += won.size();
        if(n_winners == w.get_boards_number())
            return v * w.score(won[won.size() - 1]);
    }
    return 0;
}

}

void aoc2021::day04()
{
#if DEV
    World sample{Utils{"data/day04/sample.txt"}.read_lines()};
    assert(day04_p1(sample) == 4512);
    assert(day04_p2(sample) == 1924);
#endif

    World input{Utils{"data/day04/input.txt"}.read_lines()};
    assert(day04_p1(input) == 38913);
    assert(day04_p2(input) == 16836);
}

//******************************************************************************
