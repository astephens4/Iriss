#ifndef IRISS_LINE_DETECTOR_H
#define IRISS_LINE_DETECTOR_H 1

#include <vector>
#include <map>
#include "Utils/Color.hpp"
#include "LineAnalysis/Line.hpp"

namespace LineAnalysis {

/**
 * Apply a filter to create a binary image, where it is white for any
 * value between the thresholds, and black elsewhere. Any small white areas
 * are also removed.
 * @param [in] image HSV image to filter
 * @param [out] filtered Binary result of filtering the image
 * @param [in] hueRange Range of accepted hue values
 * @param [in] satRange Range of accepted saturation values
 * @param [in] valRange Range of accepted value values
 * @param [in] doBlur Blur the image before filtering
 */
void get_filtered_image(const cv::Mat& image, cv::Mat& filtered, const cv::Range& hueRange, const cv::Range& satRange, const cv::Range& valRange, bool doBlur);

/**
 * Convert the line's width in pixels into height in Inches
 * @param [in] pixelsWide Line Width in pixels
 * @return The distance from the line, in Inches
 */
Utils::Inches get_distance_from_width(unsigned int pixelsWide);

/**
 * Get the pixels->Inches conversion factor for this camera at the
 * given distance.
 * @param [in] height Distance of the object from the camera
 * @return The conversion factor, in inches/pixel
 */
float get_in_per_pix(const Utils::Distance& height);

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
     * @param [in] threshLow Percentage below a hue which will be ignored in detection for that color
     * @param [in] threshHigh Percentage above a hue which will be ignored in detection for that color
     */
    LineDetector(cv::Mat image, const std::vector<Utils::Color>& colors, float threshLow, float threshHigh);

    ~LineDetector(void);

    /**
     * Set the image to detect lines in.
     * @param [in] image The image to use for line detectino
     */
    void set_image(cv::Mat image);

    /**
     * Set the colors of the lines to be detected
     * @param [in] colors Colors to use in line detection
     */
    void set_colors(const std::vector<Utils::Color>& colors);

    /**
     * Set the thresholds to use around the specified colors
     * @param [in] threshLow Percentage below a hue which will be ignored in detection for that color
     * @param [in] threshHigh Percentage above a hue which will be ignored in detection for that color
     */
    void set_thresholds(float threshLow, float threshHigh);

    /**
     * Add a color to the color detection list
     * @param [in] color Color to add to the list. If the color is within the set thresholds
     *                   of a color already in the list, then it will not be added
     * @return True if the color was added to the list, false if it was not.
     */
    bool add_color(const Utils::Color& color);

    /**
     * Find the lines in the image, with hints if possible
     * @param [out] detectedLines A set of lines that were found in the image
     * @return True if there are any line detections
     */
    bool get_lines(std::vector<LineAnalysis::Line>& detectedLines);

private:
    std::vector<Utils::Color> m_colorList; ///< Check for these color lines in the image
    std::vector<LineAnalysis::Line> m_lineList; ///< The list of detected lines
    cv::Mat m_image; ///< The image to check for lines
    float m_threshLow; ///< The precentage below a color which will not be used in line detection
    float m_threshHigh; ///< The percentage above a color which will not be used in line detection

    typedef std::map<Utils::Color, LineAnalysis::Line> HintMap;
    HintMap hints; ///< A place to keep track of previous detections to help with future detections
};

}

#endif // IRISS_LINE_DETECTOR_H
