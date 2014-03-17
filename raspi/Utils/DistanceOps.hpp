#ifndef IRISS_DISTANCE_OPERATIONS_H
#define IRISS_DISTANCE_OPERATIONS_H 1

#include "Utils/Inches.hpp"
#include "Utils/Feet.hpp"

/**
 *
 */
Utils::Inches operator+(const Utils::Distance& lhs, const Utils::Distance& rhs);

/**
 *
 */
Utils::Inches operator-(const Utils::Distance& lhs, const Utils::Distance& rhs);

/**
 *
 */
Utils::Inches operator*(const Utils::Distance& lhs, const Utils::Distance& rhs);

/**
 *
 */
Utils::Inches operator/(const Utils::Distance& lhs, const Utils::Distance& rhs);

#endif // IRISS_DISTANCE_OPERATIONS_H
