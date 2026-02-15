//-------------------------------------------------------------------------------------------------
//
//  timeval.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  copy of this software and associated documentation files (the "Software")
//
//-------------------------------------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>
#include <sys/time.h>



#ifndef _CXTIMEVAL_
#define _CXTIMEVAL_


/**
 * @short TimeVal class
 *
 * TimeVal class is used for storing time value. Similar to "struct timeval",
 * the time value is in seconds and microseconds.
 */
 

class CxTimeVal {

public:
    static const long ONE_MILLION = 1000000;

public:
    /**
     * Default constructor
     */
    CxTimeVal() : _sec(0), _usec(0) {}
    
    /**
     * Constructor for given seconds and microseconds.
     * 
     * @param sec the number of seconds.
     * @param usec the number of microseconds.
     */
    CxTimeVal(long sec, long usec) : _sec(sec), _usec(usec) {}
    
    /**
     * Constructor for given "struct timeval".
     * 
     * @param timeval the "struct timeval" time value to initialize this
     * object with.
     */
    CxTimeVal(const timeval& timeval)
	: _sec(timeval.tv_sec), _usec(timeval.tv_usec) {}
    
    /**
     * Constructor for given double-float time value.
     * 
     * @param d the double-float time value to initialize this object with.
     */
    CxTimeVal(const double& d);
    
    /**
     * Get the number of seconds.
     * 
     * @return the number of seconds.
     */
    long sec() const		{ return _sec; }
    
    /**
     * Get the number of microseconds.
     * 
     * @return the number of microseconds.
     */
    long usec() const	{ return _usec; }
    
    /**
     * @return seconds and microseconds as a string.
     */
/*
    string str() const {return c_format("%d.%d", _sec, _usec); }
*/

    /**
     * Pretty print the time
     *
     * @return the time as formated by ctime(3) without the newline.
     */
/*
    string pretty_print() const {
	time_t t = static_cast<time_t>(_sec);
	return c_format("%.24s", asctime(localtime(&t))); 
    }
*/
    /**
     * Get zero value.
     */
    static CxTimeVal ZERO();
    
    /**
     * Get the maximum permitted value.
     */
    static CxTimeVal MAXIMUM();

    /**
     * Get the minimum permitted value.
     */
    static CxTimeVal MINIMUM();
    
    /**
     * Copy the time value from a timeval structure.
     * 
     * @param timeval the storage to copy the time from.
     * @return the number of copied octets.
     */
    size_t copy_in(const timeval& timeval);
    
    /**
     * Copy the time value to a timeval structure.
     * 
     * @param timeval the storage to copy the time to.
     * @return the number of copied octets.
     */
    size_t copy_out(timeval& timeval) const;
    
    /**
     * Convert a TimeVal value to a double-float value.
     * 
     * @return the double-float value of this TimeVal time.
     */
    double get_double() const { return (_sec * 1.0 + _usec * 1.0e-6); }

    /**
     * Assignment Operator
     */
    CxTimeVal& operator=(const CxTimeVal& other);
    
    /**
     * Equality Operator
     * 
     * @param other the right-hand operand to compare against.
     * @return true if the left-hand operand is numerically same as the
     * right-hand operand.
     */
    bool operator==(const CxTimeVal& other) const;
    
    /**
     * Less-Than Operator
     * 
     * @param other the right-hand operand to compare against.
     * @return true if the left-hand operand is numerically smaller than the
     * right-hand operand.
     */
    bool operator<(const CxTimeVal& other) const;
    
    /**
     * Assign-Sum Operator
     * 
     * @param delta the TimeVal value to add to this TimeVal object.
     * @return the TimeVal value after the addition of @ref delta.
     */
    const CxTimeVal& operator+=(const CxTimeVal& delta);
    
    /**
     * Addition Operator
     * 
     * @param other the TimeVal value to add to the value of this
     * TimeVal object.
     * @return the TimeVal value after the addition of @ref other. 
     */
    CxTimeVal operator+(const CxTimeVal& other) const;
    
    /**
     * Assign-Difference Operator
     * 
     * @param delta the TimeVal value to substract from this TimeVal object.
     * @return the TimeVal value after the substraction of @ref delta.
     */
    const CxTimeVal& operator-=(const CxTimeVal& delta);
    
    /**
     * Substraction Operator
     * 
     * @param other the TimeVal value to substract from the value of this
     * TimeVal object.
     * @return the TimeVal value after the substraction of @ref other.
     */
    CxTimeVal operator-(const CxTimeVal& other) const;

    /**
     * Multiplication Operator for integer operand
     * 
     * @param n the integer value used in multiplying the value of this
     * object with.
     * @return the TimeVal value of multiplying the value of this object
     * by @ref n.
     */
    CxTimeVal operator*(int n) const;

    /**
     * Multiplication Operator for double float operand
     * 
     * @param d the double float value used in multiplying the value of this
     * object with.
     * @return the TimeVal value of multiplying the value of this object
     * by @ref d.
     */
    CxTimeVal operator*(const double& d) const;
    
    /**
     * Division Operator for integer operand
     * 
     * @param n the integer value used in dividing the value of this
     * object with. 
     * @return the TimeVal value of dividing the value of this object
     * by @ref n.
     */
    CxTimeVal operator/(int n) const;

    /**
     * Division Operator for double-float operand
     * 
     * @param d the double-float value used in dividing the value of this
     * object with. 
     * @return the TimeVal value of dividing the value of this object
     * by @ref d.
     */
    CxTimeVal operator/(const double& d) const;

private:
    long _sec;		// The number of seconds
    long _usec;		// The number of microseconds
};


#endif _CXTIMEVAL_
