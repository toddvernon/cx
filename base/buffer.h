//-------------------------------------------------------------------------------------------------
//
//  buffer.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxBuffer Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
//#include <iostream>

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
    


#ifndef _CxBuffer_h_
#define _CxBuffer_h_




//-------------------------------------------------------------------------
// class CxBuffer
//
//------------------------------------------------------------------------- 
class CxBuffer
{
  public:

	CxBuffer( void );
	// constructor

	CxBuffer( const CxBuffer& b_ );
	// copy constructor

	CxBuffer( size_t len_ );
	// construct a buffer of certain size

	CxBuffer( const void * data_, unsigned int len_ );
	// construct a buffer from a pointer source of size len
	
   ~CxBuffer( void );
	// destructor

	CxBuffer& operator=( const CxBuffer& b_ );
	// assignment operator

	void append( void * data_, unsigned int len_ );
	// append a dataset to self

	unsigned int length( void ) const;
	// return the length of self

	void *data( void ) const;
	// return a pointer to data

	int isEmpty( void ) const;
	// return true if item has no data

	unsigned int hashValue( void ) const;
	// return a hash of self

  private:

  //  friend ostream& operator<<(ostream& str, const CxBuffer& b_ );
    // outputs a CxBuffer to an ostream (debug)
 
	void reAssign( const void *, int len=-1 );
	// internal reassignment

	unsigned char *_data;
	// internal pointer 

	unsigned _len;
	// internal length
};


#endif
