#include "test/Testing.hpp"
#include "LineAnalysis/Line.hpp"

using LineAnalysis::Line;

int main(void)
{
    // contruct with distance, length, color, and angle
    Line testLine(Utils::Inches(12), Utils::Inches(2), Utils::Color(0, 255, 0), Utils::Degrees(0));
    Utils::TimeStamp ts;

    AssertEquals(testLine.get_distance().asInches(), 12.0f);
    AssertEquals(testLine.get_length().asInches(), 2.0f);
    AssertEquals(testLine.get_color().asUint32(), 0x0000FF00u);
    AssertEquals(testLine.get_angle().asDegrees(), 0.0f);

    AssertEquals(testLine.last_updated_at().get_sec(), ts.get_sec());

    return 0;
}
