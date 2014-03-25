#ifndef MATH_PT2_H
#define MATH_PT2_H 1

#include <sstream>
#include "Math/tuple.hpp"

namespace Math {

template <typename T>
class pt2 : public Tuple<T,2>
{
public:
	pt2<T>();
	pt2<T>(const pt2<T>& other);
	pt2<T>( T x, T y );
	pt2<T>(T* data);
	virtual ~pt2<T>() {}
	T distSq(const pt2& other) const;
	T dist(const pt2& other) const;
	T x() const { return this->m_data[0]; }
	T y() const { return this->m_data[1]; }
	virtual std::string ToString() const
	{
		std::stringstream sstream;
		sstream << "2D Point {(x=" << x() << ",y=" << y() << ")}";
		return sstream.str();
	}

	const pt2<T>& scale(T value);
	const pt2<T>& operator=(pt2<T> other);

	// A whole slew of mother effing operators
	const pt2<T> operator+(const pt2<T>& rhs) const;
	const pt2<T> operator-(const pt2<T>& rhs) const;
	const pt2<T>& operator+=(const pt2<T>& rhs);
	const pt2<T>& operator-=(const pt2<T>& rhs);
};

template <typename T>
pt2<T>::pt2() : Tuple<T, 2>()
{

}

template <typename T>
pt2<T>::pt2(const pt2<T>& other) : Tuple<T, 2>(other)
{

}

template <typename T>
pt2<T>::pt2( T x, T y )
{
	this->m_data[0] = x;
	this->m_data[1] = y;
}

template <typename T>
pt2<T>::pt2(T* data) : Tuple<T, 2>(data)
{

}

template <typename T>
T pt2<T>::distSq(const pt2& other) const
{
	T ret = T((this->m_data[0] - other.m_data[0])*
			(this->m_data[0] - other.m_data[0]));
	for(unsigned int i = 1; i < 2; ++i) {
		ret += T((this->m_data[i] - other.m_data[i])*
				 (this->m_data[i] - other.m_data[i]));
	}
	return ret;
}

template <typename T>
T pt2<T>::dist(const pt2& other) const
{
	return T(sqrt(distSq(other)));
}

template <typename T>
const pt2<T>& pt2<T>::scale(T value)
{
	for(unsigned int i = 0; i < 2; ++i)
		this->m_data[i] = this->m_data[i] * value;
	return *this;
}

template <typename T>
const pt2<T>& pt2<T>::operator=(pt2<T> other)
{
	T tmp;
	for(unsigned int i = 0; i < 2; ++i) {
		tmp = other.m_data[i];
		other.m_data[i] = this->m_data[i];
		this->m_data[i] = tmp;
	}
	return *this;
}

template <typename T>
const pt2<T> pt2<T>::operator+(const pt2<T>& rhs) const
{
	return pt2<T>(this->m_data[0] + rhs.m_data[0],
					 this->m_data[1] + rhs.m_data[1]);
}

template <typename T>
const pt2<T> pt2<T>::operator-(const pt2<T>& rhs) const
{
	return pt2<T>(this->m_data[0] - rhs.m_data[0],
					 this->m_data[1] - rhs.m_data[1]);
}

template <typename T>
const pt2<T>& pt2<T>::operator+=(const pt2<T>& rhs)
{
	*this = *this + rhs;
	return *this;
}

template <typename T>
const pt2<T>& pt2<T>::operator-=(const pt2<T>& rhs)
{
	*this = *this - rhs;
	return *this;
}

typedef pt2<float> pt2f;
typedef pt2<double> pt2d;
typedef pt2<int> pt2i;

} // end namespace Math

#endif
