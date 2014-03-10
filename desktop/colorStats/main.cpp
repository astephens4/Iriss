#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

std::string cv_depth_str(int depth)
{
    switch(depth) {
        case CV_8U:
            return "CV_8U";
        case CV_8S:
            return "CV_8S";
        case CV_16U:
            return "CV_16U";
        case CV_16S:
            return "CV_16S";
        case CV_32S:
            return "CV_32S";
        case CV_32F:
            return "CV_32F";
        case CV_64F:
            return "CV_64F";
    }
    return "OOPS!";
}

std::string cv_type_str(int depth, int chan)
{
    std::ostringstream oss;
    oss << cv_depth_str(depth);
    oss << "C" << chan;
    return oss.str();
}

void show_point(cv::Mat image, cv::Point location, cv::Scalar color, unsigned int size)
{
    if(size > 10) {
        size = 10;
    }

    int x0 = location.x,
        y0 = location.y;
    int x1 = location.x + size,
        y1 = location.y + size;
    cv::Size bloop = image.size();
    if(x1 > bloop.width) {
        x1 -= 2*size;
    }
    if(y1 > bloop.height) {
        y1 -= 2*size;
    }
    x0 = x0 < x1 ? x0 : x1;
    y0 = y0 < y1 ? y0 : y1;

    cv::Mat ptSection(image, cv::Rect(x0, y0, size, size));
    ptSection = color;
}

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
    if(image.empty()) {
        std::cerr << "Unable to read " << imageFile << std::endl;
        return -1;
    }
    std::cout << "FILE:" << imageFile << std::endl;

    cv::Mat asHSV;
    cv::cvtColor(image, asHSV, CV_BGR2HSV);
    std::cerr << "Some info on the image:\n"
              << "Dimensions: " << image.size() << std::endl
              << "Type stored: " << cv_type_str(image.depth(), image.channels()) << std::endl;

    // min, max, location
    double min, max;
    cv::Point minLoc;
    cv::Point maxLoc;

    cv::Mat bgrChannels[3];
    cv::split(image, bgrChannels);

    for(int i = 0; i < image.channels(); ++i) {
        cv::minMaxLoc(bgrChannels[i], &min, &max, &minLoc, &maxLoc);
        std::ostringstream oss;
        switch(i) {
            case 0:
                oss << "B;";
                break;
            case 1:
                oss << "G;";
                break;
            case 2:
                oss << "R;";
                break;
        }
        oss << "MIN:" << minLoc << "{" << min << "}" << "," << "MAX:" << maxLoc << "{" << max << "}";
        show_point(bgrChannels[i], minLoc, cv::Scalar(0), 4);
        show_point(bgrChannels[i], maxLoc, cv::Scalar(255), 4);
        imshow(oss.str(), bgrChannels[i]);
        std::cout << oss.str() << std::endl;
    }
    
    cv::Mat hsvChannels[3];
    cv::split(asHSV, hsvChannels);

    cv::Mat adjustedHue(hsvChannels[0].size(), hsvChannels[0].depth());
    for(int i = 0; i < image.channels(); ++i) {
        cv::minMaxLoc(hsvChannels[i], &min, &max, &minLoc, &maxLoc);
        std::ostringstream oss;
        switch(i) {
            case 0:
                oss << "H;";
                // adjust the contrast of the hue channel
                adjustedHue = 180 * (hsvChannels[0] - min)/(max - min);
                imshow("Contrast Stretched Hue", adjustedHue);
                break;
            case 1:
                oss << "S;";
                break;
            case 2:
                oss << "V;";
                break;
        }
        // grab the already known line's bounding box and take the average of those values
        cv::Mat lineBB(hsvChannels[i], cv::Rect(504, 0, 88, 720));
        double average = cv::mean(lineBB)[0];
        oss << "MIN:" << minLoc << "{" << min << "}" << "," << "MAX:" << maxLoc << "{" << max << "}" << "AVG:{" << average << "}";
        show_point(hsvChannels[i], minLoc, cv::Scalar(0), 4);
        show_point(hsvChannels[i], maxLoc, cv::Scalar(255), 4);
        imshow(oss.str(), hsvChannels[i]);
        std::cout << oss.str() << std::endl;
    }

    cv::waitKey();
}
