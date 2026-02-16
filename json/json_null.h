//-------------------------------------------------------------------------------------------------
//
//  json_null.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/slist.h>

#include <cx/json/json_base.h>


#ifndef _CXJSON_NULL_
#define _CXJSON_NULL_



//-------------------------------------------------------------------------
// CxJSONBoolean
//
//-------------------------------------------------------------------------
class CxJSONNull: public CxJSONBase
{
  public:

	CxJSONNull( void );

	~CxJSONNull( void );

    virtual CxString toJsonString(void) const;

  protected:

    virtual void print(std::ostream& str ) const;

  private:

	int _i;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONNull& base_ );
    // outputs a CxString to an ostream
};


#endif
