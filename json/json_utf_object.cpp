//-------------------------------------------------------------------------------------------------
//
//  json_utf_object.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFObject Class - UTF-8 aware JSON object
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_utf_number.h>
#include <cx/json/json_utf_boolean.h>
#include <cx/json/json_utf_string.h>
#include <cx/json/json_utf_null.h>

#include <cx/json/json_utf_member.h>
#include <cx/json/json_utf_object.h>

#include <cx/json/json_utf_array.h>



//-------------------------------------------------------------------------
// CxJSONUTFObject::CxJSONUTFObject
//
//-------------------------------------------------------------------------
CxJSONUTFObject::CxJSONUTFObject(void)
{
    _type = CxJSONUTFBase::OBJECT;
}

//-------------------------------------------------------------------------
// CxJSONUTFObject::~CxJSONUTFObject
//
//-------------------------------------------------------------------------
CxJSONUTFObject::~CxJSONUTFObject(void)
{
	clear();
}

//-------------------------------------------------------------------------
// CxJSONUTFObject::append
//
//-------------------------------------------------------------------------
void
CxJSONUTFObject::append( CxJSONUTFMember *v )
{
    _memberList.append( v );
}


//-------------------------------------------------------------------------
// CxJSONUTFObject::entries
//
//-------------------------------------------------------------------------
int
CxJSONUTFObject::entries( void ) const
{
    return (_memberList.entries() );
}


//-------------------------------------------------------------------------
// CxJSONUTFObject::at
//
//-------------------------------------------------------------------------
CxJSONUTFMember *
CxJSONUTFObject::at( int i ) const
{
    CxJSONUTFMember *m = _memberList.at( i );
    return(m);
}

//-------------------------------------------------------------------------
// CxJSONUTFObject::removeAt
//
//-------------------------------------------------------------------------
CxJSONUTFMember *
CxJSONUTFObject::removeAt( int i )
{
	if (_memberList.entries() > 0) {

		CxJSONUTFMember *m = _memberList.at( i );
		_memberList.removeAt( i );
		return( m );
	}

	return (NULL );
}


//-------------------------------------------------------------------------
// CxJSONUTFObject::find
//
//-------------------------------------------------------------------------
CxJSONUTFMember *
CxJSONUTFObject::find( CxUTFString name )
{
    for (unsigned int c=0; c<_memberList.entries(); c++) {

		CxJSONUTFMember *m = _memberList.at(c);

		if (m->var() == name) {
			return( m );
		}
	}

	return (NULL );
}


//-------------------------------------------------------------------------
// CxJSONUTFObject::find
//
//-------------------------------------------------------------------------
CxJSONUTFMember *
CxJSONUTFObject::find( const char *utf8name )
{
    CxUTFString name;
    name.fromUTF8Bytes(utf8name, strlen(utf8name));
    return find(name);
}


//-------------------------------------------------------------------------
// CxJSONUTFObject::clear
//
//-------------------------------------------------------------------------
void
CxJSONUTFObject::clear(void)
{
    for (unsigned int c=0; c<_memberList.entries(); c++) {
        CxJSONUTFMember *m = _memberList.at(c);
		delete m;
	}

    _memberList.clear();
}


/* virtual */
void CxJSONUTFObject::print(std::ostream& str ) const
{
    int first = 1;

    str << "{";
    for (unsigned int c=0; c< _memberList.entries(); c++) {
        CxJSONUTFMember *m =  _memberList.at(c);

		if (!first) str << ",";
		str << *m;

        first=0;
	}
    str << "}";
}

/* virtual */
CxString CxJSONUTFObject::toJsonString(void) const
{
    CxString result("{");
    int first = 1;

    for (unsigned int c=0; c< _memberList.entries(); c++) {
        CxJSONUTFMember *m =  _memberList.at(c);

        if (!first) result += ",";

        if (m != NULL) {
            result += m->toJsonString();
        }

        first = 0;
    }
    result += "}";
    return result;
}

/* virtual */
CxString CxJSONUTFObject::toPrettyJsonString(int indent) const
{
    CxString result("{\n");
    CxString indentStr;
    CxString nextIndentStr;

    // Build indentation strings
    for (int i = 0; i < indent; i++) {
        indentStr += "  ";
    }
    nextIndentStr = indentStr + "  ";

    int first = 1;

    for (unsigned int c = 0; c < _memberList.entries(); c++) {
        CxJSONUTFMember *m = _memberList.at(c);

        if (!first) result += ",\n";

        if (m != NULL) {
            result += nextIndentStr;
            result += m->toPrettyJsonString(indent + 1);
        }

        first = 0;
    }

    result += "\n";
    result += indentStr;
    result += "}";
    return result;
}

//-------------------------------------------------------------------------
// CxJSONUTFObject::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONUTFObject& o_ )
{
    o_.print(str );
    return(str);
}
