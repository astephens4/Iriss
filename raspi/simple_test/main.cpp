#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(void)
{
    cv::Mat image = cv::imread("test.jpg");
    cv::Mat edges;
    cv::Canny(image, edges, 125, 250);
    cv::imshow("Look at these contours!\n", edges);
    while(1) {};
}
