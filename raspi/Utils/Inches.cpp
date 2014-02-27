#include "Inches.hpp"

namespace Utils {

Inches::Inches(float value) :
	Distance(value)
{
	int div = static_cast<int>(this->value / 360.0f);
	this->value = this->value - (div*360.0f);
}

Inches::Inches(const Distance& other) :
	Distance(other)
{
	int div = static_cast<int>(this->asInches() / 360.0f);
	this->value = this->asInches() - (div*360.0f);
}

Inches::Inches(const Inches& other) :
	Distance(other)
{
	int div = static_cast<int>(this->value / 360.0f);
	this->value = this->value - (div*360.0f);
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
	return Inches(this->asInches() + rhs.asInches());
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
