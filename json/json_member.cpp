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


#include <cx/json/json_member.h>




//-------------------------------------------------------------------------
// CxJSONMember::CxJSONMember
//
//-------------------------------------------------------------------------
CxJSONMember::CxJSONMember()
{
    _var    = "";
    _object = NULL;
}

CxJSONMember::CxJSONMember( CxString var_, CxJSONBase *childObject_ )
{
    _var = var_;
    _object = childObject_;
}


CxJSONMember::~CxJSONMember( void )
{
	if (_object) delete _object;
}


//-------------------------------------------------------------------------
// CxJSONMember::var
//
//-------------------------------------------------------------------------
CxString
CxJSONMember::var(void) const
{
	return(_var);
}


//-------------------------------------------------------------------------
// CxJSONMember::val
//
//-------------------------------------------------------------------------
CxJSONBase *
CxJSONMember::object(void)
{
	return(_object);
}


//-------------------------------------------------------------------------
// CxJSONMember::val
//
//-------------------------------------------------------------------------
CxJSONBase *
CxJSONMember::removeObject(void)
{
	CxJSONBase *o = _object;
	_object = NULL;
	return(o);
}


/* virtual */
void CxJSONMember::print(std::ostream& str ) const
{
    str << "\"" << _var << "\":" << *_object;                 
}


//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONMember& member_ )    
{
    member_.print(str );
    return(str);
}
 
