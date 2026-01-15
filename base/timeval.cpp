//
// Copyright (c) 2001-2003 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

// $XORP: xorp/libxorp/timeval.hh,v 1.15 2003/09/17 22:33:13 atanu Exp $



#include <cx/base/timeval.h>


CxTimeVal::CxTimeVal(const double& d)
    : _sec((long)d),
      _usec((long)((d - ((double)_sec)) * ONE_MILLION + 0.5e-6))
{
    //
    // Adjust
    //
    if (_usec >= ONE_MILLION) {
	_sec += _usec / ONE_MILLION;
	_usec %= ONE_MILLION;
    }
}

size_t
CxTimeVal::copy_in(const timeval& timeval)
{
    _sec = timeval.tv_sec;
    _usec = timeval.tv_usec;
    return (sizeof(_sec) + sizeof(_usec));
}

size_t
CxTimeVal::copy_out(timeval& timeval) const
{
    timeval.tv_sec = _sec;
    timeval.tv_usec = _usec;
    return (sizeof(_sec) + sizeof(_usec));
}

CxTimeVal&
CxTimeVal::operator=(const CxTimeVal& other)
{
    _sec = other.sec();
    _usec = other.usec();
    return *this;
}

bool
CxTimeVal::operator==(const CxTimeVal& other) const
{
    return (_sec == other.sec()) && (_usec == other.usec());
}

bool
CxTimeVal::operator<(const CxTimeVal& other) const
{
    return (_sec == other.sec()) ? _usec < other.usec() : _sec < other.sec();
}

const CxTimeVal&
CxTimeVal::operator+=(const CxTimeVal& delta)
{
    _sec += delta.sec();
    _usec += delta.usec();
    if (_usec >= ONE_MILLION) {
	_sec++;
	_usec -= ONE_MILLION;
    }
    return (*this);
}

CxTimeVal
CxTimeVal::operator+(const CxTimeVal& other) const
{
    CxTimeVal tmp_tv(*this);
    return tmp_tv += other;
}

const CxTimeVal&
CxTimeVal::operator-=(const CxTimeVal& delta)
{
    _sec -= delta.sec();
    if (_usec < delta.usec()) {
	// Compensate
	_sec--;
	_usec += ONE_MILLION;
    }
    _usec -= delta.usec();
    
    return (*this);
}

CxTimeVal
CxTimeVal::operator-(const CxTimeVal& other) const
{
    CxTimeVal tmp_tv(*this);
    return tmp_tv -= other;
}

CxTimeVal
CxTimeVal::operator*(int n) const
{
    u_long tmp_sec, tmp_usec;
    
    tmp_usec = _usec * n;
    tmp_sec = _sec * n + tmp_usec / ONE_MILLION;
    tmp_usec %= ONE_MILLION;
    return CxTimeVal(tmp_sec, tmp_usec);
}

CxTimeVal
CxTimeVal::operator*(const double& d) const
{
    return CxTimeVal(get_double() * d);
}

CxTimeVal
CxTimeVal::operator/(int n) const
{
    return CxTimeVal(_sec / n, ((_sec % n) * ONE_MILLION + _usec) / n);
}

CxTimeVal
CxTimeVal::operator/(const double& d) const
{
    return CxTimeVal(get_double() / d);
}

CxTimeVal
CxTimeVal::ZERO()
{
    return CxTimeVal(0, 0);
}

CxTimeVal
CxTimeVal::MAXIMUM()
{
    return CxTimeVal(0x7fffffff, ONE_MILLION - 1);
}

CxTimeVal
CxTimeVal::MINIMUM()
{
    return CxTimeVal(- 0x7fffffff - 1, - (ONE_MILLION - 1));
}

/**
 * Prefix unary minus.
 */
 
CxTimeVal
operator-(const CxTimeVal& v)
{
    return CxTimeVal(-v.sec(), -v.usec());
}

/**
 * Multiply TimeVal by integer.
 */
CxTimeVal
operator*(int n, const CxTimeVal& t)
{
    return t * n;
}

/**
 * Multiply TimeVal by double.
 */
 
CxTimeVal
operator*(const double& d, const CxTimeVal& t)
{
    return t * d;
}


/**
 * Generate a TimeVal value from a uniform random distribution between
 * specified bounds.
 * @param lower lower bound of generated value.
 * @param upper upper bound of generated value.
 * @return value chosen from uniform random distribution.
 */
 
CxTimeVal
random_uniform(const CxTimeVal& lower, const CxTimeVal& upper)
{
    double d = (upper - lower).get_double();
    d *= double(random()) / double(RAND_MAX);
    return lower + CxTimeVal(d);
}

/**
 * Generate a TimeVal value from a uniform random distribution between
 * zero and specified bound.
 * @param upper upper bound of generated value.
 * @return value chosen from uniform random distribution.
 */
 
CxTimeVal
random_uniform(const CxTimeVal& upper)
{
    double d = upper.get_double();
    d *= double(random()) / double(RAND_MAX);
    return CxTimeVal(d);
}

/**
 * Generate a TimeVal value from a uniform random distribution between
 * the bounds center - factor * center and center + factor * center.
 * If the lower bound is less than TimeVal::ZERO() it is rounded up to
 * TimeVal::ZERO().
 *
 * @param center mid-point of generated time value.
 * @param factor the spread of the uniform random distribution.
 * @return value chosen from uniform random distribution.
 */

CxTimeVal
positive_random_uniform(const CxTimeVal& center, const double& factor)
{
//    CxTimeVal l = max(center - center * factor, CxTimeVal::ZERO());
//    CxTimeVal u = center + center * factor;
//    return random_uniform(l, u);

    return random_uniform(0,0);
}





