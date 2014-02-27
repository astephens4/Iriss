#include "Radians.hpp"
#include "Degrees.hpp"

Utils::Degrees useGeneralArg(Utils::Angle& angle1, Utils::Angle& angle2)
{
    return Utils::Degrees(angle1 + angle2);
}

int main(void)
{

    Utils::Degrees ninety(90);
    Utils::Radians pi_2(M_PI/2.0f);

    assertFloatEquals(ninety, pi_2);

    Utils::Degrees oneEighty = useGeneralArgs(ninety, pi_2);

    assertFloatEquals(oneEighty.asDegrees(), 180.0f);
    
}
