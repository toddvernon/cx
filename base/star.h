/***************************************************************************
 *
 *  star.h
 *
 *  CxMatchTemplate Class
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

#include <stdio.h>

#include <sys/types.h>



#if defined(_LINUX_) || defined(_OSX_)
#include <unistd.h>
#include <sys/wait.h>
#endif

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/slist.h>


#ifndef _CxGLOB_
#define _CxGLOB_



//-------------------------------------------------------------------------
// CxStringMatch
//
//-------------------------------------------------------------------------
class CxStringMatch
{
  public:
	CxStringMatch( void );
	// constructor	
 
    CxStringMatch( CxString s );
	// copy constructor

	int isNext( CxString sub );
	// 

  private:

	int      _lastIndex;
	CxString _buffer;
};


//-------------------------------------------------------------------------
// CxMatchTemplate
//
//-------------------------------------------------------------------------
class CxMatchTemplate
{

  public:

    CxMatchTemplate( void );
	// constructor

	CxMatchTemplate( CxString t );
	// constructor

	CxMatchTemplate( char *t );
	// constructor

	CxMatchTemplate( const CxMatchTemplate& mt );
	// copy constructor

	CxMatchTemplate& operator=( const CxMatchTemplate& mt );
	// assignment operator

	int test( CxString candidate );
	// returns true if candidate string is a match to the wild
	// card template.	

  private:

	void parse( CxString );
	// pre-parse the template into a form ready to match against 

	CxSList< CxString > _parts;
	// list of parts to match against

	int      _leadingStar;
	int      _trailingStar;
	int      _matchAll;
	int      _matchNone;
	CxString _tString;
};


#endif
