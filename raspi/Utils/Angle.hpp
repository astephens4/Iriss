#ifndef IRISS_ANGLE_H
#define IRISS_ANGLE_H 1

#include <cmath>
#include <stdexcept>
#include "FloatEquals.hpp"

namespace Utils {

class Angle
{
public:
	Angle(float value);
	Angle(const Angle& other);
	virtual ~Angle();
	virtual float asRadians() const = 0;
	virtual float asDegrees() const = 0;

	// Equality
	bool operator==(const Angle& rhs) const;
	bool operator!=(const Angle& rhs) const;
	bool operator<(const Angle& rhs) const;
	bool operator<=(const Angle& rhs) const;
	bool operator>(const Angle& rhs) const;
	bool operator>=(const Angle& rhs) const;
protected:
	Angle() { throw std::logic_error("No argument construction is not allowed"); }
	float value;
};

} // end namespace Utils
#endif // IRISS_ANGLE_H
