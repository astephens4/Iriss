#ifndef MATH_PT3_H
#define MATH_PT3_H 1

#include <sstream>
#include "Math/tuple.hpp"

namespace Math {

template <typename T>
class pt3 : public Tuple<T,3>
{
public:
	pt3<T>();
	pt3<T>(const pt3<T>& other);
	pt3<T>( T x, T y, T z );
	pt3<T>(T* data);
	virtual ~pt3<T>() {}
	T distSq(const pt3& other) const;
	T dist(const pt3& other) const;
	T x() const { return this->m_data[0]; }
	T y() const { return this->m_data[1]; }
	T z() const { return this->m_data[2]; }
	virtual std::string ToString() const
	{
		std::stringstream sstream;
		sstream << "3D Point {(x=" << x() << ",y=" << y() << ",z=" << z() << ")}";
		return sstream.str();
	}

	const pt3<T>& scale(T value);
	const pt3<T>& operator=(pt3<T> other);

	// A whole slew of mother effing operators
	const pt3<T> operator+(const pt3<T>& rhs) const;
	const pt3<T> operator-(const pt3<T>& rhs) const;
	const pt3<T>& operator+=(const pt3<T>& rhs);
	const pt3<T>& operator-=(const pt3<T>& rhs);
};

template <typename T>
pt3<T>::pt3() : Tuple<T, 3>()
{

}

template <typename T>
pt3<T>::pt3(const pt3<T>& other) : Tuple<T, 3>(other)
{

}

template <typename T>
pt3<T>::pt3( T x, T y, T z )
{
	this->m_data[0] = x;
	this->m_data[1] = y;
	this->m_data[2] = z;
}

template <typename T>
pt3<T>::pt3(T* data) : Tuple<T, 3>(data)
{

}

template <typename T>
T pt3<T>::distSq(const pt3& other) const
{
	T ret = T((this->m_data[0] - other.m_data[0])*
			(this->m_data[0] - other.m_data[0]));
	for(unsigned int i = 1; i < 3; ++i) {
		ret += T((this->m_data[i] - other.m_data[i])*
				 (this->m_data[i] - other.m_data[i]));
	}
	return ret;
}

template <typename T>
T pt3<T>::dist(const pt3& other) const
{
	return T(sqrt(distSq(other)));
}

template <typename T>
const pt3<T>& pt3<T>::scale(T value)
{
	for(unsigned int i = 0; i < 3; ++i)
		this->m_data[i] = this->m_data[i] * value;
	return *this;
}

template <typename T>
const pt3<T>& pt3<T>::operator=(pt3<T> other)
{
	T tmp;
	for(unsigned int i = 0; i < 3; ++i) {
		tmp = other.m_data[i];
		other.m_data[i] = this->m_data[i];
		this->m_data[i] = tmp;
	}
	return *this;
}

template <typename T>
const pt3<T> pt3<T>::operator+(const pt3<T>& rhs) const
{
	return pt3<T>(this->m_data[0] + rhs.m_data[0],
					 this->m_data[1] + rhs.m_data[1],
					 this->m_data[2] + rhs.m_data[2]);
}

template <typename T>
const pt3<T> pt3<T>::operator-(const pt3<T>& rhs) const
{
	return pt3<T>(this->m_data[0] - rhs.m_data[0],
					 this->m_data[1] - rhs.m_data[1],
					 this->m_data[2] - rhs.m_data[2]);
}

template <typename T>
const pt3<T>& pt3<T>::operator+=(const pt3<T>& rhs)
{
	*this = *this + rhs;
	return *this;
}

template <typename T>
const pt3<T>& pt3<T>::operator-=(const pt3<T>& rhs)
{
	*this = *this - rhs;
	return *this;
}

typedef pt3<float> pt3f;
typedef pt3<double> pt3d;
typedef pt3<int> pt3i;

} // end namespace apl

#endif
