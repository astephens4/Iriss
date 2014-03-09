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
cv::Mat orig;

void UpdateImages(int, void*);

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

    orig = cv::imread(imageFile);
    if(orig.empty()) {
        std::cout << "Not open file :(\n";
        return 0;
    }
    hueLow = 40;
    hueHigh = 50;
    satLow = 130;
    satHigh = 255;
    valLow = 105;
    valHigh = 235;
    UpdateImages(0, (void*)NULL);
    cv::waitKey();
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

    // Step 2: Perform edge detecredtion
    cv::Mat edged;
    cv::Canny(threshed, edged, 75, 255, 3);

    // Step 3: Perform line detection
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(edged, lines, 1, CV_PI/180.0f, 60);

    cv::Mat lineImg(edged.size(), CV_8UC3, cv::Scalar(0, 0, 0));

    for(auto r_theta : lines) {
        float rho = r_theta[0];
        float theta = r_theta[1];
        cv::Point p1, p2;
        double a = std::cos(theta),
               b = std::sin(theta);
        double x0 = a*rho,
               y0 = b*rho;
        p1.x = std::round(x0 + 1000*(-b));
        p1.y = std::round(y0 + 1000*(a));
        p2.x = std::round(x0 - 1000*(-b));
        p2.y = std::round(y0 - 1000*(a));
        cv::line(lineImg, p1, p2, cv::Scalar(0, 0, 255), 2, CV_AA);
    }

    cv::imshow("Original", blured);
    cv::imshow("Thresholding", threshed);
    cv::imshow("Edges", edged);
    cv::imshow("Lines", lineImg);
}
