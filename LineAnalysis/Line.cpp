#include "LineAnalysis/Line.hpp"

namespace LineAnalysis {
/**
 * Create a line with default values
 */
Line::Line(void):
    m_length(10),
    m_width(2),
    m_color(0, 255, 0),
    m_angle(0),
    m_time()
{

}

Line::Line(const Utils::Distance& len, const Utils::Distance& width, const Utils::Color& c, const Utils::Angle& ang):
    m_length(len),
    m_width(width),
    m_color(c),
    m_angle(ang),
    m_time()
{

}

} // end namespace LineAnalysis
