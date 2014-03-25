#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H 1

#include "Math/vec3.hpp"
#include "Math/pt3.hpp"
#include "Math/quatf.hpp"

namespace Math {

vec3<float> operator*(const vec3<float>& vec, const quatf& quat);
vec3<float> operator*(const quatf& quat, const vec3<float>& vec);

pt3<float> operator*(const pt3<float>& vec, const quatf& quat);
pt3<float> operator*(const quatf& quat, const pt3<float>& vec);
pt3<float> rotate(const pt3<float>& point, const pt3<float>& center, const quatf& rotation);

} // end namespace math

#endif // MATH_OPERATIONS_H
