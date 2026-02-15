//-------------------------------------------------------------------------------------------------
//
//  rbuffer.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxRingBuffer Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/rbuffer.h>


//-------------------------------------------------------------------------
// CxRingBuffer::CxRingBuffer
//
//-------------------------------------------------------------------------
CxRingBuffer::CxRingBuffer( unsigned int len_): 
_data( NULL ), _len(0)
{
	_len      = len_;
	_data     = new unsigned char[ len_ ];
	_headPtr  = _data;
	_tailPtr  = _data;
	_startPtr = _data;
	_endPtr   = _data + (len_-1);
}


//-------------------------------------------------------------------------
// CxRingBuffer::~CxRingBuffer
//
//-------------------------------------------------------------------------
CxRingBuffer::~CxRingBuffer( void )
{
	if (_len) delete[] _data;	
}


//-------------------------------------------------------------------------
// CxRingBuffer::size
//
//-------------------------------------------------------------------------
unsigned long
CxRingBuffer::size( void )
{
	return( _len );
}


//-------------------------------------------------------------------------
// CxRingBuffer::write
//
//-------------------------------------------------------------------------
unsigned long
CxRingBuffer::write( void *buffer_ , unsigned long len_ )
{
	unsigned char *ptr = (unsigned char *) buffer_;

	//---------------------------------------------------------------------
	// check for the two full conditions
	//
	//---------------------------------------------------------------------
	if ( _headPtr == (_tailPtr-1)) return( 0 );
	if (( _headPtr == _endPtr) && (_tailPtr == _startPtr )) return( 0 );


	for (unsigned int c=0; c<len_; c++) {

		//-----------------------------------------------------------------
		// insert the data
		//-----------------------------------------------------------------
		*_headPtr = *ptr; 

		//-----------------------------------------------------------------
		// inc the head pointer
		//-----------------------------------------------------------------
		_headPtr++; ptr++;

		//-----------------------------------------------------------------
		// check for overflow and wrap
		//-----------------------------------------------------------------
		if (_headPtr > _endPtr) _headPtr = _startPtr;

		//-----------------------------------------------------------------
		// check for the two full conditions
		//
		//-----------------------------------------------------------------
		if ( _headPtr == (_tailPtr-1)) return( c+1 );
		if (( _headPtr == _endPtr) && (_tailPtr == _startPtr )) return( c+1 );
	}
	
	return( len_ );
}



//-------------------------------------------------------------------------
// CxRingBuffer::read
//
//-------------------------------------------------------------------------
unsigned long
CxRingBuffer::read( void *data_, unsigned long bytes_ )
{
	unsigned char *destPtr = (unsigned char *) data_;

	//---------------------------------------------------------------------
	// check for empty condition
	//---------------------------------------------------------------------
	if ( _tailPtr == _headPtr ) return ( 0 );


	for (unsigned long c=0; c<bytes_; c++) {

		//-----------------------------------------------------------------
		// get the data
		//-----------------------------------------------------------------
		*destPtr = *_tailPtr;

		//-----------------------------------------------------------------
		// inc the head pointer
		//-----------------------------------------------------------------
		_tailPtr++; destPtr++;

		//-----------------------------------------------------------------
		// check for overflow and wrap
		//-----------------------------------------------------------------
		if (_tailPtr > _endPtr) _tailPtr = _startPtr;

		//-----------------------------------------------------------------
		// check for empty condition
		//-----------------------------------------------------------------
		if ( _tailPtr == _headPtr ) return ( c+1 );
	}
	
	return( bytes_ );
}



//-------------------------------------------------------------------------
// CxRingBuffer::isEmpty
//
//-------------------------------------------------------------------------
int
CxRingBuffer::isEmpty(void) const
{
	if (_tailPtr == _headPtr) return( TRUE );
	return(FALSE);
}


//-------------------------------------------------------------------------
// CxBuffer::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxRingBuffer& b_ )
{
    CxString left, right;
    int offset=0;
     
    unsigned char *bptr = b_._data;
    unsigned char data[1000];
    unsigned char data2[100];
  
	sprintf((char*) data, "startPtr=0x%lx; tailPtr=0x%lx; headPtr=0x%lx; endPtr=0x%lx", 
		(unsigned long) b_._startPtr, 
		(unsigned long) b_._tailPtr, 
		(unsigned long) b_._headPtr, 
		(unsigned long) b_._endPtr ); 

	str << data << std::endl;


    for (unsigned int b=0; b<b_._len; b++) {
    
        sprintf((char*) data, "%x ", *bptr );
        if (strlen( (char*) data)<3) left += CxString("0");
        left  +=  CxString( (char*) data );
 
        if ((*bptr >= 32) && (*bptr <=126)) {
            sprintf((char*) data2, "%c", *bptr );
            right += CxString( (char*) data2 );
        } else {
            right += CxString(".");
        }

        offset++;
        bptr++;
    
        if (offset == 16 ) {
            str << left <<  "| " << right << std::endl;
            offset = 0;
            left  = "";
            right = "";
        }
    }

    if ((offset < 16) && (offset > 0)) {
        while ( offset < 16 ) {
            left  += ".. ";
            right += " ";
            offset++;
        }
        str << left <<  "| " << right << std::endl;
    }
  

    return(str);
}
