//-------------------------------------------------------------------------------------------------
//
//  double.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxDouble Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/double.h>


//-------------------------------------------------------------------------
// CxDouble::<constructor>
//
//-------------------------------------------------------------------------
CxDouble::CxDouble( void )
{
    value = 0.0;
}

//-------------------------------------------------------------------------
// CxDouble::<constructor>
//
//-------------------------------------------------------------------------
CxDouble::CxDouble( double d_ )
{
    value = d_;
}

//-------------------------------------------------------------------------
// CxDouble::<constructor>
//
//-------------------------------------------------------------------------
CxDouble::CxDouble( CxString valueString_ )
{
    value = valueString_.toDouble();
}

//-------------------------------------------------------------------------
// CxDouble::<copy constructor>
//
//-------------------------------------------------------------------------
CxDouble::CxDouble( const CxDouble& d_ )
{
    if (&d_ != this) {
        value = d_.value;
    }
}

//-------------------------------------------------------------------------
// CxDouble::<assignment operator>
//
//-------------------------------------------------------------------------
CxDouble&
CxDouble::operator=( const CxDouble& rhs_ )
{
    if ( &rhs_ != this ) {
        value = rhs_.value;
    }
    return( *this );
}

//-------------------------------------------------------------------------
// CxDouble::<assignment operator>
//
//-------------------------------------------------------------------------
CxDouble
CxDouble::operator+( const CxDouble& rhs_ )
{
    CxDouble result( value + rhs_.value );
    return(result);
}

CxDouble
CxDouble::operator-( const CxDouble &rhs_ )
{
    CxDouble result( value - rhs_.value );
    return(result);
}

CxDouble
CxDouble::operator*( const CxDouble& rhs_ )
{
    CxDouble result( value * rhs_.value );
    return(result);
}

CxDouble
CxDouble::operator/( const CxDouble &rhs_ )
{
    CxDouble result( value / rhs_.value );
    return(result);
}

CxDouble&
CxDouble::operator+=( const CxDouble &rhs_ )
{
    value = value + rhs_.value;
    return( *this );
}

CxDouble&
CxDouble::operator-=( const CxDouble &rhs_ )
{
    value = value - rhs_.value;
    return( *this );
}


CxDouble&
CxDouble::operator*=( const CxDouble &rhs_ )
{
    value = value * rhs_.value;
    return( *this );
}

CxDouble&
CxDouble::operator/=( const CxDouble &rhs_ )
{
    value = value / rhs_.value;
    return( *this );
}

CxDouble
CxDouble::operator++( int )    // postfix ++ "var++"
{
    CxDouble result( value );
    value = value + 1;
    return( result );
}

CxDouble&
CxDouble::operator++( void ) // prefix "++var"
{
    value = value + 1;
    return(*this);
}

CxDouble
CxDouble::operator--( int )    // postfix "var--"
{
    CxDouble result( value );
    value = value - 1;
    return( result );
}

CxDouble&
CxDouble::operator--( void ) // prefix "--var"
{
    value = value - 1;
    return(*this);
}


CxDouble::operator double( void )
{
    return( value );
}

CxDouble::operator int(void)
{
    int iValue = value;
    return( iValue );
}

CxDouble::operator float(void)
{
    float fValue = value;
    return( fValue );
}

CxDouble::operator long(void)
{
    long lValue = value;
    return( lValue );
}

bool
CxDouble::operator==( const CxDouble& rhs_)
{
    if (value == rhs_.value) return(TRUE);
    return(FALSE);
}

bool
CxDouble::operator!=( const CxDouble& rhs_)
{
    if (value != rhs_.value) return(TRUE);
    return(FALSE);
}

bool
CxDouble::operator<=( const CxDouble& rhs_)
{
    if (value <= rhs_.value) return(TRUE);
    return(FALSE);
}

bool
CxDouble::operator>=( const CxDouble& rhs_)
{
    if (value >= rhs_.value) return(TRUE);
    return(FALSE);
}

bool
CxDouble::operator>( const CxDouble& rhs_)
{
    if (value > rhs_.value) return(TRUE);
    return(FALSE);
}

bool
CxDouble::operator<( const CxDouble& rhs_)
{
    if (value < rhs_.value) return(TRUE);
    return(FALSE);
}

int
CxDouble::compare( CxDouble otherDouble )
{
    if (value < otherDouble.value) return(-1);
    if (value > otherDouble.value) return(1);
    return(0);
}

//-------------------------------------------------------------------------
// CxDouble::isNAN
//
//-------------------------------------------------------------------------
int
CxDouble::isNAN( void )
{
    if ( fpclassify(value ) == FP_NAN ) return ( TRUE );
    return( FALSE );
}

//-------------------------------------------------------------------------
// CxDouble::isINFINITE
//
//-------------------------------------------------------------------------
int
CxDouble::isINFINITE ( void )
{
    if ( fpclassify(value ) == FP_INFINITE ) return ( TRUE );
    return( FALSE );
}

//-------------------------------------------------------------------------
// CxDouble::isZERO
//
//-------------------------------------------------------------------------
int
CxDouble::isZERO( void )
{
    if ( fpclassify(value ) == FP_ZERO ) return ( TRUE );
    return( FALSE );
}

//-------------------------------------------------------------------------
// CxDouble::isSUBNORMAL
//
//-------------------------------------------------------------------------
int
CxDouble::isSUBNORMAL( void )
{
    if ( fpclassify(value ) == FP_SUBNORMAL ) return ( TRUE );
    return( FALSE );
}


