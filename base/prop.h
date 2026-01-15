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

#include <cx/base/string.h>
#include <cx/base/slist.h>


#ifndef _PROPERTYLIST_
#define _PROPERTYLIST_


//-------------------------------------------------------------------------
// CxPropEntry
//
//-------------------------------------------------------------------------
class CxPropEntry
{
  public:

    CxPropEntry(void);
    // constructor

    CxPropEntry( const CxPropEntry& pe );
    // copy constructor

    CxPropEntry( CxString, CxString );
    // construct from name and value strings

    CxPropEntry&
    operator=(const CxPropEntry& pe );
    // assignment operator

    int
    operator==(const CxPropEntry& pe) const;
    // comparison operator

    CxString var( void ) const;
    CxString val( void ) const;

    CxString _var;
    CxString _val;
};



//-------------------------------------------------------------------------
// CxPropertyList
//
//-------------------------------------------------------------------------
class CxPropertyList
{
  public:

	CxPropertyList( void );

	CxPropertyList( const CxPropertyList& p_ );

	~CxPropertyList( void );

	CxPropertyList& 
	operator=( const CxPropertyList& p );

	CxPropertyList& 
	operator+( CxPropertyList& p );

	void 
	set(CxString p, CxString v);

	void 
	set( CxString varval_ );

	void
	import(CxString text_, CxString delims_);

	CxString 
	get( CxString p ) const;

	int 
	has( CxString p ) const;

	int 
	remove( CxString p );

	int 
	entries(void) const;
    
    static CxPropertyList
    parseArgs(CxString commandline, CxPropertyList argSet);

	void 
	at( int i, CxString *var, CxString *val) const;

	void 
	clear(void);

	void dump( void );

  private:

    CxSList< CxPropEntry > _list;
};


#endif
