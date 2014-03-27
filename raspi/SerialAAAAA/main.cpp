#include "Utils/SerialPeer.hpp"
#include "Iriss/Command.hpp"
#include <iostream>

int main(void)
{
    Utils::SerialPeer::Settings settings;
    settings.mode = Utils::SerialPeer::UART_8N1;
    settings.flow = Utils::SerialPeer::UART_NO_FLOW_CONTROL;
    settings.speed = Utils::SerialPeer::BAUD_57600;

    Utils::SerialPeer uart("/dev/ptmx", settings);
    
    // wait to receive an ACK
    Iriss::Command cmd;
    while(1) {
        uart.recv(cmd);

        if(cmd.get() == Iriss::Command::ACK) {
            std::cout << "Got an ACK over serial!\n";
        }
        
        cmd.set(Iriss::Command::ACK);
        uart.send(cmd);
    }

}
