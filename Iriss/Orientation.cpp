#include "Iriss/Orientation.hpp"
#include <cassert>
#include <iostream>

namespace Iriss {

Orientation::Orientation(float r, float p, float y) :
    roll(r),
    pitch(p),
    yaw(y)
{

}

void Orientation::pack(std::vector<uint8_t>& bytes) const
{
    bytes.clear();
    bytes.resize(3*sizeof(float)+1);
    std::cout << "Orientation size: " << bytes.size() << std::endl;
    bytes[0] = ORNTMSG;
    pack_float32(roll, &(bytes[1]));
    pack_float32(pitch, &(bytes[5]));
    pack_float32(yaw, &(bytes[9]));
    return;
}

void Orientation::unpack(const std::vector<uint8_t>& bytes)
{
    std::cout << "Orientation size: " << bytes.size() << std::endl;
    assert(bytes.size() >= 3*sizeof(float)+1);
    assert(bytes[0] == ORNTMSG);
    unpack_float32(&(bytes[1]), roll);
    unpack_float32(&(bytes[5]), pitch);
    unpack_float32(&(bytes[9]), yaw);
    return;
}

} // end namespace Iriss
