//-------------------------------------------------------------------------------------------------
//
//  json_array.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONArray Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/json/json_base.h>


#ifndef _CXJSON_ARRAY_
#define _CXJSON_ARRAY_



//-------------------------------------------------------------------------
// CxJSONArray
//
//-------------------------------------------------------------------------
class CxJSONArray: public CxJSONBase
{
  public:

	CxJSONArray( void );

	~CxJSONArray( void );

	void
 	append( CxJSONBase *b );

    CxJSONBase *
	at( int c) const;

	int 
	entries(void) const;

	void 
	clear(void);

  protected:

	virtual void print(std::ostream& str ) const;

  private:

    CxSList< CxJSONBase *> _objectList;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONArray& a_ );
    // outputs a CxString to an ostream

};


#endif
