/***************************************************************************
 *
 *  json_array.h
 *
 *  CxJSONArray Class
 *
 ***************************************************************************
 *
 * (c) Copyright 2018 T.Vernon
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
