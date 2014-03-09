#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int nargs, char **argv)
{
    // parse inputs
    unsigned char hue;
    std::string imageFile;
    bool doVerbose = false;
    for(int i = 0; i < nargs; ++i) {
        if((strcmp(argv[i], "-i") == 0) && (i+1 < nargs)) {
            imageFile = std::string(argv[++i]);
        }
        else if(strcmp(argv[i], "-v") == 0) {
            doVerbose = true;
        }
        else if((strcmp(argv[i], "-h") == 0) && (i+1 < nargs)) {
            sscanf(argv[++i], "%hhu", &hue);
        }
    }

    cv::Mat orig = cv::imread(imageFile);

    // Step 0: Blur!
    cv::Mat blured;
    cv::blur(orig, blured, cv::Size(3, 3));

    cv::Mat origHSV;
    cv::cvtColor(blured, origHSV, CV_BGR2HSV); 

    // Step 1: Perform threshold filtering
    cv::Mat threshed;
    cv::inRange(origHSV, cv::Scalar(hue*0.80f, 100, 0), cv::Scalar(hue*1.2f, 240, 255), threshed);

    // Step 2: Perform edge detecredtion
    cv::Mat edged;
    cv::Canny(threshed, edged, 75, 255, 3);

    // Step 3: Perform line detection
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(edged, lines, 1, CV_PI/180.0f, 60, 75, 10);

    cv::Mat lineImg(edged.size(), CV_8UC3, cv::Scalar(0, 0, 0));

    for(auto endPts : lines) {
        cv::line(lineImg, cv::Point(endPts[0], endPts[1]), cv::Point(endPts[2], endPts[3]), cv::Scalar(255, 0, 0), 2);
    }

    // Create the display shits!
    std::string windowName = "OpenCV Shits!";
    namedWindow(windowName, CV_WINDOW_AUTOSIZE);



    cv::imshow("Original!", blured);

    cv::imshow("Thresholded!", threshed);

    cv::imshow("Edge Detected!", edged);

    cv::imshow("Lines Detected!", lineImg);

    cv::waitKey();
}
