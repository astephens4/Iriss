#include "Math/operations.hpp"

namespace Math {

vec3f operator*(const vec3f& vec, const quatf& quat)
{
    // method taken from the wikipedia page quaternions and spatial rotation
    // ret = v + 2r^(r^v + w*v)
    vec3f r(quat.x(), quat.y(), quat.z()),
          v = vec,
          ret = r^v;
    ret += v.scale(quat.w());
    r += r; 
    ret = r^ret;
    ret += vec;
    return ret;
}

vec3f operator*(const quatf& quat, const vec3f& vec)
{
	return vec*quat;
}

pt3<float> operator*(const pt3<float>& point, const quatf& quat)
{
    vec3<float> ret = vec3<float>(point.x(), point.y(), point.z()) * quat;
    return pt3<float>(ret.x(), ret.y(), ret.z());
}

pt3<float> operator*(const quatf& quat, const pt3<float>& point)
{
	return point*quat;
}

pt3<float> rotate(const pt3<float>& point, const pt3<float>& center, const quatf& rotation)
{
    pt3<float> p = point - center;
    p = p*rotation;
    p += center;
    return p;
}

} // end namespace math
