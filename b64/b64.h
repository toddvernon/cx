//-------------------------------------------------------------------------------------------------
//
//  b64.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  Cx64Encoder, Cx64Decoder Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <cx/base/slist.h>
#include <cx/base/string.h>
#include <cx/base/buffer.h>
#include <cx/base/rbuffer.h>


//-------------------------------------------------------------------------
// standard includes
//
//------------------------------------------------------------------------- 
#ifdef _LINUX_
#include <unistd.h>
#endif
   
#ifndef TRUE
#define TRUE 1
#endif
    
#ifndef FALSE
#define FALSE 0
#endif
    


#ifndef _CxB64_h_
#define _CxB64_h_




//-------------------------------------------------------------------------
// class CxB64Encoder
//
//------------------------------------------------------------------------- 
class CxB64Encoder
{
  public:

	CxB64Encoder( void );
   ~CxB64Encoder( void );

	void initialize(void);
	void process( void *data_, unsigned int len_, CxSList< CxString >& lineList_ );
	void finalize( CxSList< CxString >& lineList_ );

  private:

	void process_input( int finalize_ = 0 );
	void process_output( CxSList< CxString >& lineList, int finalize_ = 0 );
	void encode_buffer( unsigned char* in_, unsigned char* out_ );

	unsigned char _dtable[256];

	CxRingBuffer *_inBuffer;
	CxRingBuffer *_outBuffer;

};



//-------------------------------------------------------------------------
// class CxB64Decoder
//
//------------------------------------------------------------------------- 
class CxB64Decoder
{
  public:

	CxB64Decoder( void );
   ~CxB64Decoder( void );

	void initialize( void );
	CxBuffer process( CxSList< CxString >& lineList );
	CxBuffer finalize( void );

  private:

	void process_input( int finalize_ = 0 );
	void decode_buffer( unsigned char* in_, unsigned char* out_ );

	unsigned char _dtable[256];

	unsigned char *_outputData;
	unsigned char *_outputPtr;
	unsigned long  _outputLen;
	CxRingBuffer *_inBuffer;
};






#endif
