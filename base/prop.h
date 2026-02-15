//-------------------------------------------------------------------------------------------------
//
//  prop.h
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
