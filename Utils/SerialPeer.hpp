#ifndef IRISS_SERIAL_PEER_H
#define IRISS_SERIAL_PEER_H 1

#include <termios.h>
#include <string>
#include <cstring>
#include <vector>

namespace Utils {

class Packable;

class SerialPeer {
public:
    const static unsigned char UART_BEGIN_MESSAGE = 128;
    enum Mode {
        UART_8N1,
        UART_7E1,
        UART_7O1,
        UART_7M1,
        UART_7S1
    };

    enum FlowControl {
        UART_NO_FLOW_CONTROL,
        UART_HARDWARE,
        UART_SOFTWARE
    };

    enum Speed {
        BAUD_9600 = B9600,
        BAUD_19200 = B19200,
        BAUD_38400 = B38400,
        BAUD_57600 = B57600,
        BAUD_115200 = B115200,
        BAUD_230400 = B230400,
    };

    struct Settings {
        Speed speed;
        Mode mode;
        FlowControl flow;
    };

    SerialPeer(const std::string& serFile, const SerialPeer::Settings& settings);
    ~SerialPeer(void);

    bool send(const char *str);
    bool send(const std::vector<uint8_t>& bytes);
    bool send(const uint8_t *bytes, uint32_t len);
    bool send(const Utils::Packable& data);

    bool recv(std::vector<uint8_t>& bytes);
    bool recv(uint8_t *bytes, uint32_t len);
    bool recv(Utils::Packable& data);

private:
    bool m_isValid;
    int m_uartFd;
};

} // end namespace Utils
#endif // IRISS_SERIAL_PEER_H
