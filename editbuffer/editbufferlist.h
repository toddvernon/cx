//
//  editbufferlist
//  EditBufferList
//
//  Created by Todd Vernon on 6/22/23.
//

#include <stdio.h>
#include <cx/base/slist.h>
#include <cx/editbuffer/editbuffer.h>


#ifndef editbufferlist_h
#define editbufferlist_h


class CxEditBufferList
{
    
public:
    
    CxEditBufferList( void );
    // default constructor
    
    CxEditBufferList( const CxEditBufferList& ebl_ );
    // copy constructor
    
    ~CxEditBufferList( void );
    // destructor
    
    CxEditBufferList& operator=( const CxEditBufferList& ebl_ );
    // assignment operator
    
    
    int items(void);
    // return the number of buffers
    
    int currentItemIndex(void);
    // return the index of the current item
    
    
    void add( CxEditBuffer *buffer );
    // add a buffer to the list
    
    CxEditBuffer *
    findPath( CxString path );
    // return the named buffer if it exists
    
    CxEditBuffer *current( void );
    // get the current edit buffer (last one asked for)
    
    CxEditBuffer *at( int n );
    // get the buffer at index
    
    CxEditBuffer *next( void );
    // get the next buffer in the list
    
    CxEditBuffer *first( void );
    // get the first buffer in the list
    
    CxEditBuffer *previous(void);
    // get the previous buffer in the list
    
    CxEditBuffer *last( void );
    // get the last buffer in the list
    
private:
    
    int currentIndex;
    CxSList< CxEditBuffer *> list;
    
};


#endif /* editbufferlist_h */
