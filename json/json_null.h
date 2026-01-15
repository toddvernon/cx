/***************************************************************************
 *
 *  prop.h
 *
 *  CxPropEntry, CxPropertyList Class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, T.Vernon , which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/

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

  protected:

    virtual void print(std::ostream& str ) const;

  private:

	int _i;

    friend std::ostream& operator<<(std::ostream& str, const CxJSONNull& base_ );
    // outputs a CxString to an ostream
};


#endif
