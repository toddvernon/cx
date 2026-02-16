//-------------------------------------------------------------------------------------------------
//
//  json_boolean.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_boolean.h>




//-------------------------------------------------------------------------
// CxJSONBoolean::CxJSONBoolean
//
//-------------------------------------------------------------------------
CxJSONBoolean::CxJSONBoolean(void)
{
    _type   = CxJSONBase::BOOLEAN;
	_i = false;
}


//-------------------------------------------------------------------------
// CxJSONBoolean::CxJSONBoolean
//
//-------------------------------------------------------------------------
CxJSONBoolean::CxJSONBoolean(int i )
{
    _type   = CxJSONBase::BOOLEAN;
	_i = i;
}


//-------------------------------------------------------------------------
// CxJSONBoolean::~CxJSONBoolean
//
//-------------------------------------------------------------------------
CxJSONBoolean::~CxJSONBoolean(void)
{
}


//-------------------------------------------------------------------------
// CxJSONBoolean::set
//
//-------------------------------------------------------------------------
void
CxJSONBoolean::set( int i )
{
	_i = i;
}


//-------------------------------------------------------------------------
// CxJSONBoolean::get
//
//-------------------------------------------------------------------------
int 
CxJSONBoolean::get( void )
{
	return( _i );
}

/* virtual */
void CxJSONBoolean::print(std::ostream& str ) const
{
    CxString s = "false";
    if (_i) s = "true";

    str << s;
}

/* virtual */
CxString CxJSONBoolean::toJsonString(void) const
{
    if (_i) return CxString("true");
    return CxString("false");
}

//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONBoolean& b_ )
{
    b_.print( str );
    return(str);
}
 
