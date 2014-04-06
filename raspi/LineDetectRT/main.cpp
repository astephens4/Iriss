// Basic I/O stuff
#include <iostream>
#include <string>

// allow printing of system errors
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <errno.h>
#include <cassert>
#include <fstream>

// Some Utilities
#include "Utils/Color.hpp"
#include "LineAnalysis/LineDetector.hpp"

// Some opencv stuff
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

int main(void)
{
   
    // check for a running process called CameraDaemon.py, run it if isn't running
    system("mkdir -p /dev/shm/photocache");
    std::string imageFile = "/dev/shm/photocache/lineFollow.jpg";
    std::string takePhoto = "raspistill -awb auto -mm backlit -ex auto -w 640 -h 480 -t 1 -o " + imageFile;

    LineAnalysis::LineDetector detect;
    Utils::Color lineColor(110, 180, 41);
    detect.add_color(lineColor);
    while(1) {
        system(takePhoto.c_str());
        cv::Mat image = cv::imread(imageFile, CV_LOAD_IMAGE_COLOR);
        detect.set_image(image);
        std::vector<LineAnalysis::Line> lines;
        detect.get_lines(lines);
        std::cout << "*************DETECTED LINES*************\n";
        int count = 0;
        char buf[32];
        snprintf(buf, 32, "%lu", time(NULL));
        std::string name(buf);
        std::ofstream lineFile("/dev/shm/photocache/"+name+".line");
        for(LineAnalysis::Line line : lines) {
            std::cout << "Line " << count++ << std::endl;
            std::cout << "\tDistance: " << line.get_distance().asInches() << " in\n";
            std::cout << "\tAngle: " << line.get_angle().asDegrees() << "*\n";
            std::cout << "\tDisplacement: (" << line.get_center_x().asInches() << ", " << line.get_center_y().asInches() << ")\n";
            lineFile << line;
        }
        std::string mvCmd = "mv /dev/shm/photocache/lineFollow.jpg /dev/shm/photocache/"+name+std::string(".jpg");
        system(mvCmd.c_str());
        std::cout << "***********END DETECTED LINES***********\n";
    }
    return 0;
}
