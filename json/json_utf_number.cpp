//-------------------------------------------------------------------------------------------------
//
//  json_utf_number.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFNumber Class - UTF-8 aware JSON number
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_utf_number.h>


//-------------------------------------------------------------------------
// CxJSONUTFNumber::CxJSONUTFNumber
//
//-------------------------------------------------------------------------
CxJSONUTFNumber::CxJSONUTFNumber(void)
{
    _type   = CxJSONUTFBase::NUMBER;
	_d = 0.0;
}

//-------------------------------------------------------------------------
// CxJSONUTFNumber::CxJSONUTFNumber
//
//-------------------------------------------------------------------------
CxJSONUTFNumber::CxJSONUTFNumber(double d)
{
    _type   = CxJSONUTFBase::NUMBER;
	_d = d;
}

//-------------------------------------------------------------------------
// CxJSONUTFNumber::~CxJSONUTFNumber
//
//-------------------------------------------------------------------------
CxJSONUTFNumber::~CxJSONUTFNumber(void)
{
}


//-------------------------------------------------------------------------
// CxJSONUTFNumber::set
//
//-------------------------------------------------------------------------
void
CxJSONUTFNumber::set( double d )
{
	_d = d;
}


//-------------------------------------------------------------------------
// CxJSONUTFNumber::get
//
//-------------------------------------------------------------------------
double
CxJSONUTFNumber::get( void )
{
	return( _d );
}

/* virtual */
void CxJSONUTFNumber::print(std::ostream& str ) const
{
    str << _d;
}

/* virtual */
CxString CxJSONUTFNumber::toJsonString(void) const
{
    char buf[64];
    sprintf(buf, "%.17g", _d);
    return CxString(buf);
}

/* virtual */
CxString CxJSONUTFNumber::toPrettyJsonString(int indent) const
{
    return toJsonString();
}


//-------------------------------------------------------------------------
// CxJSONUTFNumber::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONUTFNumber& number_ )
{
    number_.print( str);
    return(str);
}
