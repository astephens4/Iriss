#ifndef IRISS_COLOR_H
#define IRISS_COLOR_H 1

#include <cinttypes>

namespace Utils {

/**
 * Represent a 3-component color
 */
class Color {
public: 
    unsigned char r;
    unsigned char g;
    unsigned char b;
    
    Color(unsigned char r, unsigned char g, unsigned char b);
    Color(uint32_t color);
    /**
     * Get the color as a 32-bit integer, with the least significant
     * 24 bits storing each color component in the order rgb
     */
    unsigned int asUint32(void);

    /**
     * Read a RGB 32-bit unsigned integer into this color
     * @param [in] color Color stored as an integer in 32-bits, with the MSB clear.
     */
    void from_uint32(uint32_t color);
};

}

#endif // IRISS_COLOR_H
