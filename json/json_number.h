//-------------------------------------------------------------------------------------------------
//
//  json_number.h
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


#ifndef _CXJSON_NUMBER_
#define _CXJSON_NUMBER_



//-------------------------------------------------------------------------
// CxJSONNumber
//
//-------------------------------------------------------------------------
class CxJSONNumber: public CxJSONBase
{
  public:

	CxJSONNumber( void );

	CxJSONNumber( double d );

	~CxJSONNumber( void );

	void 
	set(double d);

	double
	get( void );

    void dump(void);

    virtual CxString toJsonString(void) const;

  protected:

    virtual void print(std::ostream& str ) const;


  private:

	double _d;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONNumber& base_ );
    // outputs a CxString to an ostream

};


#endif
