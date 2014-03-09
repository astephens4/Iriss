#include "LineDetector.hpp"

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
    unsigned char lineHue;
    std::vector<cv::Vec2f> prelimLines;
    for(auto color : m_colors) {
        linehue = Utils::hue(*color);
        LineAnalysis::ImageFilter(m_image,
                                  prelimLines,
                                  cv::Scalar(lineHue*(1.0f-m_threshLow), 55, 55),
                                  cv::Scalar(lineHue*(1.0f+m_threshHigh), 255, 255)
                                 );
    }

    // Match pairs of lines to get a single line
    std::vector<std::vector<cv::Vec2f> > similarLines;
    float thetaHigh;
    float thetaLow;
    const float similarPercent = 0.02f;
    for(auto line = prelimLines.begin(); line != prelimLines.end(); ++line) {
        // find another line in the list with a similar theta
        thetaHigh = (*line)[1] * 1.0f + similarPercent;
        thetaLow = (*line)[1] * 1.0f - similarPercent;
        std::vector<cv::Vec2f> currentList;
        currentList.push_back(*line);
        for(auto line2 = line+1; line2 != prelimLines.end();) {
            // similar theta means +-2%? <--- TODO tweak percentage as needed
            if((*line2)[1] >= thetaLow && (*line2)[1] <= thetaHigh) {
                currentList.push_back(*line2);
                line2 = prelimLines.erase(line2);
            }
            else {
                ++line2;
            }
        }
        if(currentList.size() > 1) {
            similarLines.push_back(currentList);
        }
    }

    double pixApart;
    double pixLong;
    for(auto similar : similarLines) {
        if(similar->size() == 2) {
            // Create a LineAnalysis::Line!
            pixApart = std::fabs((*similar)[0] - (*similar)[1]);
        }
        else { // 3 or more similarly oriented lines, check distances. Meh
            // for an even number of lines, there may be adjacent parallel lines
            // of similar dimensions, check for that

        }
        LineAnalysis::Line detectedLine;
        // calculate real world width
        detectedLine.setWidth(LineAnalysis::ConvertPixelsToInches(pixApart));

        // calculate real world length
        // uuh, HoughLines won't report where lines stop, wait to see if this is really
        // needed. Until then, set it as the length from where the parallel line between
        // the two enters and exits the image.

    }

    // Update the hint map
}

