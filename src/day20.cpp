/*
 *
 */

#include "utils.h"

using namespace aoc2021;

namespace
{

class World
{
    bitset<512> m_mask;
    bool m_b0;
    vector<uint8_t> m_image;
    int m_sizex, m_sizey, m_x1, m_x2, m_y1, m_y2;
    int m_n;

    uint8_t get_bit(int x0, int y0, uint8_t def);
    void set_bit(int x0, int y0, uint8_t v, vector<uint8_t>& image);
    void calc_offset(int x0, int y0, size_t& byte_no, size_t& prefix);

    size_t get_mask_idx(int x0, int y0, uint8_t def);

public:
    World(const vector<string>& l, int n);
    size_t enhance();
};

World::World(const vector<string>& l, int n) : m_n{n}
{
    const string& s = l[0];
    for(size_t i = 0; i < s.size(); i++)
        if(s[i] == '#')
            m_mask.set(i);
    m_b0 = m_mask.test(0);

    m_x1 = 0;
    m_y1 = 0;
    m_x2 = l[2].size();
    m_y2 = l.size() - 2;
    m_sizex = m_x2 + 2 * n;
    m_sizey = m_y2 + 2 * n;
    m_image.resize(((m_sizex * m_sizey) >> 3) + 1, 0);

    for(int y = 0; y < m_y2; y++)
        for(int x = 0; x < m_x2; x++)
            if(l[y + 2][x] == '#')
                set_bit(x, y, 1, m_image);
}

void World::calc_offset(int x0, int y0, size_t& byte_no, size_t& prefix)
{
    x0 += m_n;
    y0 += m_n;
    size_t pos = y0 * m_sizex + x0;
    byte_no = pos >> 3;
    prefix = pos % 8;
}

uint8_t World::get_bit(int x0, int y0, uint8_t def)
{
    if(x0 < m_x1 || x0 >= m_x2 || y0 < m_y1 || y0 >= m_y2)
        return def;
    size_t byte_no, prefix;
    calc_offset(x0, y0, byte_no, prefix);
    uint8_t bt = m_image[byte_no];
    return (bt & (1 << prefix)) == 0 ? 0 : 1;
}

void World::set_bit(int x0, int y0, uint8_t v, vector<uint8_t>& image)
{
    size_t byte_no, prefix;
    calc_offset(x0, y0, byte_no, prefix);
    uint8_t& bt = image[byte_no];
    if(v == 0)
        bt &= ~(1 << prefix);
    else
        bt |= 1 << prefix;
}

size_t World::get_mask_idx(int x0, int y0, uint8_t def)
{
    size_t idx = 0;
    for(int y = y0 - 1; y <= y0 + 1; y++)
        for(int x = x0 - 1; x <= x0 + 1; x++)
            idx = (idx << 1) + get_bit(x, y, def);
    return idx;
}

size_t World::enhance()
{
    size_t weight = 0;
    uint8_t def;
    vector<uint8_t> image(m_image.size(), 0);
    for(int no = 1; no <= m_n; no++)
    {
        def = (m_b0 && no % 2 == 0 ? 1 : 0);
        weight = 0;
        for(int x = m_x1 - 1; x < m_x2 + 1; x++)
            for(int y = m_y1 - 1; y < m_y2 + 1; y++)
                set_bit(x, y, m_mask.test(get_mask_idx(x, y, def)) ? (++weight, 1) : 0, image);

        m_image = move(image);
        image.clear();
        image.resize(m_image.size(), 0);
        --m_x1, --m_y1;
        ++m_x2, ++m_y2;
    }

    return weight;
}

//******************************************************************************

size_t day20_p1(const vector<string>& l)
{
    return World{l, 2}.enhance();
}

size_t day20_p2(const vector<string>& l)
{
    return World{l, 50}.enhance();
}

}

void aoc2021::day20()
{
#if DEV
    auto sample = Utils{"data/day20/sample.txt"}.read_lines();
    assert(day20_p1(sample) == 35);
    assert(day20_p2(sample) == 3351);
#endif

    auto input = Utils{"data/day20/input.txt"}.read_lines();
    assert(day20_p1(input) == 5583);
    assert(day20_p2(input) == 19592);
}

//******************************************************************************
