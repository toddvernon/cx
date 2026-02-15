//-------------------------------------------------------------------------------------------------
//
//  buffer.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxBuffer Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/buffer.h>

using namespace std;


//-------------------------------------------------------------------------
// CxBuffer::CxBuffer
//
//-------------------------------------------------------------------------
CxBuffer::CxBuffer( void ): 
_data( NULL ), _len(0)
{
}


//-------------------------------------------------------------------------
// CxBuffer::CxBuffer
//
//-------------------------------------------------------------------------
CxBuffer::CxBuffer( size_t len_ ): 
_data( NULL ), _len(0)
{
    unsigned char *cptr = new unsigned char[ len_ ];
    memset( cptr, 0, len_);

    _data = cptr;
    _len  = len_;
}


//-------------------------------------------------------------------------
// CxBuffer::CxBuffer
//
//-------------------------------------------------------------------------
CxBuffer::CxBuffer( const void* b_, unsigned int len_ )
: _data(NULL), _len(0)
{
    reAssign( b_, len_ );
}


//-------------------------------------------------------------------------
// CxBuffer::CxBuffer
//
//-------------------------------------------------------------------------
CxBuffer::CxBuffer( const CxBuffer& b_ )
: _data(NULL), _len(0)
{
    if ( &b_ != this ) {
	reAssign( b_.data(), b_.length() );
    }
}


//-------------------------------------------------------------------------
// CxBuffer::~CxBuffer
//
//-------------------------------------------------------------------------
CxBuffer::~CxBuffer( void )
{
	if (_len) delete[] _data;	
}


//-------------------------------------------------------------------------
// CxBuffer::operator=
//
//-------------------------------------------------------------------------
CxBuffer&
CxBuffer::operator=( const CxBuffer& b_ )
{
    if ( &b_ != this ) {
    	reAssign( b_.data(), b_.length() );
    }
    return( *this );
}


//-------------------------------------------------------------------------
// CxBuffer::append
//
//-------------------------------------------------------------------------
void
CxBuffer::append( void *buffer_ , unsigned int len_ )
{
    unsigned int newLen = _len + len_;

    unsigned char *cptr = new unsigned char[ newLen ];
    memset( cptr, 0, newLen );

    memcpy( cptr, &(_data[0]), _len );
    memcpy( &(cptr[_len]), buffer_, len_ );

    if (_len) delete[] _data;	

    _data = cptr;
    _len  = newLen;
}


//-------------------------------------------------------------------------
// CxBuffer::reAssign
//
//-------------------------------------------------------------------------
void
CxBuffer::reAssign( const void *vptr_, int len_ )
{
    if (_len) delete[] _data;	

    unsigned char *cptr = new unsigned char[ len_ ];
    memcpy( cptr, vptr_, len_ );

    _data = cptr;
    _len  = len_;
}


//-------------------------------------------------------------------------
// CxBuffer::data
//
//-------------------------------------------------------------------------
void *
CxBuffer::data( void ) const
{
	return( _data );
}


//-------------------------------------------------------------------------
// CxBuffer::length
//
//-------------------------------------------------------------------------
unsigned int
CxBuffer::length( void ) const
{
	return( _len );
}


//-------------------------------------------------------------------------
// CxBuffer::isEmpty
//
//-------------------------------------------------------------------------
int
CxBuffer::isEmpty(void) const
{
	if (_len) return( FALSE );
	return(TRUE);
}


//-------------------------------------------------------------------------
// CxBuffer::operator<<
//
//-------------------------------------------------------------------------
/*
ostream& operator<<(ostream& str, const CxBuffer& b_ )
{
	CxString left, right;
	int offset=0;

	unsigned char *bptr = b_._data;
	unsigned char data[100];
	unsigned char data2[100];

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
			str << left <<  "| " << right << endl;
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
		str << left <<  "| " << right << endl;
	} 


    return(str);
}
*/
 

//-------------------------------------------------------------------------
// CxBuffer::hashValue
//
//-------------------------------------------------------------------------
unsigned int
CxBuffer::hashValue( void ) const
{
	unsigned int i = 0;

	for (unsigned int c=0; c<_len; c++) {
		i += (unsigned int) _data[c];
	}
	
	return( i );
}
