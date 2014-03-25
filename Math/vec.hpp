#ifndef APL_VEC_H
#define APL_VEC_H 1

#include <sstream>
#include "tuple.hpp"

namespace apl {
namespace math {

template <typename T, unsigned int N>
class vec : public Tuple<T,N>
{
public:
	vec<T>();
	vec<T>(const vec<T>& other);
	vec<T>( T x, T y, T z );
	vec<T>(T* data);
	virtual ~vec<T>() {}
	T mag() const;
	const vec<T>& norm();

	T x() const { return this->m_data[0]; }
	T y() const { return this->m_data[1]; }
	T z() const { return this->m_data[2]; }
	const vec<T>& scale(T value);
	const vec<T>& operator=(vec<T> other);

	virtual std::string ToString() const
	{
		std::stringstream sstream;
		sstream << "3D Vector {(x=" << x() << ",y=" << y() << ",z=" << z() << ")}";
		return sstream.str();
	}

	// A whole slew of mother effing operators
	vec<T> operator+(const vec<T>& rhs) const;
	vec<T> operator-(const vec<T>& rhs) const;
	const vec<T>& operator+=(const vec<T>& rhs);
	const vec<T>& operator-=(const vec<T>& rhs);
	T operator*(const vec<T>& rhs) const;
	vec<T> operator^(const vec<T>& rhs) const;

};

template <typename T>
vec<T>::vec() : Tuple<T, 3>()
{

}

template <typename T>
vec<T>::vec(const vec<T>& other) : Tuple<T, 3>(other)
{

}

template <typename T>
vec<T>::vec( T x, T y, T z )
{
	this->m_data[0] = x;
	this->m_data[1] = y;
	this->m_data[2] = z;
}

template <typename T>
vec<T>::vec(T* data) : Tuple<T, 3>(data)
{

}

/**
 * Get the magnitude (length) of this vector.
 */
template <typename T>
T vec<T>::mag() const
{
	T ret = T(this->m_data[0]*this->m_data[0]);
	ret = ret + T(this->m_data[1]*this->m_data[1]);
	ret = ret + T(this->m_data[2]*this->m_data[2]);
	return sqrt(ret);
}

template <typename T>
const vec<T>& vec<T>::scale(T value)
{
	for(unsigned int i = 0; i < 3; ++i)
		this->m_data[i] = this->m_data[i] * value;
	return *this;
}

template <typename T>
const vec<T>& vec<T>::operator=(vec<T> other)
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
const vec<T>& vec<T>::norm()
{
	T m = this->mag();
	m = 1.0f/(float)m;
	return this->scale(m);
}

template<typename T>
vec<T> vec<T>::operator+(const vec<T>& rhs) const
{
    vec<T, N> ret;
    for(unsigned int i = 0; i < N; ++i)
        ret.m_data[i] = this->m_data[i] - rhs.m_data[i],
    return *this;
}

template <typename T>
vec<T> vec<T>::operator-(const vec<T>& rhs) const
{
    vec<T, N> ret;
    for(unsigned int i = 0; i < N; ++i)
        ret.m_data[i] = this->m_data[i] - rhs.m_data[i],
    return *this;
}

template <typename T>
const vec<T>& vec<T>::operator+=(const vec<T>& rhs)
{
    for(unsigned int i = 0; i < N; ++i)
        this->m_data[i] = this->m_data[i] + rhs.m_data[i],
    return *this;
}

template <typename T>
const vec<T>& vec<T>::operator-=(const vec<T>& rhs)
{
    for(unsigned int i = 0; i < N; ++i)
        this->m_data[i] = this->m_data[i] - rhs.m_data[i],
    return *this;
}


/**
 * Dot product
 */
template <typename T>
T vec<T>::operator*(const vec<T>& rhs) const
{
	T ret = 0;
    for(unsigned int i = 0; i < N; ++i)
        ret += this->m_data[i]*rhs.m_data[i];
    return ret;
}

} // end namespace math
} // and namespace apl

#endif // APL_VEC_H
