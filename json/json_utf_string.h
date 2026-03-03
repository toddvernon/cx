//-------------------------------------------------------------------------------------------------
//
//  json_utf_string.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFString Class - UTF-8 aware JSON string
//
//-------------------------------------------------------------------------------------------------

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/utfstring.h>
#include <cx/base/slist.h>

#include <cx/json/json_utf_base.h>


#ifndef _CXJSON_UTF_STRING_
#define _CXJSON_UTF_STRING_



//-------------------------------------------------------------------------
// CxJSONUTFString
//
// JSON string type that properly handles UTF-8 characters.
// Uses CxUTFString for internal storage.
//-------------------------------------------------------------------------
class CxJSONUTFString: public CxJSONUTFBase
{
  public:

	CxJSONUTFString( void );

	CxJSONUTFString( CxUTFString s );

	CxJSONUTFString( const char *utf8bytes );

	~CxJSONUTFString( void );

	void
	set(CxUTFString s);

	void
	set(const char *utf8bytes);

	CxUTFString
	get( void );

    void dump(void);

    virtual CxString toJsonString(void) const;
    virtual CxString toPrettyJsonString(int indent = 0) const;

  protected:

    virtual void print(std::ostream& str ) const;


  private:

	CxUTFString _string;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONUTFString& base_ );
    // outputs to an ostream
};


#endif
