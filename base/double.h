//-------------------------------------------------------------------------------------------------
//
//  double.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxDouble Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>


//-------------------------------------------------------------------------
// platform includes
//
//-------------------------------------------------------------------------
#if defined(_LINUX_)
#include <unistd.h>
#endif


//-------------------------------------------------------------------------
// local includes
//
//-------------------------------------------------------------------------
#include <cx/base/string.h>

#ifndef _CxDouble_
#define _CxDouble_

#ifndef nlog10
#define nlog10(x) (x == 0.0 ? 0.0 : log10(x))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif



//-------------------------------------------------------------------------
// class CxDouble
//
//-------------------------------------------------------------------------
class CxDouble
{
  public:

    CxDouble( void );
    // constructor creates a invalid double object

    CxDouble( double value_ );
    // construct with a value
    
    CxDouble( CxString valueString_ );
    // construct from a string value
    
    CxDouble( const CxDouble& d_);
    // copuy constructor
    
    CxDouble& operator=(const CxDouble& rhs_);
    // assignment operator
    
    CxDouble operator+(const CxDouble& rhs_);
    // operator +
    
    CxDouble operator-( const CxDouble& rhs_);
    // operator -
    
    CxDouble operator*(const CxDouble& rhs_);
    // operator *
    
    CxDouble operator/( const CxDouble& rhs_);
    // operator /
    
    CxDouble& operator+=( const CxDouble &rhs_ );
    // operator +=
    
    CxDouble& operator-=( const CxDouble &rhs_ );
    // operator -=
    
    CxDouble& operator*=( const CxDouble &rhs_ );
    // operator *=
    
    CxDouble& operator/=( const CxDouble &rhs_ );
    // operator /=
    
    bool operator==( const CxDouble& rhs_);
    
    bool operator!=( const CxDouble& rhs_);
    
    bool operator<=( const CxDouble& rhs_);
    
    bool operator>=( const CxDouble& rhs_);
    
    bool operator>( const CxDouble& rhs_);
    
    bool operator<( const CxDouble& rhs_);
    
    int compare( CxDouble otherDouble );
    
    CxDouble operator++( int );
    // operator++ (var++)
    
    CxDouble& operator++( void );
    // operator++ (++var)
    
    CxDouble operator--( int );
    // operator-- (var--)
    
    CxDouble& operator--( void );
    // operator-- (--var)
    
    operator double(void);
    // implicit cast to a double
    
    operator int(void);
    // implicit cast to an int
    
    operator float(void);
    // implicit cast to a float
    
    operator long(void);
    // inplicit cast to a long
    
    int isNAN( void );
    // returns true if the value is "Not A Number"
    
    int isINFINITE( void );
    // returns true if the value is infinite
    
    int isZERO( void );
    // returns true if the value is zero
    
    int isSUBNORMAL( void );
    // returns true if the value is subnormal
    
    int isNORMAL( void );
    // returns true if the value represents a valid floating point double
    
    int findExponent(void);
    // given a number figure out how many places to move the decimal point to
    // end up with a number of power 1
    
    int findNiceExponent(void);
    // returns an exponent that represents the number using the convenient notiations such
    // as (ie -9, -6, -3, 0, 3, 6 9)
    
    CxDouble valueScaledToExponent( int exponent );
    // returns the value scaled for a specific exponent
    
    CxString valueStringAsExponentNotation( int exponent, int decimalPlacesAfter);
    // returns a number 

    CxDouble roundUpToNiceNumber( void );
    // round up to a number of the same power that is a nice number. Based on 1,2,5,10
    
    CxDouble roundDownToNiceNumber( void  );
    // round down to a number of the same power that is a nice number. Based on 1,2,5,10
    
    CxDouble interpolate( double max, double min, double newMax, double newMin);
    // interpolate a number between two to a new range
    
    double value;

    
private:
    

    friend std::ostream& operator<<(std::ostream& str, const CxDouble& d_ );
    // outputs a CxString to an ostream
    
};


#endif
