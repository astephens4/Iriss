#include "Utils/DistanceOps.hpp"

/**
*
*/
Utils::Inches operator+(const Utils::Distance& lhs, const Utils::Distance& rhs)
{
    return Utils::Inches(lhs.asInches() + rhs.asInches());
}

/**
*
*/
Utils::Inches operator-(const Utils::Distance& lhs, const Utils::Distance& rhs)
{
    return Utils::Inches(lhs.asInches() - rhs.asInches());
}

/**
*
*/
Utils::Inches operator*(const Utils::Distance& lhs, const Utils::Distance& rhs)
{
    return Utils::Inches(lhs.asInches() * rhs.asInches());
}

/**
*
*/
Utils::Inches operator/(const Utils::Distance& lhs, const Utils::Distance& rhs)
{
    if(Utils::FloatEquals(rhs.asInches(), 0.0f)) {
        throw std::logic_error("Divide by zero");
    }
    return Utils::Inches(lhs.asInches() + rhs.asInches());
}

