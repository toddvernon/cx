//-------------------------------------------------------------------------------------------------
//
//  json_utf_string.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFString Class - UTF-8 aware JSON string
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_utf_string.h>


//-------------------------------------------------------------------------
// CxJSONUTFString::CxJSONUTFString
//
//-------------------------------------------------------------------------
CxJSONUTFString::CxJSONUTFString(void)
{
    _type   = CxJSONUTFBase::STRING;
}

//-------------------------------------------------------------------------
// CxJSONUTFString::CxJSONUTFString
//
//-------------------------------------------------------------------------
CxJSONUTFString::CxJSONUTFString( CxUTFString s)
{
    _type   = CxJSONUTFBase::STRING;
	_string = s;
}

//-------------------------------------------------------------------------
// CxJSONUTFString::CxJSONUTFString
//
//-------------------------------------------------------------------------
CxJSONUTFString::CxJSONUTFString( const char *utf8bytes )
{
    _type   = CxJSONUTFBase::STRING;
    _string.fromUTF8Bytes(utf8bytes, strlen(utf8bytes));
}

//-------------------------------------------------------------------------
// CxJSONUTFString::~CxJSONUTFString
//
//-------------------------------------------------------------------------
CxJSONUTFString::~CxJSONUTFString(void)
{
}


//-------------------------------------------------------------------------
// CxJSONUTFString::set
//
//-------------------------------------------------------------------------
void
CxJSONUTFString::set( CxUTFString s )
{
	_string = s;
}

//-------------------------------------------------------------------------
// CxJSONUTFString::set
//
//-------------------------------------------------------------------------
void
CxJSONUTFString::set( const char *utf8bytes )
{
    _string.fromUTF8Bytes(utf8bytes, strlen(utf8bytes));
}


//-------------------------------------------------------------------------
// CxJSONUTFString::get
//
//-------------------------------------------------------------------------
CxUTFString
CxJSONUTFString::get( void )
{
	return( _string );
}


/* virtual */
void CxJSONUTFString::print(std::ostream& str ) const
{
    str << "\"";
    // Get raw UTF-8 bytes and escape special characters
    CxString bytes = _string.toBytes();
    for (unsigned long i = 0; i < bytes.length(); i++) {
        char c = bytes.data()[i];
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
                    // Regular character (including UTF-8 continuation bytes)
                    str << c;
                }
                break;
        }
    }
    str << "\"";
}


/* virtual */
CxString CxJSONUTFString::toJsonString(void) const
{
    CxString result("\"");
    // Get raw UTF-8 bytes and escape special characters
    CxString bytes = _string.toBytes();
    for (unsigned long i = 0; i < bytes.length(); i++) {
        char c = bytes.data()[i];
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            default:
                if ((unsigned char)c < 0x20) {
                    char buf[8];
                    sprintf(buf, "\\u%04x", (unsigned char)c);
                    result += buf;
                } else {
                    result += c;
                }
                break;
        }
    }
    result += "\"";
    return result;
}

/* virtual */
CxString CxJSONUTFString::toPrettyJsonString(int indent) const
{
    // Strings don't need special formatting - just use toJsonString
    return toJsonString();
}


//-------------------------------------------------------------------------
// CxJSONUTFString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONUTFString& s_ )
{
    s_.print(str);
    return(str);
}
