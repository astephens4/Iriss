#include "Orientation.hpp"

namespace Iriss {

const uint8_t Orientation::ORNTMSG = 3;
const uint32_t Orientation::PACKED_SIZE = 13;

Orientation::Orientation() :
    roll(0.0f),
    pitch(0.0f),
    yaw(0.0f)
{

}

Orientation::Orientation(float r, float p, float y) :
    roll(r),
    pitch(p),
    yaw(y)
{

}

void Orientation::pack(uint8_t *bytes, uint16_t bufLen) const
{
    if(bufLen < PACKED_SIZE) {
        return;
    }
    bytes[0] = ORNTMSG;
    pack_float32(roll, &(bytes[1]));
    pack_float32(pitch, &(bytes[5]));
    pack_float32(yaw, &(bytes[9]));
    return;
}

void Orientation::unpack(const uint8_t *bytes, uint16_t bufLen)
{
    if(bufLen < PACKED_SIZE) {
        return;
    }
    unpack_float32(&(bytes[1]), roll);
    unpack_float32(&(bytes[5]), pitch);
    unpack_float32(&(bytes[9]), yaw);
    return;
}

} // end namespace Iriss

