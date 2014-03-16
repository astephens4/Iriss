#include "Utils/Packable.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>

namespace Utils {

union Float_t {
    Float_t(float val) : f(val)
    {
    }

    float f;
    uint32_t i;
};

bool Packable::send(int socket) const
{
    if(socket < 0) {
        std::cerr << "Given socket is invalid!\n";
        return false;
    }

    std::vector<uint8_t> bytes;
    pack(bytes);

    int bytesToSend = bytes.size();
    while(bytesToSend > 0) {
        int numBytes = ::send(socket, &(bytes[0]), bytesToSend, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection receiver!!!!\n";
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (Packable::send)");
            return false;
        }
        bytesToSend -= numBytes;
    }

    if(bytesToSend == 0) {
        return true;
    }
    return false;
}

bool Packable::receive(int socket)
{
    if(socket < 0) {
        std::cerr << "Given socket is invalid!\n";
        return false;
    }

    std::vector<uint8_t> bytes;
    unsigned char messageSize;
    recv(socket, &messageSize, 1, 0);
    bytes.reserve(messageSize);

    uint32_t pos = 0;
    while(pos < messageSize) {
        int numBytes = ::recv(socket, &(bytes[pos]), messageSize-pos, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection to sender!!!!\n";
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (Packable::receive)");
            return false;
        }
        pos += numBytes;
    }

    unpack(bytes);
    return true;
}

void Packable::pack_uint32(uint32_t host, uint8_t *data) const
{
    uint32_t net = htonl(host);
    data[0] = static_cast<uint8_t>((net&0xFF000000) >> 24);
    data[1] = static_cast<uint8_t>((net&0x00FF0000) >> 16);
    data[2] = static_cast<uint8_t>((net&0x0000FF00) >>  8);
    data[3] = static_cast<uint8_t>((net&0x000000FF));
}

void Packable::pack_int32(int32_t host, uint8_t *data) const
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
void Packable::pack_float32(float host, uint8_t *data) const
{
    Float_t hostf(host);
    data[0] = static_cast<uint8_t>((hostf.i&0xFF000000) >> 24);
    data[1] = static_cast<uint8_t>((hostf.i&0x00FF0000) >> 16);
    data[2] = static_cast<uint8_t>((hostf.i&0x0000FF00) >>  8);
    data[3] = static_cast<uint8_t>((hostf.i&0x000000FF));
    return;
}

void Packable::pack_float64(double host, uint8_t *data) const
{
    assert(0 && "Not implemented yet!" && host && data);
}


void Packable::unpack_uint32(const uint8_t *data, uint32_t& host) const
{
    uint32_t net = 0;
    net |= data[0] << 24;
    net |= data[1] << 16;
    net |= data[2] <<  8;
    net |= data[3];
    host = ntohl(net);
}

void Packable::unpack_int32(const uint8_t *data, int32_t& host) const
{
    int32_t net = 0;
    net |= data[0] << 24;
    net |= data[1] << 16;
    net |= data[2] <<  8;
    net |= data[3];
    host = ntohl(net);

}

void Packable::unpack_float32(const uint8_t *data, float& host) const
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

void Packable::unpack_float64(const uint8_t *data, double& host) const
{
    assert(0 && "Not implemented yet!" && host && data);
}


} // end namespace Utils
