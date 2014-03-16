#include "Utils/Radians.hpp"
#include "Utils/Degrees.hpp"
#include "test/Testing.hpp"
#include <cmath>

Utils::Degrees useGeneralArg(Utils::Angle& angle1, Utils::Angle& angle2)
{
    return Utils::Degrees(angle1.asDegrees() + angle2.asDegrees());
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
    return 0; 
}
