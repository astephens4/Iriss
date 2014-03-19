#ifndef IRISS_DISTANCE_H
#define IRISS_DISTANCE_H 1

#include <stdexcept>
#include "FloatEquals.hpp"

namespace Utils {

class Distance
{
public:
	Distance(float value);
	Distance(const Distance& other);
	virtual ~Distance();
	virtual float asFeet() const = 0;
	virtual float asInches() const = 0;

	// Equality
	bool operator==(const Distance& rhs) const;
	bool operator!=(const Distance& rhs) const;
	bool operator<(const Distance& rhs) const;
	bool operator<=(const Distance& rhs) const;
	bool operator>(const Distance& rhs) const;
	bool operator>=(const Distance& rhs) const;
protected:
	Distance() { throw std::logic_error("No argument construction is not allowed"); }
	float value;
};

} // end namespace Utils
#endif // IRISS_DISTANCE_H
