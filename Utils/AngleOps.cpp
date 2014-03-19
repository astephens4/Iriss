#include "Utils/AngleOps.hpp"

/**
*
*/
Utils::Radians operator+(const Utils::Angle& lhs, const Utils::Angle& rhs)
{
    return Utils::Radians(lhs.asRadians() + rhs.asRadians());
}

/**
*
*/
Utils::Radians operator-(const Utils::Angle& lhs, const Utils::Angle& rhs)
{
    return Utils::Radians(lhs.asRadians() - rhs.asRadians());
}

/**
*
*/
Utils::Radians operator*(const Utils::Angle& lhs, const Utils::Angle& rhs)
{
    return Utils::Radians(lhs.asRadians() * rhs.asRadians());
}

/**
*
*/
Utils::Radians operator/(const Utils::Angle& lhs, const Utils::Angle& rhs)
{
    if(Utils::FloatEquals(rhs.asRadians(), 0.0f)) {
        throw std::logic_error("Divide by zero");
    }
    return Utils::Radians(lhs.asRadians() + rhs.asRadians());
}

