#include <iostream>
#include <string>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

const std::string windowName = "OpenCV Shits!";
int hueLow;
int hueHigh;
int satLow;
int satHigh;
int valLow;
int valHigh;
int angle;
int bbSize;
cv::Mat orig;

void UpdateImages(int, void*);

int main(int nargs, char **argv)
{
    // parse inputs
    unsigned char hue;
    std::string imageFile;
    for(int i = 0; i < nargs; ++i) {
        if((strcmp(argv[i], "-i") == 0) && (i+1 < nargs)) {
            imageFile = std::string(argv[++i]);
        }
        else if((strcmp(argv[i], "-h") == 0) && (i+1 < nargs)) {
            sscanf(argv[++i], "%hhu", &hue);
        }
    }

    orig = cv::imread(imageFile);
    if(orig.empty()) {
        std::cout << "Not open file :(\n";
        return 0;
    }
    hueLow = 40;
    hueHigh = 50;
    satLow = 140;
    satHigh = 255;
    valLow = 115;
    valHigh = 245;
    cv::Mat asHue(1, 1, orig.type(), cv::Scalar(45, (255+140)/2, (245+115)/2));
    cv::Mat asBGR;
    cv::cvtColor(asHue, asBGR, CV_HSV2BGR);
    std::cout << asHue << " ---> " << asBGR << std::endl;
    std::cout << asHue.at<cv::Vec3b>(0, 0) << " or " << asHue.at<cv::Vec3b>(1, 1) << std::endl;
    cv::namedWindow("Thresholding");
    cv::createTrackbar("BB Area Truncate", "Thresholding", &bbSize, 2000, UpdateImages);
    UpdateImages(0, (void*)NULL);
    while(cv::waitKey() != 'q') {}
}

void UpdateImages(int, void*)
{
    // Step 0: Blur!
    cv::Mat blured;
    cv::blur(orig, blured, cv::Size(3, 3));

    cv::Mat origHSV;
    cv::cvtColor(blured, origHSV, CV_BGR2HSV); 

    // Step 1: Perform threshold filtering
    cv::Mat threshed;
    cv::inRange(origHSV, cv::Scalar(hueLow, satLow, valLow), cv::Scalar(hueHigh, satHigh, valHigh), threshed);

    cv::Mat threshedCopy = threshed.clone();
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(threshedCopy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    for(auto contour : contours) {
        cv::Rect bbox = cv::boundingRect(cv::Mat(contour));
        if(bbox.area() < bbSize) {
            // draw the bbox in the same color as its neighbor
            cv::Point tl = bbox.tl();
            cv::Scalar fillColor = threshed.at<cv::Scalar>(tl);
            cv::Mat roi(threshed, bbox);
            roi = fillColor;
        }
    }

    cv::imshow("Thresholding", threshed);
}
