//-------------------------------------------------------------------------------------------------
//
//  json_string.h
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


#ifndef _CXJSON_STRING_
#define _CXJSON_STRING_



//-------------------------------------------------------------------------
// CxJSONString
//
//-------------------------------------------------------------------------
class CxJSONString: public CxJSONBase
{
  public:

	CxJSONString( void );

	CxJSONString( CxString s );

	~CxJSONString( void );

	void 
	set(CxString s);

	CxString
	get( void );

    void dump( void );

  protected:

	virtual void print(std::ostream& str ) const;

  private:

	CxString _string;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONString& base_ );
    // outputs a CxString to an ostream

};


#endif
