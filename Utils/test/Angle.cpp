#include "Utils/Radians.hpp"
#include "Utils/Degrees.hpp"
#include "Utils/AngleOps.hpp"
#include "test/Testing.hpp"
#include <cmath>

Utils::Degrees useGeneralArg(Utils::Angle& angle1, Utils::Angle& angle2)
{
    return angle1 + angle2;
}

int main(void)
{

    Utils::Degrees ninety(90);
    Utils::Radians pi_2(M_PI/2.0f);

    AssertEquals(ninety.asDegrees(), pi_2.asDegrees());
    AssertEquals(ninety.asRadians(), pi_2.asRadians());

    Utils::Degrees oneEighty = useGeneralArg(ninety, pi_2);

    AssertEquals(oneEighty.asDegrees(), 180.0f);
    AssertEquals(oneEighty.asRadians(), M_PI);

    // Make sure all of the math operations work
    Utils::Degrees a(10);
    Utils::Degrees b(23);

    Utils::Degrees c = a + b;
    AssertEquals(c.asDegrees(), 33.0f);

    c += a;
    AssertEquals(c.asDegrees(), 43.0f);

    c -= b;
    AssertEquals(c.asDegrees(), 20.0f);

    c = a * b;
    AssertEquals(c.asDegrees(), 230.0f);

    c *= a; // 2300.0f will wrap around to 140.0f
    AssertEquals(c.asDegrees(), 140.0f);

    c /= a;
    AssertEquals(c.asDegrees(), 14.0f);

    c = b/a;
    AssertEquals(c.asDegrees(), 2.3f);

    c = a - b;
    AssertEquals(c.asDegrees(), -13.0f);

    return 0; 
}
