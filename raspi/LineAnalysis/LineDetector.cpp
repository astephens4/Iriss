#include "LineDetector.hpp"

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
void LineDetector::set_image(cv::mat& image)
{
    m_image = image;
}

/**
 * Set the colors of the lines to be detected
 * @param [in] colors Colors to use in line detection
 */
void LineDetector::set_colors(const std::vector<Utils::Color>& colors)
{
    m_colors.clear();
    m_colors.insert(colors.begin(), colors.end());
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
    unsigned char hue = Utils::hue(color);
    for(auto itr = m_colors.begin(); itr != m_colors.end(); ++itr) {
        lowHue = Utils::hue(*itr)*(1.0f-m_lowThresh);
        highHue = Utils::hue(*itr)*(1.0f+m_highThresh);
        if(Utils::hue(color) >= lowHue && Utils::hue(color) <= highHue) {
            return false;
        }
    }
    m_colors.push_back(color);
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
        cv::Range hueRange(color.hue()*(1.0f-m_threshLow), color.hue()*(1.0f+m_threshHigh));
        cv::Range satRange(140, 255);
        cv::Range valRange(115, 245);
        get_filtered_image(m_image, filtered, hueRange, satRange, valRange, true); 

        // do some sool stuff!
    }

    // Update the hint map
}

void LineAnalysis::get_filtered_image(const cv::Mat& image, cv::Mat& filtered, const cv::Range& hueRange, const cv::Range& satRange, const cv::Range& valRange, bool doBlur)
{
    assert(filtered.depth() == CV_8U && filtered.channels() == 1);
    cv::Mat origHSV;
    cv::cvtColor(image, origHSV, CV_BGR2HSV); 
    if(doBlur) {
        // Step 0: Blur!
        cv::blur(origHSV, origHSV, cv::Size(3, 3));
    }

    // Step 1: Perform threshold filtering
    cv::Scalar low(hueRange.start, satRange.start, valRange.start);
    cv::Scalar high(hueRange.end, satRange.end, valRange.end);
    cv::inRange(origHSV, low, high, filtered);

    // Step 2: Discard small fragments which passed threshold filtering
    std::vector<std::vector<cv::Point> > contours;
    cv::findCountours(filtered, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    for(auto contour : contours) {
        std::vector<std::vector<cv::Point> > contourPoly;
        cv::approxPolyDP(cv::Mat(contour), contourPoly, 3, true);
        cv::Rect bbox = cv::boundingRect(cv::Mat(contourPoly));
        if(bbox.area() < 100) {
            // draw the ROI as black
            cv::Mat roi(filtered, bbox);
            roi = cv::Scalar(0, 0, 0);
        }
    }

}

} // end namespace LineAnalysis
