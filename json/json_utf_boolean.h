//-------------------------------------------------------------------------------------------------
//
//  json_utf_boolean.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFBoolean Class - UTF-8 aware JSON boolean
//
//-------------------------------------------------------------------------------------------------

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/slist.h>

#include <cx/json/json_utf_base.h>


#ifndef _CXJSON_UTF_BOOLEAN_
#define _CXJSON_UTF_BOOLEAN_



//-------------------------------------------------------------------------
// CxJSONUTFBoolean
//
//-------------------------------------------------------------------------
class CxJSONUTFBoolean: public CxJSONUTFBase
{
  public:

	CxJSONUTFBoolean( void );

	CxJSONUTFBoolean( int i );

	~CxJSONUTFBoolean( void );

	void
	set(int i);

	int
	get( void );

	void dump(void);

    virtual CxString toJsonString(void) const;
    virtual CxString toPrettyJsonString(int indent = 0) const;

  protected:

    virtual void print(std::ostream& str ) const;

  private:

	int _i;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONUTFBoolean& base_ );
    // outputs to an ostream
};


#endif
