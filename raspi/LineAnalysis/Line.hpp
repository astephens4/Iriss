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
    Line(const Utils::Distance& len, const Utils::Distance& width, const Utils::Color& c, const Utils::Angle& ang);

    // setters
    /**
     * Set the length of the line
     * @param [in] length Length of the line
     */
    void set_length(const Utils::Distance& length) { m_length = length; };

    /**
     * Set the width of the line
     * @param [in] width Width of the line
     */
    void set_width(const Utils::Distance& width) { m_width = width; };

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

    // getters
    Utils::Inches get_length() const { return m_length; };
    Utils::Inches get_width() const { return m_width; };
    Utils::Color get_color() const { return m_color; };
    Utils::Degrees get_angle() const { return m_angle; };

    /**
     * Get the last time this line was updated
     * @return A tamestamp of the last time the line was changed
     */
    const Utils::TimeStamp& last_updated_at(void) const { return m_time; };

private:
    Utils::Inches m_length; ///< Length of the line in inches
    Utils::Inches m_width; ///< Width of the line in inches
    Utils::Color m_color; ///< Color of the line
    Utils::Degrees m_angle; ///< Angle of the line w.r.t. the bottom of the image
    Utils::TimeStamp m_time; ///< When the line was created or last updated
};

}

#endif // IRISS_LINE_H
