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

bool Angle::operator==(const Angle& rhs) const
{
	return FloatEquals(this->asRadians(), rhs.asRadians());
}

bool Angle::operator!=(const Angle& rhs) const
{
	return !this->operator==(rhs);
}

bool Angle::operator<(const Angle& rhs) const
{
	return this->asRadians() < rhs.asRadians();
}

bool Angle::operator<=(const Angle& rhs) const
{
	return this->operator<(rhs) ||
           this->operator==(rhs);
}

bool Angle::operator>(const Angle& rhs) const
{
	return !this->operator<=(rhs);
}

bool Angle::operator>=(const Angle& rhs) const
{
	return !this->operator<(rhs);
}

} // end namespace Utils
