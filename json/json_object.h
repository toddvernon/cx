//-------------------------------------------------------------------------------------------------
//
//  json_object.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/base/slist.h>

#include <cx/json/json_base.h>
#include <cx/json/json_member.h>


#ifndef _CXJSON_OBJECT_
#define _CXJSON_OBJECT_



//-------------------------------------------------------------------------
// CxJSONObject
//
//-------------------------------------------------------------------------
class CxJSONObject: public CxJSONBase
{
  public:

	CxJSONObject( void );

	~CxJSONObject( void );

	void 
	append(CxJSONMember *m);

    CxJSONMember *
	at( int c) const;

	CxJSONMember *
	removeAt( int i );

	CxJSONMember *
	find( CxString name);

	int 
	entries(void) const;

	void
	clear(void);

    virtual CxString toJsonString(void) const;

  protected:

    virtual void print(std::ostream& str ) const;

  private:

    CxSList< CxJSONMember *> _memberList;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONObject& o_ );
    // outputs a CxString to an ostream

};


#endif
