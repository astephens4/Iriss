#ifndef IRISS_COLOR_H
#define IRISS_COLOR_H 1

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
    /**
     * Get the color as a 32-bit integer, with the least significant
     * 24 bits storing each color component in the order rgb
     */
    unsigned int asUint32(void);
};

}

#endif // IRISS_COLOR_H
