//-------------------------------------------------------------------------------------------------
//
//  json_base.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#ifndef _CXJSON_BASE_
#define _CXJSON_BASE_


#include <iostream>
#include <cx/base/string.h>

class CxJSONBase;


//-------------------------------------------------------------------------
// CxJSONBase
//
//-------------------------------------------------------------------------
class CxJSONBase
{
  public:

    enum JSONObjectType { BASE, STRING, BOOLEAN, JNULL, NUMBER, OBJECT, ARRAY };

    CxJSONBase(void);
    // constructor

    void dump( void );

    CxJSONBase::JSONObjectType
	type(void);

    virtual CxString toJsonString(void) const;
    // returns the JSON representation as a CxString (portable, no sstream needed)

  protected:

    JSONObjectType _type;

	virtual void print(std::ostream& str ) const;

  private:

	friend std::ostream& operator<<(std::ostream& str, const CxJSONBase& base_ );
    // outputs a CxString to an ostream

};

#endif
