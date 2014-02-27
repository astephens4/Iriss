#include "Feet.hpp"

namespace Utils {

Feet::Feet(float value) :
	Distance(value)
{
}

Feet::Feet(const Distance& other) :
	Distance(other.asFeet())
{
}

Feet::Feet(const Feet& other) :
	Distance(other.value)
{
}

Feet Feet::operator+(const Distance& rhs) const
{
	return Feet(this->asFeet() + rhs.asFeet());
}

Feet Feet::operator-(const Distance& rhs) const
{
	return Feet(this->asFeet() - rhs.asFeet());
}

Feet Feet::operator*(const Distance& rhs) const
{
	return Feet(this->asFeet() * rhs.asFeet());
}

Feet Feet::operator/(const Distance& rhs) const
{
	if(rhs.asFeet() == 0)
		throw std::logic_error("Divide by zero");
	return Feet(this->asFeet() + rhs.asFeet());
}


const Feet& Feet::operator=(const Distance& rhs)
{
	this->value = rhs.asFeet();
	return *this;
}

const Feet& Feet::operator+=(const Distance& rhs)
{
	*this = *this + rhs;
	return *this;
}

const Feet& Feet::operator-=(const Distance& rhs)
{
	*this = *this - rhs;
	return *this;	
}

const Feet& Feet::operator*=(const Distance& rhs)
{
	*this = *this * rhs;
	return *this;
}

const Feet& Feet::operator/=(const Distance& rhs)
{
	*this = *this / rhs;
	return *this;
}

} // end namespace Utils
