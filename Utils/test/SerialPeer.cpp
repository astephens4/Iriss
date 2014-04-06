#include "Utils/SerialPeer.hpp"
#include <iostream>

int main(void)
{
    Utils::SerialPeer::Settings settings;
    settings.mode = Utils::SerialPeer::UART_8N1;
    settings.speed = Utils::SerialPeer::BAUD_115200;
    settings.flow = Utils::SerialPeer::UART_HARDWARE;
    settings.blocking = Utils::SerialPeer::UART_FULL_BLOCK;
    Utils::SerialPeer usb("/dev/ttyUSB0", settings);

    std::vector<uint8_t> recvd;
    usb.recv(recvd);

    recvd.push_back('\0');
    std::cout << "Got from serial " << &(recvd[0]) << std::endl;

    usb.send("Hi Friend!\n");

    usb.recv(recvd);

    recvd.push_back('\0');
    std::cout << "Got from serial " << &(recvd[0]) << std::endl;

    return 0;
}
