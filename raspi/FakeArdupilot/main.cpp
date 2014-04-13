#define _XOPEN_SOURCE
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "Utils/SerialPeer.hpp"
#include "Iriss/Command.hpp"
#include "Iriss/Orientation.hpp"
#include <ctime>

Utils::SerialPeer *uart;

bool sendAckBack = false;
Iriss::Command lastSent;

void do_respond(const Iriss::Command& cmd, int16_t *channels)
{
    uint32_t directives = cmd.get();
    if(directives == Iriss::Command::TX_ERR) {
        return;
    }
    
    if(directives & Iriss::Command::ACK) {
        // reply with an ACK if not waiting on one
        if(sendAckBack) {
            Iriss::Command resp(Iriss::Command::ACK);
            uart->send(resp);
            sendAckBack = false;
        }
        else {
            sendAckBack = true;
        }
    }

    if(directives & Iriss::Command::SEND_AGAIN) {
        // send the last command    
        Iriss::Command resp;
        resp.copy(lastSent);
        uart->send(lastSent);
    }
  
    if(directives & Iriss::Command::GET_ORIENTATION) {
        // get the orientation from ins and send it
        float roll = 0.0f,
              pitch = 0.0f,
              yaw = 0.0f;
       
        Iriss::Orientation orientation(roll, pitch, yaw);
        uart->send(orientation);
    }
   
    if(directives & Iriss::Command::NUDGE_ROLL_LEFT) {
        // update channels 0
        --channels[0];
    }
   
    if(directives & Iriss::Command::NUDGE_ROLL_RIGHT) {
        // update channels 0 in the other way from LEFT 
        ++channels[0];
    }
   
    if(directives & Iriss::Command::NUDGE_UP) {
        // update channels 2 
        ++channels[2];
    }
   
    if(directives & Iriss::Command::NUDGE_DOWN) {
        // update channels 2 in the other way from UP 
        --channels[2];
    }
   
    if(directives & Iriss::Command::NUDGE_YAW_CCW) {
        // update channels 3 
        ++channels[3];
    }
   
    if(directives & Iriss::Command::NUDGE_YAW_CW) {
        // update channels 3 in the other way from CCW 
        --channels[3];
    }
   
    if(directives & Iriss::Command::NUDGE_PITCH_DOWN) {
        // update channels 1 
        ++channels[1];
    }
   
    if(directives & Iriss::Command::NUDGE_PITCH_UP) {
        // update channels 1 in the other way from  DOWN
        --channels[1];
    }
}

bool g_verbose = false;
#define PRINT_VERBOSE(output_stream) \
    if(g_verbose) { \
        std::cout << "FakeArduPilot: " << output_stream << std::endl; \
    }

int main(int nargs, char *argv[])
{
    srand(time(NULL));
    std::string serialPort = "/dev/pts/7";
    for(int i = 1; i < nargs; ++i) {
        if((strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "--serial") == 0)) {
            if(i+1 < nargs) {
                ++i;
                serialPort = argv[i];
            }
        }
        else if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            std::cout << "FlightController - Steer a quadcopter according to orders sent from a server\n"
                      << "Options:\n"
                      << " <Option,ShortForm>\t<argument>\t\t\t\t<default>\n"
                      << " --serial,-s\t\t<Serial port going to the ArduPilot>\t/dev/pts/7\n"
                      << " --help,-h\n";
            return -1;
        }
        else if((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--verbose") == 0)) {
            g_verbose = true;
        }
    }


    Utils::SerialPeer::Settings settings;
    settings.speed = Utils::SerialPeer::BAUD_115200;
    settings.mode = Utils::SerialPeer::UART_8N1;
    settings.flow = Utils::SerialPeer::UART_NO_FLOW_CONTROL;
    settings.blocking = Utils::SerialPeer::UART_FULL_BLOCK;
    uart = new Utils::SerialPeer(serialPort, settings);

    int16_t channels[8];
    for(int i = 0; i < 8; ++i)
        channels[i] = 0;

    // wait to receive an ack to know the top level controller is online
    Iriss::Command cmd;
    do { 
        PRINT_VERBOSE("Waiting for FlightController to come online");        
        uart->recv(cmd);
    } while(cmd.get() != Iriss::Command::ACK);

    PRINT_VERBOSE("FlightController online, sending ACK back");
    cmd.set(Iriss::Command::ACK);
    uart->send(cmd);
    sendAckBack = false;

    while(1) {
        PRINT_VERBOSE("Waiting for Command from FlightController");
        uart->recv(cmd);
        PRINT_VERBOSE("Got Command from FlightController");
        PRINT_VERBOSE(cmd);
        do_respond(cmd, channels);
        for(int i = 0; i < 8; ++i) {
            std::cout << "channels[" << i << "] = " << channels[i] << std::endl;
        }
    }

    delete uart;
    return 0;
}
