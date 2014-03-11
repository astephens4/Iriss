#ifndef IRISS_DEGREES_H
#define IRISS_DEGREES_H 1

#include "Utils/Angle.hpp"

namespace Utils {

class Degrees : public Angle
{
public:
	Degrees(float value);
	Degrees(const Angle& other);
	Degrees(const Degrees& other);
	virtual ~Degrees() {}
	virtual float asDegrees() const { return this->value; }
	virtual float asRadians() const { return this->value*(M_PI/180.0f); }

	Degrees operator+(const Angle& rhs);
	Degrees operator-(const Angle& rhs);
	Degrees operator*(const Angle& rhs);
	Degrees operator/(const Angle& rhs);

	const Degrees& operator=(const Angle& rhs);
	const Degrees& operator+=(const Angle& rhs);
	const Degrees& operator-=(const Angle& rhs);
	const Degrees& operator*=(const Angle& rhs);
	const Degrees& operator/=(const Angle& rhs);

private:
	Degrees() { throw std::logic_error("No argument constructor is not allowed"); }
};

} // end namespace Utils

#endif // IRISS_DEGREES_H
