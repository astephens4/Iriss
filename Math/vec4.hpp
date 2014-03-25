#ifndef MATH_VEC4_H
#define MATH_VEC4_H 1

#include <sstream>
#include "Math/tuple.hpp"

namespace Math {

template <typename T>
class vec4 : public Tuple<T,4>
{
public:
	vec4<T>();
	vec4<T>(const vec4<T>& other);
	vec4<T>( T x, T y, T z, T w );
	vec4<T>(T* data);
	virtual ~vec4<T>() {}
	T mag() const;
	const vec4<T>& norm();

	T x() const { return this->m_data[0]; }
	T y() const { return this->m_data[1]; }
	T z() const { return this->m_data[2]; }
	T w() const { return this->m_data[3]; }
	const vec4<T>& scale(T value);
	const vec4<T>& operator=(vec4<T> other);

	virtual std::string ToString() const
	{
		std::stringstream sstream;
		sstream << "4D Vector {(x=" << x() << ",y=" << y() << ",z=" << z() << ",w=" << w() << ")}";
		return sstream.str();
	}

	// A whole slew of mother effing operators
	vec4<T> operator+(const vec4<T>& rhs) const;
	vec4<T> operator-(const vec4<T>& rhs) const;
	const vec4<T>& operator+=(const vec4<T>& rhs);
	const vec4<T>& operator-=(const vec4<T>& rhs);
	T operator*(const vec4<T>& rhs) const;
	vec4<T> operator^(const vec4<T>& rhs) const;

};

template <typename T>
vec4<T>::vec4() : Tuple<T, 4>()
{

}

template <typename T>
vec4<T>::vec4(const vec4<T>& other) : Tuple<T, 4>(other)
{

}

template <typename T>
vec4<T>::vec4( T x, T y, T z, T w )
{
	this->m_data[0] = x;
	this->m_data[1] = y;
	this->m_data[2] = z;
	this->m_data[3] = w;
}

template <typename T>
vec4<T>::vec4(T* data) : Tuple<T, 4>(data)
{

}

/**
 * Get the magnitude (length) of this vector.
 */
template <typename T>
T vec4<T>::mag() const
{
	T ret = T(this->m_data[0]*this->m_data[0]);
	ret = ret + T(this->m_data[1]*this->m_data[1]);
	ret = ret + T(this->m_data[2]*this->m_data[2]);
	ret = ret + T(this->m_data[3]*this->m_data[3]);
	return sqrt(ret);
}

template <typename T>
const vec4<T>& vec4<T>::scale(T value)
{
	for(unsigned int i = 0; i < 4; ++i)
		this->m_data[i] = this->m_data[i] * value;
	return *this;
}

template <typename T>
const vec4<T>& vec4<T>::operator=(vec4<T> other)
{
	T tmp;
	for(unsigned int i = 0; i < 4; ++i) {
		tmp = other.m_data[i];
		other.m_data[i] = this->m_data[i];
		this->m_data[i] = tmp;
	}
	return *this;
}
/**
 * Normalize this vector. Returns a reference to this
 * so it can return a value for use in functions.
 */
template <typename T>
const vec4<T>& vec4<T>::norm()
{
	T m = this->mag();
	m = 1.0f/(float)m;
	return this->scale(m);
}

template<typename T>
vec4<T> vec4<T>::operator+(const vec4<T>& rhs) const
{
    return vec4<T>(this->m_data[0] + rhs.m_data[0],
                   this->m_data[1] + rhs.m_data[1],
                   this->m_data[2] + rhs.m_data[2],
                   this->m_data[3] + rhs.m_data[3]);
}

template <typename T>
vec4<T> vec4<T>::operator-(const vec4<T>& rhs) const
{
    return vec4<T>(this->m_data[0] - rhs.m_data[0],
                   this->m_data[1] - rhs.m_data[1],
                   this->m_data[2] - rhs.m_data[2],
                   this->m_data[3] - rhs.m_data[3]);
}

template <typename T>
const vec4<T>& vec4<T>::operator+=(const vec4<T>& rhs)
{
    this->m_data[0] = this->m_data[0] + rhs.m_data[0],
    this->m_data[1] = this->m_data[1] + rhs.m_data[1];
    this->m_data[2] = this->m_data[2] + rhs.m_data[2];
    this->m_data[3] = this->m_data[3] + rhs.m_data[3];
    return *this;
}

template <typename T>
const vec4<T>& vec4<T>::operator-=(const vec4<T>& rhs)
{
    this->m_data[0] = this->m_data[0] - rhs.m_data[0],
    this->m_data[1] = this->m_data[1] - rhs.m_data[1];
    this->m_data[2] = this->m_data[2] - rhs.m_data[2];
    this->m_data[3] = this->m_data[3] - rhs.m_data[3];
    return *this;
}


/**
 * Dot product
 */
template <typename T>
T vec4<T>::operator*(const vec4<T>& rhs) const
{
	return T(this->m_data[0]*rhs.m_data[0] + 
			 this->m_data[1]*rhs.m_data[1] +
			 this->m_data[2]*rhs.m_data[2] +
             this->m_data[3]*rhs.m_data[3]);
}

/**
 * Cross product
 */
template <typename T>
vec4<T> vec4<T>::operator^(const vec4<T>& rhs) const
{
	return vec4<T>(this->m_data[1]*rhs.m_data[2] -
						this->m_data[2]*rhs.m_data[1],
					  this->m_data[2]*rhs.m_data[0] -
						this->m_data[0]*rhs.m_data[2],
					  this->m_data[0]*rhs.m_data[1] -
						this->m_data[1]*rhs.m_data[0],
                      (T)1);
}

typedef vec4<float> vec4f;
typedef vec4<int> vec4i;
typedef vec4<double> vec4d;

} // end namespace Math

#endif // MATH_VEC4_H
