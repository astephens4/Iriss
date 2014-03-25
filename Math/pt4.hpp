#ifndef MATH_PT4_H
#define MATH_PT4_H 1

#include <sstream>
#include "Math/tuple.hpp"

namespace Math {

template <typename T>
class pt4 : public Tuple<T,4>
{
public:
	pt4<T>();
	pt4<T>(const pt4<T>& other);
	pt4<T>( T x, T y, T z );
	pt4<T>( T x, T y, T z , T w );
	pt4<T>(T* data);
	virtual ~pt4<T>() {}
	T distSq(const pt4& other) const;
	T dist(const pt4& other) const;
	T x() const { return this->m_data[0]; }
	T y() const { return this->m_data[1]; }
	T z() const { return this->m_data[2]; }
	T w() const { return this->m_data[3]; }
	virtual std::string ToString() const
	{
		std::stringstream sstream;
		sstream << "4D Point {(x=" << x() << ",y=" << y() << ",z=" << z() << ",w=" << w() << ")}";
		return sstream.str();
	}

	const pt4<T>& scale(T value);
	const pt4<T>& operator=(pt4<T> other);

	// A whole slew of mother effing operators
	const pt4<T> operator+(const pt4<T>& rhs) const;
	const pt4<T> operator-(const pt4<T>& rhs) const;
	const pt4<T>& operator+=(const pt4<T>& rhs);
	const pt4<T>& operator-=(const pt4<T>& rhs);
};

template <typename T>
pt4<T>::pt4() : Tuple<T, 4>()
{

}

template <typename T>
pt4<T>::pt4(const pt4<T>& other) : Tuple<T, 4>(other)
{

}

template <typename T>
pt4<T>::pt4( T x, T y, T z )
{
	this->m_data[0] = x;
	this->m_data[1] = y;
	this->m_data[2] = z;
    this->m_data[3] = (T)0;
}

template <typename T>
pt4<T>::pt4( T x, T y, T z, T w )
{
	this->m_data[0] = x;
	this->m_data[1] = y;
	this->m_data[2] = z;
    this->m_data[3] = w;
}

template <typename T>
pt4<T>::pt4(T* data) : Tuple<T, 4>(data)
{

}

template <typename T>
T pt4<T>::distSq(const pt4& other) const
{
	T ret = T((this->m_data[0] - other.m_data[0])*
			(this->m_data[0] - other.m_data[0]));
	for(unsigned int i = 1; i < 4; ++i) {
		ret += T((this->m_data[i] - other.m_data[i])*
				 (this->m_data[i] - other.m_data[i]));
	}
	return ret;
}

template <typename T>
T pt4<T>::dist(const pt4& other) const
{
	return T(sqrt(distSq(other)));
}

template <typename T>
const pt4<T>& pt4<T>::scale(T value)
{
	for(unsigned int i = 0; i < 4; ++i)
		this->m_data[i] = this->m_data[i] * value;
	return *this;
}

template <typename T>
const pt4<T>& pt4<T>::operator=(pt4<T> other)
{
	T tmp;
	for(unsigned int i = 0; i < 4; ++i) {
		tmp = other.m_data[i];
		other.m_data[i] = this->m_data[i];
		this->m_data[i] = tmp;
	}
	return *this;
}

template <typename T>
const pt4<T> pt4<T>::operator+(const pt4<T>& rhs) const
{
	return pt4<T>(this->m_data[0] + rhs.m_data[0],
					 this->m_data[1] + rhs.m_data[1],
					 this->m_data[2] + rhs.m_data[2],
                     this->m_data[3] + rhs.m_data[3]);
}

template <typename T>
const pt4<T> pt4<T>::operator-(const pt4<T>& rhs) const
{
	return pt4<T>(this->m_data[0] - rhs.m_data[0],
					 this->m_data[1] - rhs.m_data[1],
					 this->m_data[2] - rhs.m_data[2],
                     this->m_data[3] - rhs.m_data[3]);
}

template <typename T>
const pt4<T>& pt4<T>::operator+=(const pt4<T>& rhs)
{
	*this = *this + rhs;
	return *this;
}

template <typename T>
const pt4<T>& pt4<T>::operator-=(const pt4<T>& rhs)
{
	*this = *this - rhs;
	return *this;
}

typedef pt4<float> pt4f;
typedef pt4<double> pt4d;
typedef pt4<int> pt4i;

} // end namespace Math

#endif // MATH_PT4_H
