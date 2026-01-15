/***************************************************************************
 *
 *  cntbody.h
 *
 *  CxCountedBody Class
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
#include <stdlib.h>
#include <iostream>


#if defined(_LINUX_) || defined(_OSX_)
#include <unistd.h>
#endif

#if defined(_NT_)
#endif


#ifndef _CxCountedBody_h_
#define _CxCountedBody_h_


//-------------------------------------------------------------------------
// class CxCountedBody
//
//-------------------------------------------------------------------------
class CxCountedBody
{
  public:

    CxCountedBody();
	// constructor

    virtual ~CxCountedBody( void );
	// destructor

    void incCount( void );
	// increment the count

    void decCount( void );
	// decrement the count

    int count( void );
	// return the count

  private:

    int _count;
};


#endif
