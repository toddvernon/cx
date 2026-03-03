//-------------------------------------------------------------------------------------------------
//
//  json_utf_number.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFNumber Class - UTF-8 aware JSON number
//
//-------------------------------------------------------------------------------------------------

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/slist.h>

#include <cx/json/json_utf_base.h>


#ifndef _CXJSON_UTF_NUMBER_
#define _CXJSON_UTF_NUMBER_



//-------------------------------------------------------------------------
// CxJSONUTFNumber
//
//-------------------------------------------------------------------------
class CxJSONUTFNumber: public CxJSONUTFBase
{
  public:

	CxJSONUTFNumber( void );

	CxJSONUTFNumber( double d );

	~CxJSONUTFNumber( void );

	void
	set(double d);

	double
	get( void );

    void dump(void);

    virtual CxString toJsonString(void) const;
    virtual CxString toPrettyJsonString(int indent = 0) const;

  protected:

    virtual void print(std::ostream& str ) const;


  private:

	double _d;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONUTFNumber& base_ );
    // outputs to an ostream

};


#endif
