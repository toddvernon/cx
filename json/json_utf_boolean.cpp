//-------------------------------------------------------------------------------------------------
//
//  json_utf_boolean.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFBoolean Class - UTF-8 aware JSON boolean
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_utf_boolean.h>


//-------------------------------------------------------------------------
// CxJSONUTFBoolean::CxJSONUTFBoolean
//
//-------------------------------------------------------------------------
CxJSONUTFBoolean::CxJSONUTFBoolean(void)
{
    _type   = CxJSONUTFBase::BOOLEAN;
	_i = false;
}


//-------------------------------------------------------------------------
// CxJSONUTFBoolean::CxJSONUTFBoolean
//
//-------------------------------------------------------------------------
CxJSONUTFBoolean::CxJSONUTFBoolean(int i )
{
    _type   = CxJSONUTFBase::BOOLEAN;
	_i = i;
}


//-------------------------------------------------------------------------
// CxJSONUTFBoolean::~CxJSONUTFBoolean
//
//-------------------------------------------------------------------------
CxJSONUTFBoolean::~CxJSONUTFBoolean(void)
{
}


//-------------------------------------------------------------------------
// CxJSONUTFBoolean::set
//
//-------------------------------------------------------------------------
void
CxJSONUTFBoolean::set( int i )
{
	_i = i;
}


//-------------------------------------------------------------------------
// CxJSONUTFBoolean::get
//
//-------------------------------------------------------------------------
int
CxJSONUTFBoolean::get( void )
{
	return( _i );
}

/* virtual */
void CxJSONUTFBoolean::print(std::ostream& str ) const
{
    CxString s = "false";
    if (_i) s = "true";

    str << s;
}

/* virtual */
CxString CxJSONUTFBoolean::toJsonString(void) const
{
    if (_i) return CxString("true");
    return CxString("false");
}

/* virtual */
CxString CxJSONUTFBoolean::toPrettyJsonString(int indent) const
{
    return toJsonString();
}

//-------------------------------------------------------------------------
// CxJSONUTFBoolean::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONUTFBoolean& b_ )
{
    b_.print( str );
    return(str);
}
