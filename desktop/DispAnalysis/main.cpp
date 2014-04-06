#include <iostream>
#include <fstream>
#include "LineAnalysis/Line.hpp"
#include "LineAnalysis/LineDetector.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int nargs, char *argv[])
{
    if(nargs < 3)
        return -1;
    std::string lineFile(argv[1]);
    std::string imageFile(argv[2]);
    
    std::ifstream poop(lineFile);
    std::string garb;
    float distance, angle, centerX, centerY;
    poop >> garb;
    poop >> garb >> distance;
    poop >> garb >> angle;
    poop >> garb >> centerX;
    poop >> garb >> centerY;

    LineAnalysis::Line line;
    line.set_distance(Utils::Inches(distance));
    line.set_angle(Utils::Degrees(angle));
    line.set_center_x(Utils::Inches(centerX));
    line.set_center_y(Utils::Inches(centerY));


    cv::Mat picture = cv::imread(imageFile);

    // convert from inches to pixels
    int pixCX, pixCY;
    float conversion = LineAnalysis::get_in_per_pix(line.get_distance());
    pixCX = line.get_center_x().asInches()/conversion + picture.cols/2;
    pixCY = line.get_center_y().asInches()/conversion + picture.rows/2;

    int pixW, pixH;
    pixW = 1.85f/conversion;
    pixH = line.get_length().asInches()/conversion;
    cv::RotatedRect rrect(cv::Point2f(pixCX, pixCY), cv::Size2f(pixW, pixH), line.get_angle().asDegrees());

    cv::Point2f points[4];
    rrect.points(points);

    for(int i = 1; i <= 4; ++i) {
        cv::line(picture, points[i-1], points[i%4], cv::Scalar(0, 0, 255), 2);
    }
    cv::circle(picture, cv::Point(picture.cols/2, picture.rows/2), 5, cv::Scalar(255, 0, 0), -1);
    cv::circle(picture, cv::Point(pixCX, pixCY), 5, cv::Scalar(0, 0, 255), -1);

    cv::imshow("HerpaDerpaDerp", picture);

    cv::waitKey();
}
