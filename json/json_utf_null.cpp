//-------------------------------------------------------------------------------------------------
//
//  json_utf_null.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFNull Class - UTF-8 aware JSON null
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_utf_null.h>


//-------------------------------------------------------------------------
// CxJSONUTFNull::CxJSONUTFNull
//
//-------------------------------------------------------------------------
CxJSONUTFNull::CxJSONUTFNull(void)
{
    _type   = CxJSONUTFBase::JNULL;
}

//-------------------------------------------------------------------------
// CxJSONUTFNull::~CxJSONUTFNull
//
//-------------------------------------------------------------------------
CxJSONUTFNull::~CxJSONUTFNull(void)
{
}


/* virtual */
void CxJSONUTFNull::print(std::ostream& str ) const
{
    str << "null";
}

/* virtual */
CxString CxJSONUTFNull::toJsonString(void) const
{
    return CxString("null");
}

/* virtual */
CxString CxJSONUTFNull::toPrettyJsonString(int indent) const
{
    return toJsonString();
}

//-------------------------------------------------------------------------
// CxJSONUTFNull::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONUTFNull& n_ )
{
    n_.print( str );
    return(str);
}
