#ifndef IRISS_ANGLE_OPERATIONS_H
#define IRISS_ANGLE_OPERATIONS_H 1

#include "Utils/Degrees.hpp"
#include "Utils/Radians.hpp"

/**
 *
 */
Utils::Radians operator+(const Utils::Angle& lhs, const Utils::Angle& rhs);

/**
 *
 */
Utils::Radians operator-(const Utils::Angle& lhs, const Utils::Angle& rhs);

/**
 *
 */
Utils::Radians operator*(const Utils::Angle& lhs, const Utils::Angle& rhs);

/**
 *
 */
Utils::Radians operator/(const Utils::Angle& lhs, const Utils::Angle& rhs);

#endif // IRISS_ANGLE_OPERATIONS_H
