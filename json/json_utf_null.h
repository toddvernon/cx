//-------------------------------------------------------------------------------------------------
//
//  json_utf_null.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFNull Class - UTF-8 aware JSON null
//
//-------------------------------------------------------------------------------------------------

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/slist.h>

#include <cx/json/json_utf_base.h>


#ifndef _CXJSON_UTF_NULL_
#define _CXJSON_UTF_NULL_



//-------------------------------------------------------------------------
// CxJSONUTFNull
//
//-------------------------------------------------------------------------
class CxJSONUTFNull: public CxJSONUTFBase
{
  public:

	CxJSONUTFNull( void );

	~CxJSONUTFNull( void );

    virtual CxString toJsonString(void) const;
    virtual CxString toPrettyJsonString(int indent = 0) const;

  protected:

    virtual void print(std::ostream& str ) const;

  private:

    friend std::ostream& operator<<(std::ostream& str, const CxJSONUTFNull& base_ );
    // outputs to an ostream
};


#endif
