#ifndef IRISS_INCHES_H
#define IRISS_INCHES_H 1

#include "Utils/Distance.hpp"

namespace Utils {

class Inches : public Distance
{
public:
	Inches(float value);
	Inches(const Distance& other);
	Inches(const Inches& other);
	virtual ~Inches() {}
	virtual float asInches() const { return this->value; }
	virtual float asFeet() const { return this->value/12.0f; }

	Inches operator+(const Distance& rhs) const;
	Inches operator-(const Distance& rhs) const;
	Inches operator*(const Distance& rhs) const;
	Inches operator/(const Distance& rhs) const;

	const Inches& operator=(const Distance& rhs);
	const Inches& operator+=(const Distance& rhs);
	const Inches& operator-=(const Distance& rhs);
	const Inches& operator*=(const Distance& rhs);
	const Inches& operator/=(const Distance& rhs);

private:
	Inches() { throw std::logic_error("No argument constructor is not allowed"); }
};

} // end namespace Utils

#endif // IRISS_INCHES_H
