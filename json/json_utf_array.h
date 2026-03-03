//-------------------------------------------------------------------------------------------------
//
//  json_utf_array.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFArray Class - UTF-8 aware JSON array
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/json/json_utf_base.h>


#ifndef _CXJSON_UTF_ARRAY_
#define _CXJSON_UTF_ARRAY_



//-------------------------------------------------------------------------
// CxJSONUTFArray
//
// JSON array with UTF-8 aware values.
//-------------------------------------------------------------------------
class CxJSONUTFArray: public CxJSONUTFBase
{
  public:

	CxJSONUTFArray( void );

	~CxJSONUTFArray( void );

	void
 	append( CxJSONUTFBase *b );

    CxJSONUTFBase *
	at( int c) const;

	int
	entries(void) const;

	void
	clear(void);

    virtual CxString toJsonString(void) const;
    virtual CxString toPrettyJsonString(int indent = 0) const;

  protected:

	virtual void print(std::ostream& str ) const;

  private:

    CxSList< CxJSONUTFBase *> _objectList;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONUTFArray& a_ );
    // outputs to an ostream

};


#endif
