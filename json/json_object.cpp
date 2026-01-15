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

//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONObject& o_ )    
{
    o_.print(str );
    return(str);
}


