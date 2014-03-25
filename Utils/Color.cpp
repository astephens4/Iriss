#include "Color.hpp"

namespace Utils {

Color::Color(unsigned char r, unsigned char g, unsigned char b) :
    r(r), g(g), b(b)
{
}

Color::Color(uint32_t color)
{
    this->from_uint32(color);
}

unsigned int Color::asUint32(void)
{
    unsigned int ret = r;
    ret <<= 8;
    ret |= g;
    ret <<= 8;
    ret |= b;
    return ret;
}

void Color::from_uint32(uint32_t color)
{
    r = static_cast<unsigned char>((color&0x00FF0000) >> 16);
    g = static_cast<unsigned char>((color&0x0000FF00) >> 8);
    b = static_cast<unsigned char>(color&0x000000FF);
}

}
