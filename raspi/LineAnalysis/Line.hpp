#ifndef IRISS_LINE_H
#define IRISS_LINE_H 1

namespace LineAnalysis {

class Line {
public:
    /**
     * Create a line with default values
     */
    Line(void);

    /**
     * Set the length of the line
     * @param [in] length Length of the line
     */
    void setLength(const Utils::Distance& length);

    /**
     * Set the width of the line
     * @param [in] width Width of the line
     */
    void setWidth(const Utils::Distance& width);

    /**
     * Set the color of the line
     * @param [in] color Color of the line
     */
    void setColor(const Utils::Color& color);

    /**
     * Set the angle of the line with respect to the image's x-axis
     * @param [in] angle Angle of the line
     */
    void setAngle(const Utils::Angle& angle);

    /**
     * Set which image the line was taken from
     * @param [in] image A reference to the image the line found in
     */
    void setImage(const cv::Mat& image);

    /**
     * Get the last time this line was updated
     * @return A tamestamp of the last time the line was changed
     */
    const Utils::TimeStamp& lastUpdatedAt(void);

private:
    Utils::Inches length; ///< Length of the line in inches
    Utils::Inches width; ///< Width of the line in inches
    Utils::Color color; ///< Color of the line
    Utils::Degrees angle; ///< Angle of the line w.r.t. the bottom of the image
    cv::Mat& image; ///< A reference to which image contains this line
    Utils::TimeStamp time; ///< When the line was created or last updated
};

}

#endif // IRISS_LINE_H
