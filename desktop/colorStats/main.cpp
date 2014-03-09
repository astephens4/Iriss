#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int nargs, char **argv)
{
    std::string imageFile;
    bool doVerbose = false;
    for(int i = 0; i < nargs; ++i) {
        if((strcmp(argv[i], "-i") == 0) && (i+1 < nargs)) {
            imageFile = std::string(argv[++i]);
        }
        else if(strcmp(argv[i], "-v") == 0) {
            doVerbose = true;
        }
    }

    cv::Mat image = cv::imread(imageFile);

    cv::Mat asHSV;
    cv::cvtColor(image, asHSV, CV_BGR2HSV);

    // min, max, location
    double min, max;
    cv::Point minLoc(0,0), maxLoc(100,100);

    cv::minMaxLoc(image, &min, &max, &minLoc, &maxLoc);
    std::cout << "BGR;" << min << "," << max << std::endl;
    
//    image.at<cv::Scalar>(minLoc) = cv::Scalar(0, 0, 0);
//    image.at<cv::Scalar>(maxLoc) = cv::Scalar(255, 255, 255);
//    cv::imshow("Original image", image);

    cv::minMaxLoc(asHSV, &min, &max);
    std::cout << "HSV;" << min << "," << max << std::endl;
    //std::cout << "HSV;" << min << "," << max << "," << minLoc << "," << maxLoc << std::endl;

    
}
