#include "LineAnalysis/LineDetector.hpp"
#include "test/Testing.hpp"
#include <opencv2/highgui/highgui.hpp>

using LineAnalysis::LineDetector;

int main(int nargs, char *argv[])
{
    std::cout << nargs << std::endl;
    // test with table.png first
    LineDetector detector;
    cv::Mat table = cv::imread(argv[1]);
    std::cout << "Opening " << argv[1] << std::endl;

    detector.set_image(table);
    detector.add_color(Utils::Color(110, 180, 41));

    std::vector<LineAnalysis::Line> lines;
    detector.get_lines(lines);

    for(auto line : lines) {
        std::cout << "Line:\n"
                  << " \tDistance: " << line.get_distance().asInches() << std::endl
                  << " \tLength: " << line.get_length().asInches() << std::endl
                  << " \tAngle: " << line.get_angle().asDegrees() << std::endl
                  << " \tCentered At: (" << line.get_center_x().asInches() << ", " << line.get_center_y().asInches() << ")\n";
                
        // convert from inches to pixels
        int pixCX, pixCY;
        float conversion = LineAnalysis::get_in_per_pix(line.get_distance());
        pixCX = line.get_center_x().asInches()/conversion + table.cols/2;
        pixCY = line.get_center_y().asInches()/conversion + table.rows/2;

        int pixW, pixH;
        pixW = 1.85f/conversion;
        pixH = line.get_length().asInches()/conversion;
        cv::RotatedRect rrect(cv::Point2f(pixCX, pixCY), cv::Size2f(pixW, pixH), line.get_angle().asDegrees());

        cv::Point2f points[4];
        rrect.points(points);

        for(int i = 1; i <= 4; ++i) {
            cv::line(table, points[i-1], points[i%4], cv::Scalar(0, 0, 255), 2);
        }
        cv::circle(table, cv::Point(table.cols/2, table.rows/2), 5, cv::Scalar(255, 0, 0), -1);
        cv::circle(table, cv::Point(pixCX, pixCY), 5, cv::Scalar(0, 0, 255), -1);
    }

    cv::imshow("Ermehgerd, Line!", table);

    cv::waitKey();

    return 0; 
}
