//-------------------------------------------------------------------------------------------------
//
//  json_object.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/nxjson.h>

#include <cx/json/json_number.h>
#include <cx/json/json_boolean.h>
#include <cx/json/json_string.h>
#include <cx/json/json_null.h>

#include <cx/json/json_member.h>
#include <cx/json/json_object.h>

#include <cx/json/json_array.h>



//-------------------------------------------------------------------------
// CxJSONObject::CxJSONObject
//
//-------------------------------------------------------------------------
CxJSONObject::CxJSONObject(void)
{
    _type = CxJSONBase::OBJECT;
}

//-------------------------------------------------------------------------
// CxJSONObject::~CxJSONObject
//
//-------------------------------------------------------------------------
CxJSONObject::~CxJSONObject(void)
{
	clear();
}

//-------------------------------------------------------------------------
// CxJSONObject::append
//
//-------------------------------------------------------------------------
void
CxJSONObject::append( CxJSONMember *v )
{
    _memberList.append( v );
}


//-------------------------------------------------------------------------
// CxJSONObject::entries
//
//-------------------------------------------------------------------------
int
CxJSONObject::entries( void ) const
{
    return (_memberList.entries() );
}


//-------------------------------------------------------------------------
// CxJSONObject::at
//
//-------------------------------------------------------------------------
CxJSONMember *
CxJSONObject::at( int i ) const
{
    CxJSONMember *m = _memberList.at( i );
    return(m);
}

//-------------------------------------------------------------------------
// CxJSONObject::removeAt
//
//-------------------------------------------------------------------------
CxJSONMember *
CxJSONObject::removeAt( int i )
{
	if (_memberList.entries() > 0) {

		CxJSONMember *m = _memberList.at( i );
		_memberList.removeAt( i );
		return( m );
	}

	return (NULL );
}


//-------------------------------------------------------------------------
// CxJSONObject::at
//
//-------------------------------------------------------------------------
CxJSONMember *
CxJSONObject::find( CxString name )
{
    for (unsigned int c=0; c<_memberList.entries(); c++) {
        
		CxJSONMember *m = _memberList.at(c);

		if (m->var() == name) {
			return( m );
		}
	}

	return (NULL );
}


//-------------------------------------------------------------------------
// CxJSONObject::clear
//
//-------------------------------------------------------------------------
void
CxJSONObject::clear(void)
{
    for (unsigned int c=0; c<_memberList.entries(); c++) {
        CxJSONMember *m = _memberList.at(c);
		delete m;
	}

    _memberList.clear();
}


/* virtual */
void CxJSONObject::print(std::ostream& str ) const
{
    int first = 1;

    str << "{";
    for (unsigned int c=0; c< _memberList.entries(); c++) {
        CxJSONMember *m =  _memberList.at(c);

		if (!first) str << ",";
		str << *m;

        first=0;
	}
    str << "}";
}

/* virtual */
CxString CxJSONObject::toJsonString(void) const
{
    CxString result("{");
    int first = 1;

    for (unsigned int c=0; c< _memberList.entries(); c++) {
        CxJSONMember *m =  _memberList.at(c);

        if (!first) result += ",";

        if (m != NULL) {
            result += m->toJsonString();
        }

        first = 0;
    }
    result += "}";
    return result;
}

//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONObject& o_ )
{
    o_.print(str );
    return(str);
}
