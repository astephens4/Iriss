#include "main.hpp"

// Terminal and string I/O
#include <iostream>
#include <sstream>
#include <fstream>

// UART library
#include <termios.h>

// TCP/IP sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// allow printing of system errors
#include <cstdio>
#include <errno.h>
#include <cassert>

// Some Utilities
#include "Iriss/Orders.hpp"
#include "Iriss/Command.hpp"
#include "Iriss/Orientation.hpp"

int main(int nargs, char *argv[])
{
    // connect to the ArduPilot
    std::fstream uart("/dev/ttyUSB0");
    if(!uart.is_open()) {
        std::cerr << "Unable to open UART connection to ArduPilot! EXITING!\n";
        return -1;
    }

    // Configure the serial port
    struct termios uartSettings;
    cfsetspeed(&uartSettings, B115200); // set baud rate to 115200
    uartSettings.c_cflag = 0;
    uartSettings.c_cflag |= CRTSCTS; // set flow control to CTS/RTS
    uartSettings.c_cflag |= CS8;    // use 8 data bits
    uartSettings.c_cflag |= CSTOPB; // use one stop bit
    uartSettings.c_cflag |= PARODD; // use odd parity
    tcsetattr(uart, 0, &uartSettings); // set them on the serial line
    
    // send an echo command and wait for a reply
    Iriss::Command arduResp; 
    uart << Iriss::ECHO;
    uart >> arduResp;
    if(arduResp != Iriss::Command::ECHO) {
        std::cerr << "Did not get a response from the ArduPilot! EXITING!\n";
        return -1;
    }

    // connect to the CommandCenter software at 192.168.1.81:9001
    int cmdCenter = connect_to_command_center("192.168.1.81", 9001);
    if(cmdCenter < 0) {
        return -2;
    }

    // check for a running process called CameraDaemon.py, run it if isn't running
    if(!is_process_running("CameraDaemon.py")) {
        assert(is_file("/usr/local/bin/CameraDaemon.py"));
        system("python /usr/local/bin/CameraDaemon.py");
    }

    // wait for a command from the CommandCenter
    Iriss::Orders orders;
    struct Iriss::Orientation orientation;
    LineAnalysis::LineDetector detector;
    std::vector<Iriss::Command> commandList;
    std::vector<LineAnalysis::Line> lines;
    while(wait_on_orders(cmdCenter, orders)) {
        // update the detector for the given orders
        detector.set_colors(orders.get_colors());

        // Now we have command, is it completed?
        while(orders.has_tasks()) {
            std::string imageFile = find_recent_image("/dev/shm/photocache", 0.1f); 
            if(imageFile.empty()) {
                continue;
            }
            
            detector.set_image(imageFile);

            // get data from ArduPilot
            uart << Iriss::GET_ORIENTATION;
            uart >> orientation.roll
                 >> orientation.pitch
                 >> orientation.yaw
                 >> orientation.baro;

            // Get line info
            detector.get_lines(lines);
            commandList.clear();
            for(LineAnalysis::Line line : lines) {
                commandList.push_back(orders.apply(line, orientation));
            }
            

            // Send corrections to ArduPilot
            if(!commandList.empty()) {
                uart << Iriss::BEGIN_COMMAND_LIST;
            }
            for(Iriss::Command cmd : commandList) {
                uart << cmd;
            }
            uart << Iriss::END_COMMAND_LIST;

        }
    }
}

/**
 * Check to see if the named process is running
 * @param [in] process name
 * @return True if the given process name is running, false otherwise
 */
bool Iriss::is_process_running(const std::string& execName)
{
    DIR *proc = opendir("/proc");
    if(proc == nullptr) {
        perror("Could not open /proc");
        return false;
    }

    int pid = 0;
    for(struct dirent *entry = readdir(proc); entry != nullptr; entry = readdir(proc)) {
        // only open /proc/{int} directories
        if(sscanf(entry->d_name, "%d", &pid) == 1) {
            // open /proc/pid/cmdline and see if execName is found in it
            std::ostringstream oss;
            oss << "/proc/" << pid << "/cmdline";
            std::ifstream cmdFile(oss.str());
            if(cmdFile.is_open()) {
                std::string execLine;
                cmdFile >> execLine;
                if(execLine.find(execName) != std::string::npos) {
                    std::cout << "Process has been found! " << pid << std::endl;
                    closedir(proc);
                    return true;
                }
            }
        }
    }
    closedir(proc);
    return false;
}

/**
 * Wait to receive a set of orders from the command center on socket
 * @param [in] socket Open connection to the command center
 * @param [out] ccOrders Orders that are received from the command center
 * @return True if action orders are given, false if disconnected
 */
bool Iriss::wait_on_orders(int socket, Iriss::Orders& ccOrders)
{

}

/**
 * Performs various checks to see if the given orders are completed
 * @param [in] orders Orders issued to this quadcopter
 * @return True if the orders are complete, false otherwise.
 */
bool Iriss::orders_complete(const Iriss::Orders& orders)
{

}

/**
 * Check to see if the given file exists
 * @param [in] fileName File to check, can be relative or absolute
 */
bool Iriss::is_file(const std::string& fileName)
{
    struct stat fileStats;
    if(stat(fileName.c_str(), &fileStats) < 0) {
        perror("unable to stat file");
    }
    if(IS_REG(fileStats.st_mode))
        return true;
    return false;
}

