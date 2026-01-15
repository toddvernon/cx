/***************************************************************************
 *
 *  base
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

#ifndef _CXJSON_BASE_
#define _CXJSON_BASE_


#include <iostream>

class CxJSONBase;


//-------------------------------------------------------------------------
// CxJSONBase
//
//-------------------------------------------------------------------------
class CxJSONBase
{
  public:

    enum JSONObjectType { BASE, STRING, BOOLEAN, JNULL, NUMBER, OBJECT, ARRAY };

    CxJSONBase(void);
    // constructor

    void dump( void );

    CxJSONBase::JSONObjectType
	type(void);

  protected:

    JSONObjectType _type;

	virtual void print(std::ostream& str ) const;
 
  private:
	
	friend std::ostream& operator<<(std::ostream& str, const CxJSONBase& base_ );    
    // outputs a CxString to an ostream 

};

#endif
