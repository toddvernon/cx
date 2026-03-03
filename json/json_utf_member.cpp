//-------------------------------------------------------------------------------------------------
//
//  json_utf_member.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFMember Class - UTF-8 aware JSON member (key-value pair)
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_utf_member.h>




//-------------------------------------------------------------------------
// CxJSONUTFMember::CxJSONUTFMember
//
//-------------------------------------------------------------------------
CxJSONUTFMember::CxJSONUTFMember()
{
    _object = NULL;
}

CxJSONUTFMember::CxJSONUTFMember( CxUTFString var_, CxJSONUTFBase *childObject_ )
{
    _var = var_;
    _object = childObject_;
}

CxJSONUTFMember::CxJSONUTFMember( const char *utf8var, CxJSONUTFBase *childObject_ )
{
    _var.fromUTF8Bytes(utf8var, strlen(utf8var));
    _object = childObject_;
}


CxJSONUTFMember::~CxJSONUTFMember( void )
{
	if (_object) delete _object;
}


//-------------------------------------------------------------------------
// CxJSONUTFMember::var
//
//-------------------------------------------------------------------------
CxUTFString
CxJSONUTFMember::var(void) const
{
	return(_var);
}


//-------------------------------------------------------------------------
// CxJSONUTFMember::object
//
//-------------------------------------------------------------------------
CxJSONUTFBase *
CxJSONUTFMember::object(void)
{
	return(_object);
}


//-------------------------------------------------------------------------
// CxJSONUTFMember::removeObject
//
//-------------------------------------------------------------------------
CxJSONUTFBase *
CxJSONUTFMember::removeObject(void)
{
	CxJSONUTFBase *o = _object;
	_object = NULL;
	return(o);
}


/* virtual */
void CxJSONUTFMember::print(std::ostream& str ) const
{
    str << "\"" << _var.toBytes() << "\":" << *_object;
}

CxString CxJSONUTFMember::toJsonString(void) const
{
    CxString result("\"");
    // Get raw UTF-8 bytes for the key
    CxString keyBytes = _var.toBytes();
    // Escape special characters in the key
    for (unsigned long i = 0; i < keyBytes.length(); i++) {
        char c = keyBytes.data()[i];
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
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
    result += "\":";
    if (_object != NULL) {
        result += _object->toJsonString();
    } else {
        result += "null";
    }
    return result;
}

CxString CxJSONUTFMember::toPrettyJsonString(int indent) const
{
    CxString result("\"");
    // Get raw UTF-8 bytes for the key
    CxString keyBytes = _var.toBytes();
    // Escape special characters in the key
    for (unsigned long i = 0; i < keyBytes.length(); i++) {
        char c = keyBytes.data()[i];
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
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
    result += "\": ";
    if (_object != NULL) {
        result += _object->toPrettyJsonString(indent);
    } else {
        result += "null";
    }
    return result;
}


//-------------------------------------------------------------------------
// CxJSONUTFMember::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONUTFMember& member_ )
{
    member_.print(str );
    return(str);
}
