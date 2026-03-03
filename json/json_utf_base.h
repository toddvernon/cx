//-------------------------------------------------------------------------------------------------
//
//  json_utf_base.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFBase Class - UTF-8 aware JSON base class
//
//-------------------------------------------------------------------------------------------------

#ifndef _CXJSON_UTF_BASE_
#define _CXJSON_UTF_BASE_


#include <iostream>
#include <cx/base/string.h>
#include <cx/base/utfstring.h>

class CxJSONUTFBase;


//-------------------------------------------------------------------------
// CxJSONUTFBase
//
// Base class for UTF-8 aware JSON types. Uses CxUTFString for proper
// UTF-8 character handling in string values.
//-------------------------------------------------------------------------
class CxJSONUTFBase
{
  public:

    enum JSONObjectType { BASE, STRING, BOOLEAN, JNULL, NUMBER, OBJECT, ARRAY };

    CxJSONUTFBase(void);
    // constructor

    virtual ~CxJSONUTFBase(void);
    // virtual destructor for proper cleanup

    void dump( void );

    CxJSONUTFBase::JSONObjectType
	type(void);

    virtual CxString toJsonString(void) const;
    // returns the JSON representation as a CxString (raw UTF-8 bytes)

    virtual CxString toPrettyJsonString(int indent = 0) const;
    // returns pretty-printed JSON with indentation

  protected:

    JSONObjectType _type;

	virtual void print(std::ostream& str ) const;

  private:

	friend std::ostream& operator<<(std::ostream& str, const CxJSONUTFBase& base_ );
    // outputs to an ostream

};

#endif
