/***************************************************************************
 *
 *  rbuffer.h
 *
 *  CxRingBuffer Class
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


//-------------------------------------------------------------------------
// standard includes
//
//------------------------------------------------------------------------- 
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <cx/base/string.h>


//-------------------------------------------------------------------------
// standard includes
//
//------------------------------------------------------------------------- 
#if defined(_LINUX_) || defined(_OSX_)
#include <unistd.h>
#endif


   
#ifndef TRUE
#define TRUE 1
#endif
    
#ifndef FALSE
#define FALSE 0
#endif
    


#ifndef _CxRingBuffer_h_
#define _CxRingBuffer_h_




//-------------------------------------------------------------------------
// class CxRingBuffer
//
//------------------------------------------------------------------------- 
class CxRingBuffer
{
  public:

	CxRingBuffer( unsigned int len_ );
	// construct a ring buffer of len elements

   ~CxRingBuffer( void );
	// destructor

	unsigned long write( void *data_, unsigned long len_ );
	// write len bytes to ring

	unsigned long read( void *data_, unsigned long len_ );
	// read len bytes from fifo ring

	unsigned long size( void );
	// return number of elements in the ring

	int isEmpty( void ) const;
	// return true if no data is in the ring

  private:

    friend std::ostream& operator<<(std::ostream& str, const CxRingBuffer& b_ );
    // outputs a CxBuffer to an ostream

	unsigned char *_headPtr;
	unsigned char *_tailPtr;
	unsigned char *_startPtr;
	unsigned char *_endPtr;
	unsigned char *_data;

	unsigned _len;
};


#endif
