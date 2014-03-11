#include "Color.hpp"

namespace Utils {

Color::Color(unsigned char r, unsigned char g, unsigned char b) :
    r(r), g(g), b(b)
{
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

}
