#include "main.hpp"

// Basic I/O stuff
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstring>

// threads
#include <thread>

// allow printing of system errors
#include <cstdio>
#include <errno.h>
#include <cassert>

// Working with Unix filesystems
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

// Some Utilities
#include "Utils/NetClient.hpp"
#include "Utils/SerialPeer.hpp"
#include "Iriss/Orders.hpp"
#include "Iriss/Command.hpp"
#include "Iriss/Orientation.hpp"

#define PRINT_VERBOSE(output_stream) \
    if(g_verbose) { \
        std::cout << output_stream << std::endl; \
    }

int main(int nargs, char *argv[])
{
    // Default connections' settings
    std::string serverAddr = "192.168.10.81";
    unsigned short portNumber = 9001;
    std::string serialPort = "/dev/ttyAMA0";

    bool g_verbose = false;

    // Parse any command line options
    for(int i = 1; i < nargs; ++i) {
        if((strcmp(argv[i], "-i") == 0) || (strcmp(argv[i], "--ip") == 0)) {
            // read and verify ip address
            if(i+1 < nargs) {
                ++i;
                int octets[4];
                int num = sscanf(argv[i], "%d.%d.%d.%d", &(octets[0]), &(octets[1]), &(octets[2]), &(octets[3]));
                if(num < 4) {
                    std::cerr << "Malformed IP Address: " << argv[i] << std::endl;
                    return -1;
                }
                for(int j = 0; j < 4; ++j) {
                    if(octets[j] < 0 || octets[j] > 255) {
                        std::cerr << "Malformed IP Address: " << argv[i] << std::endl
                                  << "                      ";
                        char *loc = strstr(argv[i], ".");
                        for(int k = 0; k < i; ++k) {
                            loc = strstr(argv[i], ",");
                        }
                        for(size_t k = 0; k < strlen(argv[i]) - strlen(loc); ++k)
                            std::cerr << " ";
                        std::cerr << "^" << std::endl;

                    }
                }
                serverAddr = argv[i];
            }
            else {
                std::cerr << "Please sepcify an ip address after " << argv[i] << std::endl;
                return -1;
            }
        }
        else if((strcmp(argv[i], "-p") == 0) || (strcmp(argv[i], "--port") == 0)) {
            if(i+1 < nargs) {
                ++i;
                unsigned int p;
                sscanf(argv[i], "%u", &p);
                if((p <= 1024) || (p > ((1<<16)-1))) {
                    std::cerr << "Ports must be above 1024 and below " << (1<<16)-1 << std::endl;
                    return -1;
                }
                portNumber = static_cast<unsigned short>(p);
            }
            else {
                std::cerr << "Please specify a port number after " << argv[i] << std::endl;
                return -1;
            }
        }
        else if((strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "--serial") == 0)) {
            if(i+1 < nargs) {
                ++i;
                serialPort = argv[i];
            }
        }
        else if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            std::cout << "FlightController - Steer a quadcopter according to orders sent from a server\n"
                      << "Options:\n"
                      << " <Option,ShortForm>\t<argument>\t\t\t\t<default>\n"
                      << " --ip,-i\t\t<IP version 4 address of the server>\t192.168.10.81\n"
                      << " --port,-p\t\t<Port to connect to on the server>\t9001\n"
                      << " --serial,-s\t\t<Serial port going to the ArduPilot>\t/dev/ttyAMA0\n"
                      << " --help,-h\n";
            return -1;
        }
        else if((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--verbose") == 0)) {
            g_verbose = true;
        }
    }

    // connect to the ArduPilot
    PRINT_VERBOSE("Opening connection to ArduPilot on " << serialPort);
    Utils::SerialPeer::Settings settings;
    settings.mode = Utils::SerialPeer::UART_8N1;
    settings.flow = Utils::SerialPeer::UART_NO_FLOW_CONTROL;
    settings.speed = Utils::SerialPeer::BAUD_115200;
    settings.blocking = Utils::SerialPeer::UART_FULL_BLOCK;
    Utils::SerialPeer uart(serialPort, settings);

    // Let the ardupilot know we are online
    Iriss::Command cmd(Iriss::Command::ACK); 
    PRINT_VERBOSE("Sending ACK to ArduPilot");
    uart.send(cmd);

    // Wait for an ACK from the ArduPilot
    PRINT_VERBOSE("Waiting for ACK from ArduPilot");
    uart.recv(cmd);
    if(cmd.get() != Iriss::Command::ACK) {
        std::cerr << "Unable to estblish communication with ArduPilot! Exiting!\n";
        return -1;
    }

    // connect to the CommandCenter software at 
    PRINT_VERBOSE("Connecting to CommandCenter at " << serverAddr << ":" << portNumber);
    Utils::NetClient cmdCenter;
    cmdCenter.connect_to_server(serverAddr, portNumber);
    if(!cmdCenter.is_valid()) {
        std::cerr << "Could not establish connection with the command center!\n";
        return -2;
    }
    
    // check for a running process called CameraDaemon.py, run it if isn't running
    PRINT_VERBOSE("Preparing the photo cache");
    system("mkdir -p /dev/shm/photocache");
    std::string imageFile = "/dev/shm/photocache/lineFollow.png";
    std::string takePhoto = "raspistill -awb auto -mm backlit -ex auto -w 640 -h 480 -t 1 -o " + imageFile;

    // Objects to use in the main loop
    std::string removeCmd;
    Iriss::Orders orders;
    Iriss::Orientation orientation(0.0f, 0.0f, 0.0f);

    // The main Loop! wait for orders from the Command Center
    PRINT_VERBOSE("Waiting for Orders from the CommandCenter");
    while(cmdCenter.receive(orders)) {

        // Now we have command, is it completed?
        PRINT_VERBOSE("Received Orders");
        while(orders.has_tasks()) {
            system(takePhoto.c_str());

            if(imageFile.empty()) {
                continue;
            }
            PRINT_VERBOSE("Using image " << imageFile);

            // Get the orientation from the ArduPilot
            PRINT_VERBOSE("Asking for orientation from the ArduPilot");
            cmd.set(Iriss::Command::GET_ORIENTATION);
            uart.send(cmd);
            uart.recv(orientation);
            PRINT_VERBOSE("Got Orientation (" << orientation.roll << ", " << orientation.pitch << ", " << orientation.yaw << ")");

            // Apply the orientation and image to the current task
            cmd = orders.apply(imageFile, orientation);
            PRINT_VERBOSE("Applying Orders resulted with " << cmd);
            uart.send(cmd);

            // delete the image we have used
            removeCmd = "rm ";
            removeCmd += imageFile;
//            system(removeCmd.c_str());
        }
    }
    std::cerr << "Connection with the command center has been terminated! Initiate land!\n";
    // do a landing sequence where ever we are
    Iriss::Orders land;
    land.queue_task(Iriss::Orders::LAND, 0);
    while(land.has_tasks()) {
        system(takePhoto.c_str());
        if(imageFile.empty()) {
            continue;
        }
        
        // Get the orientation from the ArduPilot
        cmd.set(Iriss::Command::GET_ORIENTATION);
        uart.send(cmd);
        uart.recv(orientation);

        // Apply the orientation and image to the current task
        Iriss::Command cmd = orders.apply(imageFile, orientation);
        uart.send(cmd);

        // delete the image we have used
        removeCmd = "rm ";
        removeCmd += imageFile;
        system(removeCmd.c_str());
    }
    return 0;
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
 * Check to see if the given file exists
 * @param [in] fileName File to check, can be relative or absolute
 */
bool Iriss::is_file(const std::string& fileName)
{
    struct stat fileStats;
    if(stat(fileName.c_str(), &fileStats) < 0) {
        perror("unable to stat file");
    }
    if(S_ISREG(fileStats.st_mode))
        return true;
    return false;
}

std::string Iriss::find_recent_image(const std::string& dir, float /*validWindow*/)
{
    DIR *proc = opendir(dir.c_str());
    if(proc == nullptr) {
        perror("Could not open image directory");
        return std::string("");
    }

    time_t now = time(NULL);
    time_t shortest = now;
    std::string retName("");
    for(struct dirent *entry = readdir(proc); entry != nullptr; entry = readdir(proc)) {
        std::string fullName = dir + "/" + std::string(entry->d_name);
        if(is_file(fullName) && fullName.find("jpg") != std::string::npos) {
            struct stat buf;
            stat(fullName.c_str(), &buf);
            double diff = difftime(now, buf.st_mtime);
            if(diff < shortest) {
                shortest = diff;
                retName = fullName;
            }
        }
    }
    return retName;
}
