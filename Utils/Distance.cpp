#include "Distance.hpp"

namespace Utils {

Distance::Distance(float value) :
	value(value)
{

}

Distance::Distance(const Distance& other) :
	value(other.value)
{

}

Distance::~Distance()
{

}

bool Distance::operator==(const Distance& rhs) const
{
	return FloatEquals(this->asInches(), rhs.asInches());
}

bool Distance::operator!=(const Distance& rhs) const
{
	return !this->operator==(rhs);
}

bool Distance::operator<(const Distance& rhs) const
{
	return this->asInches() < rhs.asInches();
}

bool Distance::operator<=(const Distance& rhs) const
{
	return this->operator<(rhs) ||
           this->operator==(rhs);
}

bool Distance::operator>(const Distance& rhs) const
{
	return !this->operator<=(rhs);
}

bool Distance::operator>=(const Distance& rhs) const
{
	return !this->operator<(rhs);
}

} // end namespace Utils
