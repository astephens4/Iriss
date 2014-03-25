#ifndef MATH_VEC2_H
#define MATH_VEC2_H 1

#include <sstream>
#include "Math/tuple.hpp"

namespace Math {

template <typename T>
class vec2 : public Tuple<T,2>
{
public:
	vec2<T>();
	vec2<T>(const vec2<T>& other);
	vec2<T>( T x, T y );
	vec2<T>(T* data);
	virtual ~vec2<T>() {}
	T mag() const;
	const vec2<T>& norm();
	T x() const { return this->m_data[0]; }
	T y() const { return this->m_data[1]; }

	virtual std::string ToString() const
	{
		std::stringstream sstream;
		sstream << "2D Vector {(x=" << x() << ",y=" << y() << ")}";
		return sstream.str();
	}

	const vec2<T>& scale(T value);
	const vec2<T>& operator=(vec2<T> other);
	// A whole slew of mother effing operators
	vec2<T> operator+(const vec2<T>& rhs) const;
	vec2<T> operator-(const vec2<T>& rhs) const;
	const vec2<T>& operator+=(const vec2<T>& rhs);
	const vec2<T>& operator-=(const vec2<T>& rhs);
	T operator*(const vec2<T>& rhs) const;
	T operator^(const vec2<T>& rhs) const;

};

template <typename T>
vec2<T>::vec2() : Tuple<T, 2>()
{

}

template <typename T>
vec2<T>::vec2(const vec2<T>& other) : Tuple<T, 2>(other)
{

}

template <typename T>
vec2<T>::vec2( T x, T y )
{
	this->m_data[0] = x;
	this->m_data[1] = y;
}

template <typename T>
vec2<T>::vec2(T* data) : Tuple<T, 2>(data)
{

}

/**
 * Get the magnitude (length) of this vector.
 */
template <typename T>
T vec2<T>::mag() const
{
	T ret = T(this->m_data[0]*this->m_data[0]);
	ret = ret + T(this->m_data[1]*this->m_data[1]);
	return sqrt(ret);
}

/**
 * Normalize this vector. Returns a reference to this
 * so it can return a value for use in functions.
 */
template <typename T>
const vec2<T>& vec2<T>::norm()
{
	T m = this->mag();
	m = 1.0f/(float)m;
	return this->scale(m);
}

template <typename T>
const vec2<T>& vec2<T>::scale(T value)
{
	for(unsigned int i = 0; i < 2; ++i)
		this->m_data[i] = this->m_data[i] * value;
	return *this;
}

template <typename T>
const vec2<T>& vec2<T>::operator=(vec2<T> other)
{
	T tmp;
	for(unsigned int i = 0; i < 2; ++i) {
		tmp = other.m_data[i];
		other.m_data[i] = this->m_data[i];
		this->m_data[i] = tmp;
	}
	return *this;
}

template<typename T>
vec2<T> vec2<T>::operator+(const vec2<T>& rhs) const
{
    return vec2<T>(this->m_data[0] + rhs.m_data[0],
                   this->m_data[1] + rhs.m_data[1]);
}

template <typename T>
vec2<T> vec2<T>::operator-(const vec2<T>& rhs) const
{
    return vec2<T>(this->m_data[0] - rhs.m_data[0],
                   this->m_data[1] - rhs.m_data[1]);
}

template <typename T>
const vec2<T>& vec2<T>::operator+=(const vec2<T>& rhs)
{
    this->m_data[0] = this->m_data[0] + rhs.m_data[0],
    this->m_data[1] = this->m_data[1] + rhs.m_data[1];
    return *this;
}

template <typename T>
const vec2<T>& vec2<T>::operator-=(const vec2<T>& rhs)
{
    this->m_data[0] = this->m_data[0] - rhs.m_data[0],
    this->m_data[1] = this->m_data[1] - rhs.m_data[1];
    return *this;
}


/**
 * Dot product
 */
template <typename T>
T vec2<T>::operator*(const vec2<T>& rhs) const
{
	return T(this->m_data[0]*rhs.m_data[0] + 
			 this->m_data[1]*rhs.m_data[1]);
}

/**
 * Returns the Z component of the cross product of
 * this and rhs with 0 for the z component.
 */
template <typename T>
T vec2<T>::operator^(const vec2<T>& rhs) const
{
	return T(this->m_data[0]*rhs.m_data[1] -
			 this->m_data[1]*rhs.m_data[0]);
}

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int> vec2i;

} // end namespace Math

#endif // MATH_VEC2_H
