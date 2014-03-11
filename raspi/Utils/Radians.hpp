#ifndef IRISS_RADIANS_H
#define IRISS_RADIANS_H 1

#include "Utils/Angle.hpp"

namespace Utils {

class Radians : public Angle
{
public:
	Radians(float value);
	Radians(const Angle& other);
	Radians(const Radians& other);
	virtual ~Radians() {}
	virtual float asDegrees() const { return this->value; }
	virtual float asRadians() const { return this->value*(M_PI/180.0f); }

	Radians operator+(const Angle& rhs);
	Radians operator-(const Angle& rhs);
	Radians operator*(const Angle& rhs);
	Radians operator/(const Angle& rhs);

	const Radians& operator=(const Angle& rhs);
	const Radians& operator+=(const Angle& rhs);
	const Radians& operator-=(const Angle& rhs);
	const Radians& operator*=(const Angle& rhs);
	const Radians& operator/=(const Angle& rhs);

private:
	Radians() { throw std::logic_error("No argument constructor is not allowed"); }
};

} // end namespace Utils

#endif // IRISS_RADIANS_H
