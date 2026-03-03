//-------------------------------------------------------------------------------------------------
//
//  json_utf_base.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFBase Class - UTF-8 aware JSON base class
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_utf_base.h>


//-------------------------------------------------------------------------
// CxJSONUTFBase::CxJSONUTFBase
//
//-------------------------------------------------------------------------
CxJSONUTFBase::CxJSONUTFBase()
{
    _type = CxJSONUTFBase::BASE;
}

//-------------------------------------------------------------------------
// CxJSONUTFBase::~CxJSONUTFBase
//
//-------------------------------------------------------------------------
CxJSONUTFBase::~CxJSONUTFBase()
{
}

CxJSONUTFBase::JSONObjectType
CxJSONUTFBase::type( void )
{
	return( _type );
}

void
CxJSONUTFBase::dump(void)
{
	std::cout << "UTF_BASE" << std::endl;
}



/* virtual */
void CxJSONUTFBase::print( std::ostream& str ) const
{
    str << "CxUTFBase";
}

/* virtual */
CxString CxJSONUTFBase::toJsonString(void) const
{
    return CxString("null");
}

/* virtual */
CxString CxJSONUTFBase::toPrettyJsonString(int indent) const
{
    return CxString("null");
}

//-------------------------------------------------------------------------
// CxJSONUTFBase::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONUTFBase& base_ )
{
    base_.print( str );
    return(str);
}
