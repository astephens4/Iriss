#include "Packable.hpp"

namespace Utils {

union Float_t {
    Float_t(float val) : f(val)
    {
    }

    float f;
    uint32_t i;
};

/**
 * the ATMega2560 is little endian. Woot.
 * Network is big endian. Wooter.
 */
uint32_t AVR_htonl(uint32_t host)
{
    return ((host&0x000000FF)<<24) |
           ((host&0x0000FF00)<<8)  |
           ((host&0x00FF0000)>>8)  |
           ((host&0xFF000000)>>24);
}
#define htonl AVR_htonl

uint32_t AVR_htons(uint16_t host)
{
    return ((host&0x00FF)<<8) |
           ((host&0xFF00)>>8);
}
#define htons AVR_htons

uint32_t AVR_ntohl(uint32_t net)
{
    return ((net&0x000000FF)<<24) |
           ((net&0x0000FF00)<<8)  |
           ((net&0x00FF0000)>>8)  |
           ((net&0xFF000000)>>24);

}
#define ntohl AVR_ntohl

uint32_t AVR_ntohs(uint16_t net)
{
    return ((net&0x00FF)<<8) |
           ((net&0xFF00)>>8);
}
#define ntohs AVR_ntohs

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

    data[0] = static_cast<uint8_t>((hostf.i&0x000000FF));
    data[1] = static_cast<uint8_t>((hostf.i&0x0000FF00) >>  8);
    data[2] = static_cast<uint8_t>((hostf.i&0x00FF0000) >> 16);
    data[3] = static_cast<uint8_t>((hostf.i&0xFF000000) >> 24);
    return;
}

void Packable::pack_float64(double host, uint8_t *data)
{
    data[0] = '\0';
    data[1] = '\0';
    data[2] = '\0';
    data[3] = '\0';
}


void Packable::unpack_uint32(const uint8_t *data, uint32_t& host)
{
    uint32_t net = 0;
    net |= static_cast<uint32_t>(data[0]) << 24;
    net |= static_cast<uint32_t>(data[1]) << 16;
    net |= static_cast<uint32_t>(data[2]) <<  8;
    net |= static_cast<uint32_t>(data[3]);
    host = ntohl(net);
}

void Packable::unpack_int32(const uint8_t *data, int32_t& host)
{
    int32_t net = 0;
    net |= static_cast<uint32_t>(data[0]) << 24;
    net |= static_cast<uint32_t>(data[1]) << 16;
    net |= static_cast<uint32_t>(data[2]) <<  8;
    net |= static_cast<uint32_t>(data[3]);
    host = ntohl(net);

}

void Packable::unpack_float32(const uint8_t *data, float& host)
{
    Float_t hostf(0.0f);
    hostf.i = 0;
    hostf.i |= static_cast<uint32_t>(data[3]) << 24;
    hostf.i |= static_cast<uint32_t>(data[2]) << 16;
    hostf.i |= static_cast<uint32_t>(data[1]) <<  8;
    hostf.i |= static_cast<uint32_t>(data[0]);
    host = hostf.f;
    return;
}

void Packable::unpack_float64(const uint8_t *data, double& host)
{
    host = 1.0;
}

#undef htonl
#undef htons
#undef ntohl
#undef ntohs

} // end namespace Utils


