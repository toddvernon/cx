//-------------------------------------------------------------------------------------------------
//
//  star.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxMatchTemplate Class
//
//-------------------------------------------------------------------------------------------------

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

	CxMatchTemplate( const char *t );
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
