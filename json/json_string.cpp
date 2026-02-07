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
 

