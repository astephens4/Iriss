#include "Math/quatf.hpp"
#include "Utils/FloatEquals.hpp"
#include "Utils/Angle.hpp"
#include "Math/vec3.hpp"

namespace Math {

bool quatf::normOnWrite = false;

quatf::quatf() : Tuple<float,4>()
{
    this->m_data[0] = 0;
    this->m_data[1] = 0;
    this->m_data[2] = 0;
    this->m_data[3] = 1;
}

quatf::quatf(float x, float y, float z, float w)
{
    // Change to compare with constants::EPS
    if(Utils::FloatEquals(x, 0.0f) &&
       Utils::FloatEquals(y, 0.0f) &&
       Utils::FloatEquals(z, 0.0f) &&
       Utils::FloatEquals(w, 0.0f)) {
        throw std::logic_error("Degenerate quaternion of 0 + 0i + 0j + 0k is not allowed");
    }
	this->m_data[0] = x;
	this->m_data[1] = y;
	this->m_data[2] = z;
	this->m_data[3] = w;
	if(this->normOnWrite)
		this->norm();
}

quatf::quatf(const Utils::Angle& angle, vec3f axis)
{
	axis.norm();
	this->m_data[0] = axis.x()*std::sin(angle.asRadians()/2.0f);
	this->m_data[1] = axis.y()*std::sin(angle.asRadians()/2.0f);
	this->m_data[2] = axis.z()*std::sin(angle.asRadians()/2.0f);
	this->m_data[3] = std::cos(angle.asRadians()/2.0f);
	if(this->normOnWrite)
		this->norm();
}

quatf::quatf(const Utils::Angle& angle1, vec3f axis1,
			 const Utils::Angle& angle2, vec3f axis2,
			 const Utils::Angle& angle3, vec3f axis3)
{
	*this = quatf(angle1, axis1);
	*this *= quatf(angle2, axis2);
	*this *= quatf(angle3, axis3);
	if(this->normOnWrite)
		this->norm();
}

quatf::quatf(const quatf& other) : Tuple<float,4>(other)
{
	if(this->normOnWrite)
		this->norm();
}

quatf::quatf(float* data) : Tuple<float,4>(data)
{
	if(this->normOnWrite)
		this->norm();
}

/**
 * Get the magnitude (length) of this vector.
 */
float quatf::mag() const
{
	float ret = float(this->m_data[0]*this->m_data[0]);
	ret = ret + float(this->m_data[1]*this->m_data[1]);
	ret = ret + float(this->m_data[2]*this->m_data[2]);
	ret = ret + float(this->m_data[3]*this->m_data[3]);
	return std::sqrt(ret);
}

const quatf& quatf::scale(float val)
{
	for(unsigned int i = 0; i < 4; ++i)
		this->m_data[i] *= val;
	return *this;
}

const quatf& quatf::operator=(quatf rhs)
{
	float tmp;
	for(unsigned int i = 0; i < 4; ++i) {
		tmp = rhs.m_data[i];
		rhs.m_data[i] = this->m_data[i];
		this->m_data[i] = tmp;
	}
	if(quatf::normOnWrite)
		return this->norm();
	else
		return *this;
}

/**
 * Normalize this vector. Returns a reference to this
 * so it can return a value for use in functions.
 */
const quatf& quatf::norm()
{
	float m = this->mag();
	m = 1.0f/m;
	return this->scale(m);
}

quatf quatf::conj() const
{
	return quatf(-x(), -y(), -z(), w());
}

void quatf::alwaysNormed(bool normOnWrite)
{
	quatf::normOnWrite = normOnWrite;
}


quatf quatf::operator+(const quatf& rhs) const
{
	return quatf(x() + rhs.x(),
				 y() + rhs.y(),
				 z() + rhs.z(),
				 w() + rhs.w());
}

quatf quatf::operator-(const quatf& rhs) const
{
	return quatf(x() - rhs.x(),
				 y() - rhs.y(),
				 z() - rhs.z(),
				 w() - rhs.w());
}

/**
 * (w0 + x0i + y0j + z0k)*(w1 + x1i + y1j + z1k)
 *  w0*w1 + (w0*x1)i + (w0*y1)j + (w0*z1)k +
 * -x0*x1 + (x0*w1)i - (x0*z1)j + (x0*y1)k +
 * -y0*y1 + (y0*z1)i + (y0*w1)j - (y0*x1)k +
 * -z0*z1 - (z0*y1)i + (z0*x1)j + (z0*w1)k
 *
 * (w0*w1 - x0*x1 - y0*y1 - z0*z1)  +
 * (w0*x1 + x0*w1 + y0*z1 - z0*y1)i +
 * (w0*y1 - x0*z1 + y0*w1 + z0*x1)j +
 * (w0*zi + x0*y1 - y0*x1 + z0*w1)k
 */
quatf quatf::operator*(const quatf& rhs) const
{
	float w = this->m_data[3]*rhs.m_data[3] -
			  this->m_data[2]*rhs.m_data[2] -
			  this->m_data[1]*rhs.m_data[1] -
			  this->m_data[0]*rhs.m_data[0];

	float x = this->m_data[3]*rhs.m_data[0] +
			  this->m_data[0]*rhs.m_data[3] +
			  this->m_data[1]*rhs.m_data[2] -
			  this->m_data[2]*rhs.m_data[1];

	float y = this->m_data[3]*rhs.m_data[1] -
			  this->m_data[0]*rhs.m_data[2] +
			  this->m_data[1]*rhs.m_data[3] +
			  this->m_data[2]*rhs.m_data[0];

	float z = this->m_data[3]*rhs.m_data[2] +
			  this->m_data[0]*rhs.m_data[1] -
			  this->m_data[1]*rhs.m_data[0] +
			  this->m_data[2]*rhs.m_data[3];
	return quatf(x, y, z, w);
}

const quatf& quatf::operator+=(const quatf& rhs)
{
	*this = *this + rhs;
	return *this;
}

const quatf& quatf::operator-=(const quatf& rhs)
{
	*this = *this - rhs;
	return *this;
}


const quatf& quatf::operator*=(const quatf& rhs)
{
	*this = *this * rhs;
	return *this;
}

} // end namespace Math
