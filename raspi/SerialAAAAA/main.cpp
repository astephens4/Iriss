#include "Utils/SerialPeer.hpp"
#include "Iriss/Command.hpp"
#include <iostream>

int main(void)
{
    Utils::SerialPeer::Settings settings;
    settings.mode = Utils::SerialPeer::UART_8N1;
    settings.flow = Utils::SerialPeer::UART_NO_FLOW_CONTROL;
    settings.speed = Utils::SerialPeer::BAUD_115200;

    Utils::SerialPeer uart("/dev/pts/8", settings);
     
    std::vector<unsigned char> str;
    Iriss::Command cmd;
    while(1) {
       std::cout << "Waiting on reception of a command\n";
       uart.recv(str);
       std::cout << "Received something!: " << &(str[0]) << "\n";

       if(cmd.get() == Iriss::Command::ACK) {
           std::cout << "Got an ACK over serial!\n";
       }
        
       std::cout << "Sending an ACK back!\n";
       cmd.set(Iriss::Command::ACK);
       uart.send(cmd);
       std::cout << "Sent an ACK back!\n";
    }
}
