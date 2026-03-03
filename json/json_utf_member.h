//-------------------------------------------------------------------------------------------------
//
//  json_utf_member.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFMember Class - UTF-8 aware JSON member (key-value pair)
//
//-------------------------------------------------------------------------------------------------

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/utfstring.h>
#include <cx/base/slist.h>

#include <cx/json/json_utf_base.h>

#ifndef _CXJSON_UTF_MEMBER_
#define _CXJSON_UTF_MEMBER_


class CxJSONUTFBase;


//-------------------------------------------------------------------------
// CxJSONUTFMember
//
// A key-value pair where the key is a CxUTFString for proper UTF-8 handling.
//-------------------------------------------------------------------------
class CxJSONUTFMember
{
  public:

    CxJSONUTFMember(void);
    // constructor

    CxJSONUTFMember( CxUTFString var, CxJSONUTFBase *o );
    // construct from UTF-8 name and value

    CxJSONUTFMember( const char *utf8var, CxJSONUTFBase *o );
    // construct from C string (UTF-8 bytes) and value

	~CxJSONUTFMember( void );
	// destructor

    CxUTFString var( void ) const;

    CxJSONUTFBase *object(void);

	CxJSONUTFBase *removeObject( void );

    void dump(void);

    CxString toJsonString(void) const;
    CxString toPrettyJsonString(int indent = 0) const;

  protected:

    virtual void print(std::ostream& str ) const;

  private:

    CxUTFString _var;
    CxJSONUTFBase *_object;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONUTFMember& member_ );
    // outputs to an ostream

};

#endif
