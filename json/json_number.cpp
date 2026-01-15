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


//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONNumber& number_ )    
{
    number_.print( str); 
    return(str);
}
 



 
