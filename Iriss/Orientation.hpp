#ifndef IRISS_ORIENTATION_H
#define IRISS_ORIENTATION_H 1

#include "Utils/Packable.hpp"

namespace Iriss {

class Orientation : public Utils::Packable {
public:
    static const unsigned char ORNTMSG = 3;
    float roll;
    float pitch;
    float yaw;

    Orientation(float r, float p, float y);

    virtual void pack(std::vector<uint8_t>& bytes) const;
    virtual void unpack(const std::vector<uint8_t>& bytes);
};

};

#endif // IRISS_ORIENTATION_H
