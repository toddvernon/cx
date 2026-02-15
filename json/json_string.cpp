//-------------------------------------------------------------------------------------------------
//
//  json_string.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

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
    str << "\"";
    // Escape special characters for valid JSON output
    // Cast away const since charAt() isn't marked const but we only read
    CxString& s = const_cast<CxString&>(_string);
    for (unsigned long i = 0; i < s.length(); i++) {
        char c = (char)s.charAt(i);
        switch (c) {
            case '"':  str << "\\\""; break;
            case '\\': str << "\\\\"; break;
            case '\n': str << "\\n"; break;
            case '\r': str << "\\r"; break;
            case '\t': str << "\\t"; break;
            case '\b': str << "\\b"; break;
            case '\f': str << "\\f"; break;
            default:
                if ((unsigned char)c < 0x20) {
                    // Control character - output as \uXXXX
                    char buf[8];
                    sprintf(buf, "\\u%04x", (unsigned char)c);
                    str << buf;
                } else {
                    str << c;
                }
                break;
        }
    }
    str << "\"";
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
 
