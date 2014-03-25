#ifndef MATH_QUATF_H
#define MATH_QUATF_H 1

#include <sstream>
#include "Math/tuple.hpp"
#include "Utils/Radians.hpp"
#include "Math/vec3.hpp"


namespace Math {

class quatf : public Tuple<float, 4U>
{
public:
	quatf();
	quatf(float x, float y, float z, float w);
	quatf(const Utils::Angle& angle, vec3f axis);
	quatf(const Utils::Angle& angle1, vec3f axis1,
		  const Utils::Angle& angle2, vec3f axis2,
		  const Utils::Angle& angle3, vec3f axis3);
	quatf(const quatf& other);
	quatf(float* data);

	float x() const { return this->m_data[0]; }
	float y() const { return this->m_data[1]; }
	float z() const { return this->m_data[2]; }
	float w() const { return this->m_data[3]; }

	virtual std::string ToString() const
	{
		std::stringstream sstream;
		sstream << "Quaternion {(" << w() << " + " << x() << "i + " << y() << "j + " << z() << "k)}";
		return sstream.str();
	}

	static void alwaysNormed(bool normOnWrite);
	const quatf& scale(float val);
	float mag() const;
	const quatf& norm();
	quatf conj() const;

	const quatf& operator=(quatf rhs);
	quatf operator+(const quatf& rhs) const;
	quatf operator-(const quatf& rhs) const;
	quatf operator*(const quatf& rhs) const;
	const quatf& operator+=(const quatf& rhs);
	const quatf& operator-=(const quatf& rhs);
	const quatf& operator*=(const quatf& rhs);
private:
	static bool normOnWrite;
};

} // end namespace Math

#endif // MATH_QUAT_H
