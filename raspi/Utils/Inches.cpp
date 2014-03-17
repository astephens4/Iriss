#include "Inches.hpp"

namespace Utils {

Inches::Inches(float value) :
	Distance(value)
{
}

Inches::Inches(const Distance& other) :
	Distance(other.asInches())
{
}

Inches::Inches(const Inches& other) :
	Distance(other)
{
}

Inches Inches::operator+(const Distance& rhs) const
{
	return Inches(this->asInches() + rhs.asInches());
}

Inches Inches::operator-(const Distance& rhs) const
{
	return Inches(this->asInches() - rhs.asInches());
}

Inches Inches::operator*(const Distance& rhs) const
{
	return Inches(this->asInches() * rhs.asInches());
}

Inches Inches::operator/(const Distance& rhs) const
{
	if(rhs.asInches() == 0)
		throw std::logic_error("Divide by zero");
	return Inches(this->asInches() / rhs.asInches());
}


const Inches& Inches::operator=(const Distance& rhs)
{
	this->value = rhs.asInches();
    return *this;
}

const Inches& Inches::operator+=(const Distance& rhs)
{
	*this = *this + rhs;
	return *this;
}

const Inches& Inches::operator-=(const Distance& rhs)
{
	*this = *this - rhs;
	return *this;	
}

const Inches& Inches::operator*=(const Distance& rhs)
{
	*this = *this * rhs;
	return *this;
}

const Inches& Inches::operator/=(const Distance& rhs)
{
	*this = *this / rhs;
	return *this;
}

} // end namespace Utils
