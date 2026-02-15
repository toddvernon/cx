//-------------------------------------------------------------------------------------------------
//
//  json_null.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_null.h>


//-------------------------------------------------------------------------
// CxJSONNull::CxJSONNull
//
//-------------------------------------------------------------------------
CxJSONNull::CxJSONNull(void)
{
    _type   = CxJSONBase::JNULL;
}

//-------------------------------------------------------------------------
// CxJSONNull::~CxJSONNull
//
//-------------------------------------------------------------------------
CxJSONNull::~CxJSONNull(void)
{
}


/* virtual */
void CxJSONNull::print(std::ostream& str ) const
{
    str << "null"; 
}
 
//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONNull& n_ )    
{
    n_.print( str );
    return(str);
}
 
