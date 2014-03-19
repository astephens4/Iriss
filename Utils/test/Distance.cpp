#include "Utils/Inches.hpp"
#include "Utils/Feet.hpp"
#include "Utils/DistanceOps.hpp"
#include "test/Testing.hpp"
#include <cmath>

Utils::Inches useGeneralArg(Utils::Distance& dist1, Utils::Distance& dist2)
{
    return dist1 + dist2;
}

int main(void)
{

    Utils::Inches twelve(12.0f);
    Utils::Feet unity(1.0f);

    AssertEquals(twelve.asInches(), unity.asInches());
    AssertEquals(twelve.asFeet(), unity.asFeet());

    Utils::Inches twentyFour = useGeneralArg(twelve, unity);

    AssertEquals(twentyFour.asInches(), 24.0f);
    AssertEquals(twentyFour.asFeet(), 2.0f);

    // Make sure all of the math operations work
    Utils::Inches a(10);
    Utils::Inches b(23);

    Utils::Inches c = a + b;
    AssertEquals(c.asInches(), 33.0f);

    c += a;
    AssertEquals(c.asInches(), 43.0f);

    c -= b;
    AssertEquals(c.asInches(), 20.0f);

    c = a * b;
    AssertEquals(c.asInches(), 230.0f);

    c *= a;
    AssertEquals(c.asInches(), 2300.0f);

    c /= a;
    AssertEquals(c.asInches(), 230.0f);

    c = b/a;
    AssertEquals(c.asInches(), 2.3f);

    c = a - b;
    AssertEquals(c.asInches(), -13.0f);

    return 0; 
}
