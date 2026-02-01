//-------------------------------------------------------------------------------------------------
//
//  utfeditbufferlist.h
//
//  CxUTFEditBufferList - list of CxUTFEditBuffer pointers
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxUTFEditBufferList_h_
#define _CxUTFEditBufferList_h_

#include <stdio.h>
#include <cx/base/slist.h>
#include <cx/editbuffer/utfeditbuffer.h>


class CxUTFEditBufferList
{

public:

    CxUTFEditBufferList( void );
    // default constructor

    CxUTFEditBufferList( const CxUTFEditBufferList& ebl_ );
    // copy constructor

    ~CxUTFEditBufferList( void );
    // destructor

    CxUTFEditBufferList& operator=( const CxUTFEditBufferList& ebl_ );
    // assignment operator


    int items(void);
    // return the number of buffers

    int currentItemIndex(void);
    // return the index of the current item


    void add( CxUTFEditBuffer *buffer );
    // add a buffer to the list

    CxUTFEditBuffer *
    findPath( CxString path );
    // return the named buffer if it exists

    CxUTFEditBuffer *current( void );
    // get the current edit buffer (last one asked for)

    CxUTFEditBuffer *at( int n );
    // get the buffer at index

    CxUTFEditBuffer *next( void );
    // get the next buffer in the list

    CxUTFEditBuffer *first( void );
    // get the first buffer in the list

    CxUTFEditBuffer *previous(void);
    // get the previous buffer in the list

    CxUTFEditBuffer *last( void );
    // get the last buffer in the list

private:

    int currentIndex;
    CxSList< CxUTFEditBuffer *> list;

};


#endif
