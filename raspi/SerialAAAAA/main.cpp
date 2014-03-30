#include "Utils/SerialPeer.hpp"
#include "Iriss/Command.hpp"
#include <iostream>

int main(void)
{
    Utils::SerialPeer::Settings settings;
    settings.mode = Utils::SerialPeer::UART_8N1;
    settings.flow = Utils::SerialPeer::UART_NO_FLOW_CONTROL;
    settings.speed = Utils::SerialPeer::BAUD_57600;

    Utils::SerialPeer uart("/dev/ttyAMA0", settings);
  
    std::vector<unsigned char> sendThis;
    sendThis.push_back(0xFFu);
    
    // wait to receive an ACK
    Iriss::Command cmd(Iriss::Command::ACK);
    std::cout << "Sending ACK\n";
    uart.send(cmd);
    std::cout << "ACK sent\n";
    while(1) {
        std::cout << "Waiting on reception of a command\n";
        uart.recv(cmd);
        std::cout << "Received something!\n";

        if(cmd.get() == Iriss::Command::ACK) {
            std::cout << "Got an ACK over serial!\n";
        }
        
        std::cout << "Sending an ACK back!\n";
        cmd.set(Iriss::Command::ACK);
        uart.send(cmd);
        std::cout << "Sent an ACK back!\n";
    }
}
