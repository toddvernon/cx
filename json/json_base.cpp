//-------------------------------------------------------------------------------------------------
//
//  json_base.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_base.h>


//-------------------------------------------------------------------------
// CxJSONBase::CxJSONBase
//
//-------------------------------------------------------------------------
CxJSONBase::CxJSONBase()
{
    _type = CxJSONBase::BASE;
}

CxJSONBase::JSONObjectType 
CxJSONBase::type( void )
{
	return( _type );
}

void
CxJSONBase::dump(void)
{
	std::cout << "BASE" << std::endl;
}



/* virtual */
void CxJSONBase::print( std::ostream& str ) const
{
    str << "CxBase";
}

/* virtual */
CxString CxJSONBase::toJsonString(void) const
{
    return CxString("null");
}

//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONBase& base_ )
{
    base_.print( str );
    return(str);
}
