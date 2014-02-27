#include "Color.hpp"

namespace Utils {

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
