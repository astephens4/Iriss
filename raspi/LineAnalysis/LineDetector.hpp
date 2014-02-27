#ifndef IRISS_LINE_DETECTOR_H
#define IRISS_LINE_DETECTOR_H 1

#include <core/core.hpp>
#include <vector>
#include "Utils/Color.hpp"
#include "Line.hpp"

namespace LineAnalysis {

/**
 * Use this class to detect lines of a specific color
 * in a given image
 */
class LineDetector {
public:
    /**
     * Create an empty line detector
     */
    LineDetector(void);

    /**
     * Create a LineDetector for a given image which may contain lines
     * of the specified set of colors
     * @param image [in] A color image which should contain colored lines
     * @param colors [in] The set of colors the line should be
     */
    LineDetector(cv::Mat image, const std::vector<Utils::Color>& colors, float threshLow, float threshHigh);

    ~LineDetector(void);

    /**
     * Set the image to detect lines in.
     * @param [in] image The image to use for line detectino
     */
    void setImage(cv::Mat image);

    /**
     * Set the colors of the lines to be detected
     * @param [in] colors Colors to use in line detection
     */
    void setColors(const std::vector<Utils::Color>& colors);

    /**
     * Set the thresholds to use around the specified colors
     * @param [in] threshLow Percentage below a color which will be ignored in detection for that color
     * @param [in] threshHigh Percentage above a color which will be ignored in detection for that color
     */
    void setThresholds(float threshLow, float threshHigh);

    /**
     * Add a color to the color detection list
     * @param [in] color Color to add to the list. If the color is within the set thresholds
     *                   of a color already in the list, then it will not be added
     * @return True if the color was added to the list, false if it was not.
     */
    bool addColor(const Utils::Color& color);

private:
    std::vector<Utils::Color> m_colorList; ///< Check for these color lines in the image
    std::vector<LineAnalysis::Line> m_lineList; ///< The list of detected lines
    cv::Mat m_image; ///< The image to check for lines
    float m_threshLow; ///< The precentage below a color which will not be used in line detection
    float m_threshHigh; ///< The percentage above a color which will not be used in line detection
};

}

#endif // IRISS_LINE_DETECTOR_H
