/***************************************************************************
 *
 *  prop.cpp
 *
 *  CxPropEntry, CxPropertyList Class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, T.Vernon , which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/

#include <cx/json/json_string.h>


//-------------------------------------------------------------------------
// CxJSONString::CxJSONString
//
//-------------------------------------------------------------------------
CxJSONString::CxJSONString(void)
{
    _type   = CxJSONBase::STRING;
	_string = "";
}

//-------------------------------------------------------------------------
// CxJSONString::CxJSONString
//
//-------------------------------------------------------------------------
CxJSONString::CxJSONString( CxString s)
{
    _type   = CxJSONBase::STRING;
	_string = s;
}

//-------------------------------------------------------------------------
// CxJSONString::~CxJSONString
//
//-------------------------------------------------------------------------
CxJSONString::~CxJSONString(void)
{
}


//-------------------------------------------------------------------------
// CxJSONString::set
//
//-------------------------------------------------------------------------
void
CxJSONString::set( CxString s )
{
	_string = s;
}


//-------------------------------------------------------------------------
// CxJSONString::get
//
//-------------------------------------------------------------------------
CxString 
CxJSONString::get( void )
{
	return( _string );
}


/* virtual */
void CxJSONString::print(std::ostream& str ) const
{
    str << "\"" << _string << "\"";                 
}


//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONString& s_ )    
{
    s_.print(str);
    return(str);
}
 

