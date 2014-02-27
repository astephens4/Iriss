#include "Angle.hpp"

namespace Utils {

Angle::Angle(float value) :
	value(value)
{

}

Angle::Angle(const Angle& other) :
	value(other.value)
{

}

Angle::~Angle()
{

}

bool Angle::operator==(const Angle& rhs)
{
	return this->asRadians() == rhs.asRadians();
}

bool Angle::operator!=(const Angle& rhs)
{
	return !this->operator==(rhs);
}

bool Angle::operator<(const Angle& rhs)
{
	return this->asRadians() < rhs.asRadians();
}

bool Angle::operator<=(const Angle& rhs)
{
	return this->asRadians() <= rhs.asRadians();
}

bool Angle::operator>(const Angle& rhs)
{
	return !this->operator<=(rhs);
}

bool Angle::operator>=(const Angle& rhs)
{
	return !this->operator<(rhs);
}

} // end namespace Utils
