#include "Utils/NetClient.hpp"
#include "Utils/Packable.hpp"

// TCP/IP sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

// allow printing of system errors
#include <cstdio>
#include <errno.h>

#include <iostream>
#include <sstream>
#include <cstring>

namespace Utils {

NetClient::NetClient(void) :
    m_isValid(false),
    m_targetIP("127.0.0.1"),
    m_portNo(666),
    m_fd(-1)
{

}

NetClient::~NetClient(void)
{
    close(m_fd);
}


bool NetClient::connect_to_server(const std::string& addr, unsigned short portNo)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *results = nullptr;
    std::ostringstream port;
    port << portNo;
    int status = getaddrinfo(addr.c_str(), (port.str()).c_str(), &hints, &results);
    if(status != 0) {
        std::cerr << "Error opening connection to " << addr << ":" << portNo << " - " << gai_strerror(status) << std::endl;
        std::cerr << "Fix it\n";
        return false;
    }

    // get the IPv4 address from the result
    struct addrinfo *v4Addr = nullptr;
    for(struct addrinfo *curInfo = results; curInfo != nullptr; curInfo = curInfo->ai_next) {
        if (curInfo->ai_family == AF_INET) { // IPv4
            v4Addr = curInfo;
            break;
        }
    }

    if(v4Addr == nullptr) {
        std::cerr << "Couldn't find IPv4 address!\n" << std::endl;
        freeaddrinfo(results);
        return false;
    }

    // open the socket!
    m_fd = socket(v4Addr->ai_family, v4Addr->ai_socktype, v4Addr->ai_protocol);
    if(m_fd < 0) {
        perror("Unable to open socket!");
        freeaddrinfo(results);
        return false;
    }

    // attempt to connect to the server
    status = connect(m_fd, v4Addr->ai_addr, v4Addr->ai_addrlen);
    if(status < 0) {
        perror("Could not establish connection!");
    }
    freeaddrinfo(results);
    m_targetIP = addr;
    m_portNo = portNo;
    m_isValid = true;
    return true;

}

std::string NetClient::get_destination(void)
{
    std::ostringstream oss;
    oss << m_targetIP << ":" << m_portNo;
    return oss.str();
}

bool NetClient::send(const Utils::Packable& data)
{
    if(!m_isValid) {
        return false;
    }

    std::vector<uint8_t> bytes;
    data.pack(bytes);

    int bytesToSend = bytes.size();
    int btsNet = htonl(bytesToSend);
    int numBytes = ::send(m_fd, &btsNet, sizeof(int), 0);
    if(numBytes <= 0) {
        m_isValid = false;
        return m_isValid;
    }

    int bytePos = 0;
    while(bytesToSend > 0) {
        int numBytes = ::send(m_fd, reinterpret_cast<const void*>(&(bytes[bytePos])), bytesToSend, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection receiver!!!!\n";
            m_isValid = false;
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (NetClient::send)");
            m_isValid = false;
            return false;
        }
        bytesToSend -= numBytes;
        bytePos += numBytes;
    }

    if(bytesToSend == 0) {
        return true;
    }
    std::cerr << "Sent more bytes than I had to send... WTF?\n";
    return false;

}

bool NetClient::send(const char *str)
{
    if(!m_isValid || str == nullptr) {
        return false;
    }

    int bytesToSend = strlen(str);
    int btsNet = htonl(bytesToSend);
    int numBytes = ::send(m_fd, &btsNet, sizeof(int), 0);
    int bytePos = 0;
    while(bytesToSend > 0) {
        numBytes = ::send(m_fd, reinterpret_cast<const void*>(&(str[bytePos])), bytesToSend, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection receiver!!!!\n";
            m_isValid = false;
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (NetClient::send)");
            m_isValid = false;
            return false;
        }
        bytesToSend -= numBytes;
        bytePos += numBytes;
    }

    if(bytesToSend == 0) {
        return true;
    }
    std::cerr << "Sent more bytes than I had to send... WTF?\n";
    return false;

}

bool NetClient::receive(std::vector<char>& buf)
{
    if(!m_isValid) {
        return false;
    }

    int bytesToRecv;
    int btrNet;
    int numBytes = ::recv(m_fd, &btrNet, sizeof(int), 0);
    if(numBytes <= 0) {
        m_isValid = false;
        return m_isValid;
    }

    bytesToRecv = ntohl(btrNet);
    buf.clear();
    buf.resize(bytesToRecv);

    int pos = 0;
    while(pos < bytesToRecv) {
        numBytes = ::recv(m_fd, reinterpret_cast<void*>(&(buf[pos])), bytesToRecv-pos, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection to sender!!!!\n";
            m_isValid = false;
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (NetClient::receive)");
            m_isValid = false;
            return false;
        }
        pos += numBytes;
    }
    return true;
}

bool NetClient::receive(Utils::Packable& data)
{
    if(!m_isValid) {
        return false;
    }

    int btsNet = 0;
    int numBytes = ::recv(m_fd, &btsNet, sizeof(int), 0);
    if(numBytes <= 0) {
        m_isValid = false;
        return m_isValid;
    }

    int bytesToRecv = ntohl(btsNet);

    std::vector<uint8_t> bytes;
    bytes.resize(bytesToRecv, 0);
    int bytePos = 0;
    while(bytePos < bytesToRecv) {
        numBytes = ::recv(m_fd, &(bytes[bytePos]), bytesToRecv-bytePos, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection receiver!!!!\n";
            m_isValid = false;
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (NetClient::send)");
            m_isValid = false;
            return false;
        }
        bytePos += numBytes;
    }
    
    data.unpack(bytes);
    return true;
}


} // end namespace Utils
