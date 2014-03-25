#ifndef MATH_VEC3_H
#define MATH_VEC3_H 1

#include <sstream>
#include "Math/tuple.hpp"

namespace Math {

template <typename T>
class vec3 : public Tuple<T,3>
{
public:
	vec3<T>();
	vec3<T>(const vec3<T>& other);
	vec3<T>( T x, T y, T z );
	vec3<T>(T* data);
	virtual ~vec3<T>() {}
	T mag() const;
	const vec3<T>& norm();

	T x() const { return this->m_data[0]; }
	T y() const { return this->m_data[1]; }
	T z() const { return this->m_data[2]; }
	const vec3<T>& scale(T value);
	const vec3<T>& operator=(vec3<T> other);

	virtual std::string ToString() const
	{
		std::stringstream sstream;
		sstream << "3D Vector {(x=" << x() << ",y=" << y() << ",z=" << z() << ")}";
		return sstream.str();
	}

	// A whole slew of mother effing operators
	vec3<T> operator+(const vec3<T>& rhs) const;
	vec3<T> operator-(const vec3<T>& rhs) const;
	const vec3<T>& operator+=(const vec3<T>& rhs);
	const vec3<T>& operator-=(const vec3<T>& rhs);
	T operator*(const vec3<T>& rhs) const;
	vec3<T> operator^(const vec3<T>& rhs) const;

};

template <typename T>
vec3<T>::vec3() : Tuple<T, 3>()
{

}

template <typename T>
vec3<T>::vec3(const vec3<T>& other) : Tuple<T, 3>(other)
{

}

template <typename T>
vec3<T>::vec3( T x, T y, T z )
{
	this->m_data[0] = x;
	this->m_data[1] = y;
	this->m_data[2] = z;
}

template <typename T>
vec3<T>::vec3(T* data) : Tuple<T, 3>(data)
{

}

/**
 * Get the magnitude (length) of this vector.
 */
template <typename T>
T vec3<T>::mag() const
{
	T ret = T(this->m_data[0]*this->m_data[0]);
	ret = ret + T(this->m_data[1]*this->m_data[1]);
	ret = ret + T(this->m_data[2]*this->m_data[2]);
	return sqrt(ret);
}

template <typename T>
const vec3<T>& vec3<T>::scale(T value)
{
	for(unsigned int i = 0; i < 3; ++i)
		this->m_data[i] = this->m_data[i] * value;
	return *this;
}

template <typename T>
const vec3<T>& vec3<T>::operator=(vec3<T> other)
{
	T tmp;
	for(unsigned int i = 0; i < 3; ++i) {
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
const vec3<T>& vec3<T>::norm()
{
	T m = this->mag();
	m = 1.0f/(float)m;
	return this->scale(m);
}

template<typename T>
vec3<T> vec3<T>::operator+(const vec3<T>& rhs) const
{
    return vec3<T>(this->m_data[0] + rhs.m_data[0],
                   this->m_data[1] + rhs.m_data[1],
                   this->m_data[2] + rhs.m_data[2]);
}

template <typename T>
vec3<T> vec3<T>::operator-(const vec3<T>& rhs) const
{
    return vec3<T>(this->m_data[0] - rhs.m_data[0],
                   this->m_data[1] - rhs.m_data[1],
                   this->m_data[2] - rhs.m_data[2]);
}

template <typename T>
const vec3<T>& vec3<T>::operator+=(const vec3<T>& rhs)
{
    this->m_data[0] = this->m_data[0] + rhs.m_data[0],
    this->m_data[1] = this->m_data[1] + rhs.m_data[1];
    this->m_data[2] = this->m_data[2] + rhs.m_data[2];
    return *this;
}

template <typename T>
const vec3<T>& vec3<T>::operator-=(const vec3<T>& rhs)
{
    this->m_data[0] = this->m_data[0] - rhs.m_data[0],
    this->m_data[1] = this->m_data[1] - rhs.m_data[1];
    this->m_data[2] = this->m_data[2] - rhs.m_data[2];
    return *this;
}


/**
 * Dot product
 */
template <typename T>
T vec3<T>::operator*(const vec3<T>& rhs) const
{
	return T(this->m_data[0]*rhs.m_data[0] + 
			 this->m_data[1]*rhs.m_data[1] +
			 this->m_data[2]*rhs.m_data[2]);
}

/**
 * Cross product
 */
template <typename T>
vec3<T> vec3<T>::operator^(const vec3<T>& rhs) const
{
	return vec3<T>(this->m_data[1]*rhs.m_data[2] -
						this->m_data[2]*rhs.m_data[1],
					  this->m_data[2]*rhs.m_data[0] -
						this->m_data[0]*rhs.m_data[2],
					  this->m_data[0]*rhs.m_data[1] -
						this->m_data[1]*rhs.m_data[0]);
}

typedef vec3<float> vec3f;
typedef vec3<int> vec3i;
typedef vec3<double> vec3d;

} // end namespace Math

#endif // MATH_VEC3_H
