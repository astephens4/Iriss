#include "Utils/SerialPeer.hpp"
#include "Utils/Packable.hpp"

// File descriptor operations
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// htonl!
#include <arpa/inet.h>

// errno and stuff
#include <cstdio>
#include <errno.h>

#include <cinttypes>


namespace Utils {

SerialPeer::SerialPeer(const std::string& serFile, const SerialPeer::Settings& settings) :
    m_isValid(false),
    m_uartFd()
{
    // connect to the ArduPilot
    m_uartFd = ::open(serFile.c_str(), O_RDWR | O_NOCTTY);
    if(m_uartFd < 0) {
        perror("open serial file");
    }
    else {
        struct termios uartSettings;
        tcgetattr(m_uartFd, &uartSettings);

        // set the speed of the connection
        cfsetispeed(&uartSettings, settings.speed);
        cfsetospeed(&uartSettings, settings.speed);

        // make the port readable and local
        uartSettings.c_cflag |= (CLOCAL | CREAD);

        // set corresponding mode bits
        switch(settings.mode) {
        case UART_8N1:
            uartSettings.c_iflag &= ~(INPCK | ISTRIP);
            uartSettings.c_cflag &= ~PARENB;
            uartSettings.c_cflag &= ~CSTOPB;
            uartSettings.c_cflag &= ~CSIZE;
            uartSettings.c_cflag |= CS8;
            break;
        case UART_7E1:
            uartSettings.c_iflag |= (INPCK | ISTRIP);
            uartSettings.c_cflag |= PARENB;
            uartSettings.c_cflag &= ~PARODD;
            uartSettings.c_cflag &= ~CSTOPB;
            uartSettings.c_cflag &= ~CSIZE;
            uartSettings.c_cflag |= CS7;
            break;
        case UART_7O1:
            uartSettings.c_iflag |= (INPCK | ISTRIP);
            uartSettings.c_cflag |= PARENB;
            uartSettings.c_cflag |= PARODD;
            uartSettings.c_cflag &= ~CSTOPB;
            uartSettings.c_cflag &= ~CSIZE;
            uartSettings.c_cflag |= CS7;
            break;
        case UART_7M1:
            uartSettings.c_cflag &= ~PARENB;
            uartSettings.c_cflag |= CSTOPB;
            uartSettings.c_cflag &= ~CSIZE;
            uartSettings.c_cflag |= CS8;
            break;
        case UART_7S1:
            uartSettings.c_cflag &= ~PARENB;
            uartSettings.c_cflag &= ~CSTOPB;
            uartSettings.c_cflag &= ~CSIZE;
            uartSettings.c_cflag |= CS8;
            break;

        }

        // set flow control
        switch(settings.flow) {
        case UART_NO_FLOW_CONTROL:
            uartSettings.c_cflag &= ~CRTSCTS;
            break;
        case UART_HARDWARE:
            uartSettings.c_cflag |= CRTSCTS;
            break;
        case UART_SOFTWARE:
            uartSettings.c_iflag |= (IXON | IXOFF | IXANY);
            break;
        }

        // make the connection raw
        cfmakeraw(&uartSettings);

        if(tcsetattr(m_uartFd, TCSANOW, &uartSettings) < 0) {
            perror("Setting serial attributes");
        }
        else {
            m_isValid = true;
        }

    }

}

SerialPeer::~SerialPeer(void)
{
    ::close(m_uartFd);
}

bool SerialPeer::send(const char *str)
{
    if(!m_isValid) return false;

    int len = strlen(str);

    // write the number of bytes to be sent
    int lenNet = htonl(len);
    ssize_t ret = write(m_uartFd, &lenNet, sizeof(int));
    if(ret < 0) {
        perror("send string");
        m_isValid = false;
        return m_isValid;
    }

    // write the bytes out of the terminal
    int bytesSent = 0;
    while(bytesSent < len) {
        ret = write(m_uartFd, str, len);
        if(ret < 0) {
            perror("send string");
            m_isValid = false;
            return m_isValid;
        }
        bytesSent += ret;
    }

    // write the EOL characters out of the terminal
    char EOL[3] = "\r\n";
    ret = write(m_uartFd, EOL, 2);
    if(ret < 0) {
        perror("send string");
        m_isValid = false;
        return m_isValid;
    }

    return m_isValid;
}

bool SerialPeer::send(const std::vector<uint8_t>& bytes)
{
    if(!m_isValid) return false;

    int len = bytes.size();

    // write the number of bytes to be sent
    int lenNet = htonl(len);
    ssize_t ret = write(m_uartFd, &lenNet, sizeof(int));
    if(ret < 0) {
        perror("send bytes");
        m_isValid = false;
        return m_isValid;
    }

    // write the bytes out of the terminal
    int bytesSent = 0;
    while(bytesSent < len) {
        ret = write(m_uartFd, &(bytes[0]), len);
        if(ret < 0) {
            perror("send string");
            m_isValid = false;
            return m_isValid;
        }
        bytesSent += ret;
    }

    // write the EOL characters out of the terminal
    char EOL[3] = "\r\n";
    ret = write(m_uartFd, EOL, 2);
    if(ret < 0) {
        perror("send string");
        m_isValid = false;
        return m_isValid;
    }

    return m_isValid;
}

bool SerialPeer::send(const Utils::Packable& data)
{
    if(!m_isValid) return false;

    std::vector<uint8_t> bytes;
    data.pack(bytes);
    return this->send(bytes);

}

/**
 * Raw mode only receives one byte at a time. Need a special marker for
 * begin message.
 * Propose: 128 <- This will not coincide with ASCII traffic and fits in
 *                  an unsigned char. 
 */
bool SerialPeer::recv(std::vector<uint8_t>& bytes)
{
    if(!m_isValid) return false;

    unsigned char byte;
    // read a byte until the start byte is found
    while(read(m_uartFd, &byte, 1) != -1) {
        if(byte == UART_BEGIN_MESSAGE) {
            break;
        }
    }
    printf("Got message begin!\n");

    // read the message size
    int lenNet = 0;
    ssize_t ret;
    for(int i = 0; i < 4; ++i) {
        ret = read(m_uartFd, &byte, 1);
        printf("Byte %d of length: %hhu\n", i, byte);
        if(ret < 0) {
            perror("read bytes");
            m_isValid = false;
            return m_isValid;
        }

        lenNet |= static_cast<int>(byte)<<((3-i)*8);
    }
    int len = lenNet;
    printf("The message is %d bytes long!\n", len);

    bytes.resize(len, 0);
    int bytesRecvd = 0;
    while(bytesRecvd < len) {
        ret = read(m_uartFd, &(bytes[bytesRecvd]), 1);
        if(ret < 0) {
            perror("read bytes");
            m_isValid = false;
            return m_isValid;
        }
        bytesRecvd += ret;
    }

    return m_isValid;

}

bool SerialPeer::recv(Utils::Packable& data)
{
    std::vector<uint8_t> bytes;
    bool retVal = this->recv(bytes);
    if(retVal) {
        data.unpack(bytes);
    }
    return retVal;
}


} // end namespace Utils
