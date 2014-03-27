#ifndef IRISS_LINE_H
#define IRISS_LINE_H 1

#include "opencv2/core/core.hpp"
#include "Utils/Inches.hpp"
#include "Utils/Degrees.hpp"
#include "Utils/Color.hpp"
#include "Utils/TimeStamp.hpp"

namespace LineAnalysis {

class Line {
public:
    /**
     * Create a line with default values
     */
    Line(void);

    /**
     * Create a line with everything known
     */
    Line(const Utils::Distance& distance, const Utils::Distance& len, const Utils::Color& c, const Utils::Angle& ang);

    // setters
    /**
     * Set the length of the line
     * @param [in] length Length of the line
     */
    void set_length(const Utils::Distance& length) { m_length = length; };

    /**
     * Set the distance of the line
     * @param [in] distance distance of the line
     */
    void set_distance(const Utils::Distance& distance) { m_distance = distance; };

    /**
     * Set the color of the line
     * @param [in] color Color of the line
     */
    void set_color(const Utils::Color& color) { m_color = color; };

    /**
     * Set the angle of the line with respect to the image's x-axis
     * @param [in] angle Angle of the line
     */
    void set_angle(const Utils::Angle& angle) { m_angle = angle; };

    void set_center_x(const Utils::Distance& dist) { m_centerX = dist; };
    
    void set_center_y(const Utils::Distance& dist) { m_centerY = dist; };

    // getters
    Utils::Inches get_length() const { return m_length; };
    Utils::Inches get_distance() const { return m_distance; };
    Utils::Color get_color() const { return m_color; };
    Utils::Degrees get_angle() const { return m_angle; };
    Utils::Inches get_center_x(void) { return m_centerX; };
    Utils::Inches get_center_y(void) { return m_centerY;; };


    /**
     * Get the last time this line was updated
     * @return A tamestamp of the last time the line was changed
     */
    const Utils::TimeStamp& last_updated_at(void) const { return m_time; };

private:
    Utils::Inches m_distance; ///< distance of the line from the viewer in inches
    Utils::Inches m_length; ///< Length of the line in inches
    Utils::Inches m_centerX; ///< Location of the middle of the line, inches from the left side of the image
    Utils::Inches m_centerY; ///< Location of the middle of the line, inches from the bottom of the image
    Utils::Color m_color; ///< Color of the line
    Utils::Degrees m_angle; ///< Angle of the line w.r.t. the bottom of the image
    Utils::TimeStamp m_time; ///< When the line was created or last updated
};

}

#endif // IRISS_LINE_H
