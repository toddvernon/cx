//-------------------------------------------------------------------------------------------------
//
//  json_utf_object.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFObject Class - UTF-8 aware JSON object
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/base/utfstring.h>
#include <cx/base/slist.h>

#include <cx/json/json_utf_base.h>
#include <cx/json/json_utf_member.h>


#ifndef _CXJSON_UTF_OBJECT_
#define _CXJSON_UTF_OBJECT_



//-------------------------------------------------------------------------
// CxJSONUTFObject
//
// JSON object with UTF-8 aware keys and values.
//-------------------------------------------------------------------------
class CxJSONUTFObject: public CxJSONUTFBase
{
  public:

	CxJSONUTFObject( void );

	~CxJSONUTFObject( void );

	void
	append(CxJSONUTFMember *m);

    CxJSONUTFMember *
	at( int c) const;

	CxJSONUTFMember *
	removeAt( int i );

	CxJSONUTFMember *
	find( CxUTFString name);

	CxJSONUTFMember *
	find( const char *utf8name);

	int
	entries(void) const;

	void
	clear(void);

    virtual CxString toJsonString(void) const;
    virtual CxString toPrettyJsonString(int indent = 0) const;

  protected:

    virtual void print(std::ostream& str ) const;

  private:

    CxSList< CxJSONUTFMember *> _memberList;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONUTFObject& o_ );
    // outputs to an ostream

};


#endif
