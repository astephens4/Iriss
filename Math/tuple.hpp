#ifndef MATH_TUPLE_H
#define MATH_TUPLE_H 1

#include <cmath>

namespace Math {

/*!
 * \brief Represents a generic tuple. Only used as
 * a base for other math types.
 */
template <typename T, unsigned int N>
class Tuple
{
public:
	Tuple<T, N>();
	Tuple<T, N>(const Tuple& other);
	Tuple<T, N>(T* data);
	virtual ~Tuple<T, N>() = 0;
	virtual const Tuple<T,N>& scale(T value);
    virtual const Tuple<T,N>& operator=(const Tuple<T,N>& rhs);
	bool operator==(const Tuple<T,N>& rhs) const;
	bool operator!=(const Tuple<T,N>& rhs) const;
	bool operator<(const Tuple<T,N>& rhs) const;
	bool operator<=(const Tuple<T,N>& rhs) const;
	bool operator>(const Tuple<T,N>& rhs) const;
	bool operator>=(const Tuple<T,N>& rhs) const;
protected:
	T m_data[N];
};

template <typename T, unsigned int N>
Tuple<T, N>::Tuple()
{
	for(unsigned int i = 0; i < N; ++i) {
		m_data[i] = 0;
	}
}

template<typename T, unsigned int N>
Tuple<T, N>::Tuple(const Tuple<T, N>& other)
{
	for(unsigned int i = 0; i < N; ++i)
		m_data[i] = other.m_data[i];
}

template <typename T, unsigned int N>
Tuple<T, N>::Tuple(T* data)
{
	for(unsigned int i = 0; i < N; ++i)
		m_data[i] = data[i];
}

template <typename T, unsigned int N>
Tuple<T, N>::~Tuple()
{
}

template <typename T, unsigned int N>
const Tuple<T,N>& Tuple<T,N>::scale(T value)
{
	for(unsigned int i = 0; i < N; ++i)
		this->m_data[i] = this->m_data[i]*value;
	return *this;
}

template <typename T, unsigned int N>
const Tuple<T,N>& Tuple<T,N>::operator=(const Tuple<T,N>& rhs)
{
   // copy already got rid of values smaller than EPS
   for(unsigned int i = 0; i < N; ++i)
       this->m_data[i] = rhs.m_data[i];
   return *this;
}

template <typename T, unsigned int N>
bool Tuple<T,N>::operator==(const Tuple<T,N>& rhs) const
{
	bool ret = true;
	for(unsigned int i = 0; i < N; ++i) {
		ret = ret && (this->m_data[i] == rhs.m_data[i]);
	}
	return ret;
}

template <typename T, unsigned int N>
bool Tuple<T,N>::operator!=(const Tuple<T,N>& rhs) const
{
	return !(this->operator==(rhs));
}

template <typename T, unsigned int N>
bool Tuple<T,N>::operator<(const Tuple<T,N>& rhs) const
{
	bool ret = true;
	for(unsigned int i = 0; i < N; ++i) {
		ret = ret && (this->m_data[i] < rhs.m_data[i]);
	}
	return ret;
}

template <typename T, unsigned int N>
bool Tuple<T,N>::operator<=(const Tuple<T,N>& rhs) const
{
	return this->operator==(rhs) || this->operator<(rhs);
}

template <typename T, unsigned int N>
bool Tuple<T,N>::operator>(const Tuple<T,N>& rhs) const
{
	return !this->operator<=(rhs);
}

template <typename T, unsigned int N>
bool Tuple<T,N>::operator>=(const Tuple<T,N>& rhs) const
{
	return !this->operator<(rhs);
}

} // end namespace Math

#endif
