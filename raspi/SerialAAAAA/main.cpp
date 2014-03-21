#include "Utils/SerialPeer.hpp"

int main(void)
{
    std::vector<uint8_t> alist;
    alist.resize(10, 0xA);

    Utils::SerialPeer::Settings settings;
    settings.mode = Utils::SerialPeer::UART_8N1;
    settings.flow = Utils::SerialPeer::UART_NO_FLOW_CONTROL;
    settings.speed = Utils::SerialPeer::BAUD_115200;

    Utils::SerialPeer uart("/dev/ttyAMA0", settings);

    while(1) {
        uart.send(alist);
    }
}
