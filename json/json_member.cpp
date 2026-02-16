//-------------------------------------------------------------------------------------------------
//
//  json_member.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

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

CxString CxJSONMember::toJsonString(void) const
{
    CxString result("\"");
    result += _var;
    result += "\":";
    if (_object != NULL) {
        result += _object->toJsonString();
    } else {
        result += "null";
    }
    return result;
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
 
