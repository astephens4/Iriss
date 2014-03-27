#ifndef AVR_ORIENTATION_H
#define AVR_ORIENTATION_H

#include "Packable.hpp"

namespace Iriss {

class Orientation : public Utils::Packable {
public:
    static const uint8_t ORNTMSG;
    static const uint32_t PACKED_SIZE; ///< 4 + 4 + 4 + 1 = 13
    float roll;
    float pitch;
    float yaw;

    Orientation();
    Orientation(float r, float p, float y);

    virtual void pack(uint8_t *bytes, uint16_t bufLen) const;
    virtual void unpack(const uint8_t *bytes, uint16_t bufLen);
};

}


#endif // AVR_ORIENTATION_H
