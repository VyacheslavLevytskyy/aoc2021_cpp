/*
 *
 */

#include <variant>

#include "utils.h"

using namespace aoc2021;

namespace
{

inline
uint8_t char_to_oct(char ch)
{
    return ch < 'A' ? (ch - '0') : (ch - 55);
}

inline
uint8_t hex_to_byte(char hi, char lo)
{
    return (char_to_oct(hi) << 4) | char_to_oct(lo);
}

class Input
{
    vector<uint8_t> m_data;
    size_t m_pos = 0;

    static void set_offset(size_t pos, size_t& byte_no, size_t& prefix);
    template <typename T = uint16_t> requires integral<T> T read_bits(size_t n);

public:
    Input(const string& s);

    struct Packet
    {
        uint8_t ver;
        uint8_t id;
        variant<uint64_t, vector<Packet>> data;

        unsigned sum_ver() const;
        uint64_t calculate() const;
    };

    Packet parse();
};

Input::Input(const string& s)
{
    size_t sz = s.size();
    if(sz < 1)
        throw invalid_argument("unexpected task format");
    m_data.reserve((sz >> 1) + 1);
    for(size_t i = 1; i <= sz; i += 2)
        m_data.push_back(hex_to_byte(s[i - 1], i < sz ? s[i] : '0'));
}

void Input::set_offset(size_t pos, size_t& byte_no, size_t& prefix)
{
    byte_no = pos >> 3;
    prefix = pos % 8;
}

template <typename T> requires integral<T> T
Input::read_bits(size_t n)
{
    if(n < 1 || n > 16)
        throw logic_error("read_bits() argument is too big");

    size_t i1, j1, i2, j2;
    set_offset(m_pos, i1, j1);
    m_pos += n;
    set_offset(m_pos - 1, i2, j2);
    if(i2 >= m_data.size())
        throw out_of_range("read_bits(): out of range");

    uint32_t v = 0;
    for(size_t i = i1; i <= i2; i++)
        v = (v << 8) | m_data[i];
    v >>= 7 - j2;
    v &= (1 << n) - 1;

    return static_cast<T>(v);
}

Input::Packet Input::parse()
{
    Packet packet;
    packet.ver = read_bits<uint8_t>(3);
    packet.id = read_bits<uint8_t>(3);
    if(packet.id == 4)
    {
        uint64_t value = 0;
        for(;;)
        {
            uint8_t oct = read_bits<uint8_t>(5);
            value = (value << 4) + (oct & 0x0F);
            if((oct & 0x10) == 0)
                break;
        }
        packet.data = value;
    }
    else
    {
        vector<Packet> sub;
        uint8_t is_num = read_bits<uint8_t>(1);
        if(is_num)
        {
            uint16_t num = read_bits<uint16_t>(11);
            sub.reserve(num);
            for(uint16_t i = 0; i < num; i++)
                sub.push_back(parse());
        }
        else
        {
            uint16_t sz = read_bits<uint16_t>(15);
            size_t pos0 = m_pos;
            while(m_pos - pos0 < sz)
                sub.push_back(parse());
            if(m_pos - pos0 != sz)
                throw out_of_range("parse(): out of range");
        }
        packet.data = move(sub);
    }
    return packet;
}

unsigned Input::Packet::sum_ver() const
{
    unsigned nested = 0;
    if(!holds_alternative<uint64_t>(data))
    {
        const vector<Packet>& l = get<1>(data);
        nested = accumulate(cbegin(l), cend(l), 0, [](unsigned acc, const Packet& p){
            return acc + p.sum_ver();
        });
    }
    return ver + nested;
}

uint64_t Input::Packet::calculate() const
{
    if(id == 4)
        return get<0>(data);
    else
    {
        const vector<Packet>& l = get<1>(data);
        auto p1 = cbegin(l), p2 = cend(l);
        switch(id)
        {
            case 0:
                return accumulate(p1, p2, 0ULL, [](uint64_t acc, const Packet& p){
                    return acc + p.calculate();
                });
            case 1:
                return accumulate(p1, p2, 1ULL, [](uint64_t acc, const Packet& p){
                    return acc * p.calculate();
                });
            case 2:
                return accumulate(p1 + 1, p2, p1->calculate(), [](uint64_t acc, const Packet& p){
                    return min(acc, p.calculate());
                });
            case 3:
                return accumulate(p1 + 1, p2, p1->calculate(), [](uint64_t acc, const Packet& p){
                    return max(acc, p.calculate());
                });
            case 5:
                if(l.size() != 2)
                    throw invalid_argument("unexpected packet children size (id=5)");
                return l[0].calculate() > l[1].calculate() ? 1ULL : 0ULL;
            case 6:
                if(l.size() != 2)
                    throw invalid_argument("unexpected packet children size (id=6)");
                return l[0].calculate() < l[1].calculate() ? 1ULL : 0ULL;
            case 7:
                if(l.size() != 2)
                    throw invalid_argument("unexpected packet children size (id=7)");
                return l[0].calculate() == l[1].calculate() ? 1ULL : 0ULL;
            default:
                throw invalid_argument("unexpected packet id");
        }
    }

    return 0;
}

//******************************************************************************

unsigned day16_p1(const string& s)
{
    Input in(s);
    Input::Packet root = in.parse();
    return root.sum_ver();
}

uint64_t day16_p2(const string& s)
{
    Input in(s);
    Input::Packet root = in.parse();
    return root.calculate();
}

}

void aoc2021::day16()
{
#if DEV
    assert(day16_p1("8A004A801A8002F478") == 16);
    assert(day16_p1("A0016C880162017C3686B18A3D4780") == 31);
    assert(day16_p2("9C0141080250320F1802104A08") == 1);
#endif

    auto input = Utils{"data/day16/input.txt"}.read_lines();
    auto const& str = input[0];
    assert(day16_p1(str) == 1014);
    assert(day16_p2(str) == 1922490999789ULL);
}

//******************************************************************************
