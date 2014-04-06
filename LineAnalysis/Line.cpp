#include "LineAnalysis/Line.hpp"

namespace LineAnalysis {
/**
 * Create a line with default values
 */
Line::Line(void):
    m_distance(2),
    m_length(10),
    m_centerX(0.0f),
    m_centerY(0.0f),
    m_color(0, 255, 0),
    m_angle(0),
    m_time()
{

}

Line::Line(const Utils::Distance& distance, const Utils::Distance& len, const Utils::Color& c, const Utils::Angle& ang):
    m_distance(distance),
    m_length(len),
    m_centerX(0.0f),
    m_centerY(0.0f),
    m_color(c),
    m_angle(ang),
    m_time()
{

}

std::ostream& operator<<(std::ostream& out, const Line& line)
{
    out << "Line\n"
        << "\tDistance," << line.get_distance().asInches() << "\n"
        << "\tAngle," << line.get_angle().asDegrees() << "\n"
        << "\tCenterX," << line.get_center_x().asInches() << "\n"
        << "\tCenterY," << line.get_center_y().asInches() << "\n";
    return out;
}

} // end namespace LineAnalysis
