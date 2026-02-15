//-------------------------------------------------------------------------------------------------
//
//  json_boolean.h
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


#ifndef _CXJSON_BOOLEAN_
#define _CXJSON_BOOLEAN_



//-------------------------------------------------------------------------
// CxJSONBoolean
//
//-------------------------------------------------------------------------
class CxJSONBoolean: public CxJSONBase
{
  public:

	CxJSONBoolean( void );

	CxJSONBoolean( int i );

	~CxJSONBoolean( void );

	void 
	set(int i);

	int 
	get( void );

	void dump(void);

  protected:

    virtual void print(std::ostream& str ) const;

  private:

	int _i;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONBoolean& base_ );
    // outputs a CxString to an ostream
};


#endif
