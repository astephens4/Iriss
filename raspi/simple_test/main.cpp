#include <iostream>
#include <opencv2/core/core.hpp>

int main(void)
{
    std::cout << "Testing build with opencv" << std::endl;
    cv::Matx<float,4,4> sqMat;
    std::cout << sqMat << std::endl;
}
