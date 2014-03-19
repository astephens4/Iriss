#ifndef IRISS_FEET_H
#define IRISS_FEET_H 1

#include "Utils/Distance.hpp"

namespace Utils {

class Feet : public Distance
{
public:
	Feet(float value);
	Feet(const Distance& other);
	Feet(const Feet& other);
	virtual ~Feet() {}
	virtual float asInches() const { return this->value*12.0f; }
	virtual float asFeet() const { return this->value; }

	Feet operator+(const Distance& rhs) const;
	Feet operator-(const Distance& rhs) const;
	Feet operator*(const Distance& rhs) const;
	Feet operator/(const Distance& rhs) const;

	const Feet& operator=(const Distance& rhs);
	const Feet& operator+=(const Distance& rhs);
	const Feet& operator-=(const Distance& rhs);
	const Feet& operator*=(const Distance& rhs);
	const Feet& operator/=(const Distance& rhs);

private:
	Feet() { throw std::logic_error("No argument constructor is not allowed"); }
};

} // end namespace Utils

#endif // IRISS_FEET_H
