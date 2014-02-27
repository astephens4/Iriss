#include "Degrees.hpp"

namespace Utils {

Degrees::Degrees(float value) :
	Angle(value)
{
	int div = static_cast<int>(this->value / 360.0f);
	this->value = this->value - (div*360.0f);
}

Degrees::Degrees(const Angle& other) :
	Angle(other)
{
	int div = static_cast<int>(this->asDegrees() / 360.0f);
	this->value = this->asDegrees() - (div*360.0f);
}

Degrees::Degrees(const Degrees& other) :
	Angle(other)
{
	int div = static_cast<int>(this->value / 360.0f);
	this->value = this->value - (div*360.0f);
}

Degrees Degrees::operator+(const Angle& rhs)
{
	return Degrees(this->asDegrees() + rhs.asDegrees());
}

Degrees Degrees::operator-(const Angle& rhs)
{
	return Degrees(this->asDegrees() - rhs.asDegrees());
}

Degrees Degrees::operator*(const Angle& rhs)
{
	return Degrees(this->asDegrees() * rhs.asDegrees());
}

Degrees Degrees::operator/(const Angle& rhs)
{
	if(rhs.asDegrees() == 0)
		throw std::logic_error("Divide by zero");
	return Degrees(this->asDegrees() + rhs.asDegrees());
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
