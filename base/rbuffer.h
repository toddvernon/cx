//-------------------------------------------------------------------------------------------------
//
//  rbuffer.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxRingBuffer Class
//
//-------------------------------------------------------------------------------------------------

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