//-------------------------------------------------------------------------
// CxDouble::isNORMAL
//
//-------------------------------------------------------------------------
int
CxDouble::isNORMAL( void )
{
    if ( fpclassify(value ) == FP_NORMAL ) return ( TRUE );
    return( FALSE );
}


//-------------------------------------------------------------------------
// CxPlotTools::findExponent
//
// given a number figure out how many places to move the decimal point to
// end up with a number of power 1
//
//-------------------------------------------------------------------------
int CxDouble::findExponent(void)
{
    int exponent = (int) floor(nlog10(fabs(value)));
    //int exponent = (int) floor(nlog10(value));

    return( exponent );
}


//-------------------------------------------------------------------------
// CxPlotTools::findNiceExponent
//
// given a number findExponent figures out the friendly exponent to use
// (ie -9, -6, -3, 0, 3, 6 9) to use to represent it in exponential
// notation passing back that exponent
//
//-------------------------------------------------------------------------
int CxDouble::findNiceExponent(void)
{
    double x;
    int exponent;
    
    x = findExponent();
    
    x = x/3;
    x = floor( x );
    x = x * 3;
    
    exponent = (int) x;
    
    return( exponent );
}


//-------------------------------------------------------------------------
// CxDouble::valueScaledToExponent
//
// given an exponent this method scales the number by that exponent
// and returns the scaled number
//-------------------------------------------------------------------------
CxDouble CxDouble::valueScaledToExponent( int exponent )
{
    int c;
    double tempValue = value;
    
    if (exponent<0) {
        for (c=exponent; c<0; c++) {
            tempValue *= 10.0;
        }
    } else {
        for (c=0; c<exponent; c++) {
            tempValue /= 10.0;
        }
    }
    
    return( tempValue );
}


//-------------------------------------------------------------------------
// CxDouble::valueStringAsExponentNotation
//
// given a double value, a desired power to print the number as, and the
// number of decimal points to include, format the string to represent
// the numbers (does not includes the exponent part)
//-------------------------------------------------------------------------
CxString
CxDouble::valueStringAsExponentNotation( int exponent, int decimalPlacesAfter)
{
    int c;
    char formatString[100];
    char str[100];
    
    // scale the value to the desired exponent
    CxDouble tempValue = valueScaledToExponent( exponent );
    
    // format a string
    sprintf( formatString, "%%.%dlf", decimalPlacesAfter );
    sprintf( str, formatString, tempValue.value );
    
    // return
    return(str);
}


//-------------------------------------------------------------------------
// CxDouble::roundUpToNiceNumber
//
// round up to a number of the same power that is a nice number.
// Based on power of 1,2,5,10
//
//  example:  66   --> 100
//  example: 567.7 --> 1000
//  example: 450   --> 500
//  example: 198   --> 200
//  example: 201   --> 500
//
//-------------------------------------------------------------------------
CxDouble
CxDouble::roundUpToNiceNumber( void )
{
    int isNegative = FALSE;
    double tempValue = value;
    
    int exponent = findExponent();
    
    if (exponent<0) {
        for (int idx=exponent; idx<0; idx++) {
            tempValue *= 10.0;
        }
    } else {
        for (int idx=0; idx<exponent; idx++) {
            tempValue /= 10.0;
        }
    }
    
    if (tempValue!=0.0) {
        
        if (tempValue < 0.0 ) {
        
            if (tempValue < -10.0) {
                tempValue = -10.0;
            } else if (tempValue < -5.0) {
                tempValue = -5.0;
            } else if (tempValue < -2.0) {
                tempValue = -2.0;
            } else {
                tempValue = -1.0;
            }
        
        } else {
        
            if (tempValue>5.0) {
                tempValue = 10.0;
            } else if (tempValue > 2.0) {
                tempValue = 5.0;
            } else if (tempValue > 1.0) {
                tempValue = 2.0;
            } else {
                tempValue = 1.0;
            }
        }
    }
    
    if (exponent< 0.0) {
        for (int idx=exponent; idx<0; idx++) {
            tempValue /= 10.0;
        }
    } else {
        for (int idx=0; idx<exponent; idx++) {
            tempValue *= 10.0;
        }
    }
    
    return( tempValue );
}

// ie round1
CxDouble
CxDouble::roundDownToNiceNumber( void  )
{
    int idx;
    double tempValue = value;
    
    int exponent = findExponent();
    
    //std::cout << "e=" << exponent << std::endl;
    
    if (exponent<0) {
        
        for (idx=exponent; idx<0; idx++) {
            tempValue *= 10;
        }
        
    } else {
        
        for (idx=0; idx<exponent; idx++) {
            tempValue /= 10;
        }
    }
    
    tempValue = floor(tempValue);
    
    if (exponent<0) {
        
        for (idx=exponent; idx<0; idx++) {
            tempValue /=  10;
        }
        
    } else {
        
        for (idx=0; idx<exponent; idx++) {
            tempValue *= 10;
        }
    }
    
    return(tempValue);
}

//-------------------------------------------------------------------------
// CxDouble::interpolate
//
//-------------------------------------------------------------------------
CxDouble CxDouble::interpolate( double max, double min, double newMax, double newMin)
{
    double range = max - min;
    double percentThrough = (value - min) / range;
    
    double newRange = newMax - newMin;
    double amountToAdd = percentThrough * newRange;
    
    double newValue = newMin + amountToAdd;
    
    return( CxDouble( newValue) );
}

//-------------------------------------------------------------------------
// CxDouble::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxDouble& d_ )
{
    str << d_.value;
    return(str);
}
