#include "Radians.hpp"

namespace Utils {

Radians::Radians(float value) :
	Angle(value)
{
	int div = static_cast<int>(this->value / (M_PI*2.0f));
	this->value = this->value - (div*M_PI*2.0f);
}

Radians::Radians(const Angle& other) :
	Angle(other)
{
	int div = static_cast<int>(this->asRadians() / (M_PI*2.0f));
	this->value = this->asRadians() - (div*M_PI*2.0f);
}

Radians::Radians(const Radians& other) :
	Angle(other)
{
	int div = static_cast<int>(this->value / (M_PI*2.0f));
	this->value = this->value - (div*M_PI*2.0f);
}

Radians Radians::operator+(const Angle& rhs)
{
	return Radians(this->asRadians() + rhs.asRadians());
}

Radians Radians::operator-(const Angle& rhs)
{
	return Radians(this->asRadians() - rhs.asRadians());
}

Radians Radians::operator*(const Angle& rhs)
{
	return Radians(this->asRadians() * rhs.asRadians());
}

Radians Radians::operator/(const Angle& rhs)
{
	if(rhs.asRadians() == 0)
		throw std::logic_error("Divide by zero");
	return Radians(this->asRadians() + rhs.asRadians());
}


const Radians& Radians::operator=(const Angle& rhs)
{
	this->value = rhs.asRadians();
	return *this;
}

const Radians& Radians::operator+=(const Angle& rhs)
{
	*this = *this + rhs;
	return *this;
}

const Radians& Radians::operator-=(const Angle& rhs)
{
	*this = *this - rhs;
	return *this;	
}

const Radians& Radians::operator*=(const Angle& rhs)
{
	*this = *this * rhs;
	return *this;
}

const Radians& Radians::operator/=(const Angle& rhs)
{
	*this = *this / rhs;
	return *this;
}

} // end namespace Utils
