#include "Degrees.hpp"

namespace Utils {

Degrees::Degrees(float value) :
	Angle(value)
{
	int div = static_cast<int>(this->value / 360.0f);
	this->value = this->value - (div*360.0f);
}

Degrees::Degrees(const Angle& other) :
	Angle(other.asDegrees())
{
	int div = static_cast<int>(this->value / 360.0f);
	this->value = this->value - (div*360.0f);
}

Degrees::Degrees(const Degrees& other) :
	Angle(other)
{
	int div = static_cast<int>(this->value / 360.0f);
	this->value = this->value - (div*360.0f);
}

Degrees Degrees::operator+(const Angle& rhs) const
{
	return Degrees(this->asDegrees() + rhs.asDegrees());
}

Degrees Degrees::operator-(const Angle& rhs) const
{
	return Degrees(this->asDegrees() - rhs.asDegrees());
}

Degrees Degrees::operator*(const Angle& rhs) const
{
	return Degrees(this->asDegrees() * rhs.asDegrees());
}

Degrees Degrees::operator/(const Angle& rhs) const
{
	if(FloatEquals(rhs.asDegrees(), 0.0f)) {
		throw std::logic_error("Divide by zero");
    }
	return Degrees(this->asDegrees() / rhs.asDegrees());
}


const Degrees& Degrees::operator=(const Angle& rhs)
{
	this->value = rhs.asDegrees();
    return *this;
}

const Degrees& Degrees::operator+=(const Angle& rhs)
{
	*this = *this + rhs;
	return *this;
}

const Degrees& Degrees::operator-=(const Angle& rhs)
{
	*this = *this - rhs;
	return *this;	
}

const Degrees& Degrees::operator*=(const Angle& rhs)
{
	*this = *this * rhs;
	return *this;
}

const Degrees& Degrees::operator/=(const Angle& rhs)
{
	*this = *this / rhs;
	return *this;
}

} // end namespace Utils
