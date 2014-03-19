#include "LineDetector.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Utils/Inches.hpp"
#include "Utils/Degrees.hpp"

namespace LineAnalysis {

/**
 * Create an empty line detector
 */
LineDetector::LineDetector(void) :
    m_threshLow(0.1f),
    m_threshHigh(0.1f)
{

}

/**
 * Create a LineDetector for a given image which may contain lines
 * of the specified set of colors
 * @param image [in] A color image which should contain colored lines
 * @param colors [in] The set of colors the line should be
 * @param [in] threshLow Percentage below a hue which will be ignored in detection for that color
 * @param [in] threshHigh Percentage above a hue which will be ignored in detection for that color
 */
LineDetector::LineDetector(cv::Mat image, const std::vector<Utils::Color>& colors, float threshLow, float threshHigh) :
    m_colorList(colors),
    m_image(image),
    m_threshLow(threshLow),
    m_threshHigh(threshHigh)
{

}

LineDetector::~LineDetector(void)
{
    // no allocated resources to de-allocate
}

/**
 * Set the image to detect lines in.
 * @param [in] image The image to use for line detectino
 */
void LineDetector::set_image(cv::Mat& image)
{
    m_image = image;
}

/**
 * Set the colors of the lines to be detected
 * @param [in] colors Colors to use in line detection
 */
void LineDetector::set_colors(const std::vector<Utils::Color>& colors)
{
    m_colorList.clear();
    m_colorList.assign(colors.begin(), colors.end());
}

/**
 * Set the thresholds to use around the specified colors
 * @param [in] threshLow Percentage below a hue which will be ignored in detection for that color
 * @param [in] threshHigh Percentage above a hue which will be ignored in detection for that color
 */
void LineDetector::set_thresholds(float threshLow, float threshHigh)
{
    if(threshLow < 0.0f || threshLow > 1.0f || threshHigh < 0.0f || threshHigh > 1.0f) {
        threshLow = 0.1f;
        threshHigh = 0.1f;
        std::cerr << "Low threshold or high threshold out of range of 0 - 1, setting to defaults!\n";
        return;
    }
    m_threshLow = threshLow;
    m_threshHigh = threshHigh;
    return;
}

/**
 * Add a color to the color detection list
 * @param [in] color Color to add to the list. If the color is within the set thresholds
 *                   of a color already in the list, then it will not be added
 * @return True if the color was added to the list, false if it was not.
 */
bool LineDetector::add_color(const Utils::Color& color)
{
    float lowHue;
    float highHue;
    cv::Vec3b bgr(color.b, color.g, color.r), hsv;
    cv::cvtColor(bgr, hsv, CV_BGR2HSV);
    for(auto c : m_colorList) {
        bgr[0] = c.b;
        bgr[1] = c.g;
        bgr[2] = c.r;
        cv::cvtColor(bgr, hsv, CV_BGR2HSV);
        lowHue = hsv[0]*(1.0f-m_threshLow);
        highHue = hsv[0]*(1.0f+m_threshHigh);
        if(hsv[0] >= lowHue && hsv[0] <= highHue) {
            return false;
        }
    }
    m_colorList.push_back(color);
    return true;
}

/**
 * Find the lines in the image, with hints if possible
 * @param [out] detectedLines A set of lines that were found in the image
 * @return True if there are any line detections
 */
bool LineDetector::get_lines(std::vector<LineAnalysis::Line>& detectedLines)
{
    // TODO (later) use the hints to cut down the image size

    // do image processing for each color in the list
    m_lineList.clear();
    std::map<Utils::Color, std::vector<Line> > coloredLines;
    cv::Mat filtered(m_image.size(), CV_8UC1);
    for(auto color : m_colorList) {
        // convert to HSV
        cv::Vec3b hsv;
        cv::Vec3b bgr(color.b, color.g, color.r);
        cv::cvtColor(bgr, hsv, CV_BGR2HSV);
        cv::Range hueRange(hsv[0]*(1.0f-m_threshLow), hsv[0]*(1.0f+m_threshHigh));
        cv::Range satRange(140, 255);
        cv::Range valRange(115, 245);
        get_filtered_image(m_image, filtered, hueRange, satRange, valRange, true); 

        cv::Rect r = boundingRect(filtered); 
        std::cout << "Line Width: " << r.width << std::endl
                  << "Line Height: " << r.height << std::endl;
        Line l(Utils::Inches(r.height), Utils::Inches(r.width), color, Utils::Degrees(0));
        m_lineList.push_back(l);
    }

    detectedLines.assign(m_lineList.begin(), m_lineList.end());
    return true;
    // Update the hint map
}

void get_filtered_image(const cv::Mat& image, cv::Mat& filtered, const cv::Range& hueRange, const cv::Range& satRange, const cv::Range& valRange, bool doBlur)
{
    assert(filtered.depth() == CV_8U && filtered.channels() == 1);

    if(doBlur) {
        // Step 0: Blur!
        cv::blur(image, filtered, cv::Size(3, 3));
    }

    // Step 1: Perform threshold filtering
    cv::Scalar low(hueRange.start, satRange.start, valRange.start);
    cv::Scalar high(hueRange.end, satRange.end, valRange.end);
    cv::inRange(filtered, low, high, filtered);

    // Step 2: Discard small fragments which passed threshold filtering
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat filtCopy = filtered.clone();
    cv::findContours(filtCopy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    for(auto contour : contours) {
        cv::Rect bbox = cv::boundingRect(cv::Mat(contour));
        if(bbox.area() < 200) {
            // draw the ROI as black
            cv::Mat roi(filtered, bbox);
            roi = cv::Scalar(0, 0, 0);
        }
    }

}

} // end namespace LineAnalysis
