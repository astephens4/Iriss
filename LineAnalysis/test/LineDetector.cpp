#include "LineAnalysis/LineDetector.hpp"
#include "test/Testing.hpp"
#include <opencv2/highgui/highgui.hpp>

using LineAnalysis::LineDetector;

int main(void)
{
    // test with table.png first
    LineDetector detector;
    cv::Mat table = cv::imread("../../doc/ConversionFuncImgs/table.png");

    detector.set_image(table);
    detector.add_color(Utils::Color(110, 180, 41));

    std::vector<LineAnalysis::Line> lines;
    detector.get_lines(lines);

    for(auto line : lines) {
        std::cout << "Line:\n"
                  << " \tDistance: " << line.get_distance().asInches() << std::endl
                  << " \tLength: " << line.get_length().asInches() << std::endl
                  << " \tAngle: " << line.get_angle().asDegrees() << std::endl;
    }

    return 0; 
}
