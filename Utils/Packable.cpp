#include "Utils/Packable.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

namespace Utils {

union Float_t {
    Float_t(float val) : f(val)
    {
    }

    float f;
    uint32_t i;
};

void Packable::pack_uint32(uint32_t host, uint8_t *data)
{
    uint32_t net = htonl(host);
    data[0] = static_cast<uint8_t>((net&0xFF000000) >> 24);
    data[1] = static_cast<uint8_t>((net&0x00FF0000) >> 16);
    data[2] = static_cast<uint8_t>((net&0x0000FF00) >>  8);
    data[3] = static_cast<uint8_t>((net&0x000000FF));
}

void Packable::pack_int32(int32_t host, uint8_t *data)
{
    int32_t net = htonl(host);
    data[0] = static_cast<uint8_t>((net&0xFF000000) >> 24);
    data[1] = static_cast<uint8_t>((net&0x00FF0000) >> 16);
    data[2] = static_cast<uint8_t>((net&0x0000FF00) >>  8);
    data[3] = static_cast<uint8_t>((net&0x000000FF));

}

/**
 * NOTE: Only works on systems conforming to IEEE 32-bit floating point standard
 */
void Packable::pack_float32(float host, uint8_t *data)
{
    Float_t hostf(host);
    data[0] = static_cast<uint8_t>((hostf.i&0xFF000000) >> 24);
    data[1] = static_cast<uint8_t>((hostf.i&0x00FF0000) >> 16);
    data[2] = static_cast<uint8_t>((hostf.i&0x0000FF00) >>  8);
    data[3] = static_cast<uint8_t>((hostf.i&0x000000FF));
    return;
}

void Packable::pack_float64(double host, uint8_t *data)
{
    assert(0 && "Not implemented yet!" && host && data);
}


void Packable::unpack_uint32(const uint8_t *data, uint32_t& host)
{
    uint32_t net = 0;
    net |= data[0] << 24;
    net |= data[1] << 16;
    net |= data[2] <<  8;
    net |= data[3];
    host = ntohl(net);
}

void Packable::unpack_int32(const uint8_t *data, int32_t& host)
{
    int32_t net = 0;
    net |= data[0] << 24;
    net |= data[1] << 16;
    net |= data[2] <<  8;
    net |= data[3];
    host = ntohl(net);

}

void Packable::unpack_float32(const uint8_t *data, float& host)
{
    Float_t hostf(0.0f);
    hostf.i = 0;
    hostf.i |= data[0] << 24;
    hostf.i |= data[1] << 16;
    hostf.i |= data[2] <<  8;
    hostf.i |= data[3];
    host = hostf.f;
    return;
}

void Packable::unpack_float64(const uint8_t *data, double& host)
{
    assert(0 && "Not implemented yet!" && host && data);
}


} // end namespace Utils
