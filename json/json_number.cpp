//-------------------------------------------------------------------------------------------------
//
//  json_number.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_number.h>


//-------------------------------------------------------------------------
// CxJSONNumber::CxJSONNumber
//
//-------------------------------------------------------------------------
CxJSONNumber::CxJSONNumber(void)
{
    _type   = CxJSONBase::NUMBER;
	_d = 0.0;
}

//-------------------------------------------------------------------------
// CxJSONNumber::CxJSONNumber
//
//-------------------------------------------------------------------------
CxJSONNumber::CxJSONNumber(double d)
{
    _type   = CxJSONBase::NUMBER;
	_d = d;
}

//-------------------------------------------------------------------------
// CxJSONNumber::~CxJSONNumber
//
//-------------------------------------------------------------------------
CxJSONNumber::~CxJSONNumber(void)
{
}


//-------------------------------------------------------------------------
// CxJSONNumber::set
//
//-------------------------------------------------------------------------
void
CxJSONNumber::set( double d )
{
	_d = d;
}


//-------------------------------------------------------------------------
// CxJSONNumber::get
//
//-------------------------------------------------------------------------
double
CxJSONNumber::get( void )
{
	return( _d );
}

/* virtual */
void CxJSONNumber::print(std::ostream& str ) const
{
    str << _d;
}

/* virtual */
CxString CxJSONNumber::toJsonString(void) const
{
    char buf[64];
    sprintf(buf, "%g", _d);
    return CxString(buf);
}


//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONNumber& number_ )
{
    number_.print( str);
    return(str);
}
 



 
