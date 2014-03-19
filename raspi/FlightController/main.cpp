#include "main.hpp"

// Basic I/O stuff
#include <iostream>
#include <sstream>
#include <string>

// allow printing of system errors
#include <cstdio>
#include <errno.h>
#include <cassert>

// Some Utilities
#include "Utils/NetClient.hpp"
#include "Utils/SerialPeer.hpp"
#include "Iriss/Orders.hpp"
#include "Iriss/Command.hpp"
#include "Iriss/Orientation.hpp"

int main(int nargs, char *argv[])
{
    // connect to the ArduPilot
    Utils::SerialPeer::Settings settings;
    settings.mode = Utils::SerialPeer::UART_8N1;
    settings.flow = Utils::SerialPeer::UART_HARDWARE;
    settings.speed = Utils::SerialPeer::BAUD_115200;
    Utils::SerialPeer uart("/dev/ttyUSB0", settings);

    // send an echo command and wait for a reply
    Iriss::Command cmd; 
    cmd.set(Utils::Command::ACK);
    uart.send(cmd);

    uart.recv(cmd);
    if(cmd.get() != Utils::Command::ACK) {
        std::cerr << "Unable to establish communication with ArduPilot! Exiting!\n";
        return -1;
    }

    // connect to the CommandCenter software at 192.168.1.81:9001
    Utils::NetClient cmdCenter;
    cmdCenter.connect_to_server("192.168.1.81", 9001);
    if(!cmdCenter.is_valid()) {
        std::cerr << "Could not establish connection with the command center!\n";
        return -2;
    }
    
    // check for a running process called CameraDaemon.py, run it if isn't running
    if(!is_process_running("CameraDaemon.py")) {
        assert(is_file("/usr/local/bin/CameraDaemon.py"));
        system("python /usr/local/bin/CameraDaemon.py");
    }

    // wait for a command from the CommandCenter
    std::string removeCmd;
    Iriss::Orders orders;
    struct Iriss::Orientation orientation;
    LineAnalysis::LineDetector detector;
    std::vector<Iriss::Command> commandList;
    std::vector<LineAnalysis::Line> lines;
    while(cmdCenter.receive(orders)) {
        // update the detector for the given orders
        detector.set_colors(orders.get_colors());

        // Now we have command, is it completed?
        while(orders.has_tasks()) {
            std::string imageFile = find_recent_image("/dev/shm/photocache", 0.1f); 
            if(imageFile.empty()) {
                continue;
            }
            
            detector.set_image(imageFile);

            cmd.set(Utils::Command::GET_ORIENTATION);
            uart.send(cmd);

            uart.recv(orientation);

            // Get line info
            detector.get_lines(lines);
            commandList.clear();
            for(LineAnalysis::Line line : lines) {
                commandList.push_back(orders.apply(line, orientation));
            }
            

            // Send corrections to ArduPilot
            // TODO set how this should be done

            // delete the image we have used
            removeCmd = "rm ";
            removeCmd += imageFile;
            system(removeCmd.c_str());
        }
    }
    std::cerr << "Connection with the command center has been terminated! Initiate land!\n";
    // do a landing sequence where ever we are
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
    if(IS_REG(fileStats.st_mode))
        return true;
    return false;
}

std::string Iriss::find_recent_image(const std::string& dir, float validWindow)
{
    DIR *proc = opendir(dir.c_str());
    if(proc == nullptr) {
        perror("Could not open image directory");
        return std::string("");
    }

    time_t now = std::time(NULL);
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
