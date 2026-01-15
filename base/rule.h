/***************************************************************************
 *
 *  rule.h
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
#include <cx/base/file.h>


#ifndef _RULELIST_
#define _RULELIST_


//-------------------------------------------------------------------------
// CxRuleEntry
//
//-------------------------------------------------------------------------
class CxRuleEntry
{
  public:

    	CxRuleEntry(void);
	// constructor

    	CxRuleEntry( const CxRuleEntry& re );
	// copy constructor

    	CxRuleEntry( CxString, CxString );
	// construct from name and value strings

	CxRuleEntry(int year, 
		int bmonth_, int emonth_, 
		int bday_,   int eday_, 
		int dow_,
		int bhour_,  int ehour_,
		int bminute_, int eminute_, 
		CxString message );
	// construct a new rule


	CxRuleEntry&
	operator=(const CxRuleEntry& re );
	// assignment operator

	int
    	operator==(const CxRuleEntry& re) const;
	// comparison operator

	int isMatch( int year, int month, int day, int dow, int hour, int minute);
	// return true if candidate matches rule

	static int dayname_to_number( CxString dayname);
	// method to change a day name string to a number

	CxString message( void );


    	CxString _message;

	int _year;
	int _bmonth;
	int _emonth;
	int _bday;
	int _eday;
	int _dow;
	int _bhour;
	int _ehour;
	int _bminute;
	int _eminute;

};



//-------------------------------------------------------------------------
// CxRuleList
//
//-------------------------------------------------------------------------
class CxRuleList
{
  public:

	CxRuleList( void );

	CxRuleList( const CxRuleList& rl_ );

	~CxRuleList( void );

	CxRuleList& 
	operator=( const CxRuleList& rl );

	CxRuleList& 
	operator+( CxRuleList& rl );

	void 
	append( CxRuleEntry re );

	CxString getMessage( int year, int month, int day, int dow, int hour, int minute);
	// return message if a rule matches


	int 
	entries( void ) const;

	CxRuleEntry
	at( int n );
	
	void 
	clear(void);

	void dump( void );

	static CxRuleList loadFromFile( CxString file );

  private:

    	CxSList< CxRuleEntry > _list;
};


#endif
