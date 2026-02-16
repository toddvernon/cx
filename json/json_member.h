//-------------------------------------------------------------------------------------------------
//
//  json_member.h
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

#ifndef _CXJSON_MEMBER_
#define _CXJSON_MEMBER_


class CxJSONBase;


//-------------------------------------------------------------------------
// CxJSONMember
//
//-------------------------------------------------------------------------
class CxJSONMember
{
  public:

    CxJSONMember(void);
    // constructor

    CxJSONMember( CxString, CxJSONBase *o );
    // construct from name and value strings

	~CxJSONMember( void );
	// destructor

    CxString var( void ) const;

    CxJSONBase *object(void);

	CxJSONBase *removeObject( void );

    void dump(void);

    CxString toJsonString(void) const;

  protected:

    virtual void print(std::ostream& str ) const;

  private:

    CxString _var;
    CxJSONBase *_object;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONMember& member_ );
    // outputs a CxString to an ostream

};

#endif
