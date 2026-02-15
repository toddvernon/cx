//-------------------------------------------------------------------------------------------------
//
//  asocket.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  asocket.h
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/buffer.h>
#include <cx/base/slist.h>
#include <cx/net/socket.h>



#ifndef _cxasocket_
#define _cxasocket_



//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
class CxAsyncSocket
{
  public:

    CxAsyncSocket( CxSocket socket_ );
        
    virtual ~CxAsyncSocket( );
		
    int write( CxBuffer *buffer );
    
    CxBuffer *read( void );

    virtual unsigned int processIO( void );

    int readBacklog( void );
    int writeBacklog( void ); 
       
  protected:

    CxSocket       _socket;

  private:
    
    void doRead( void );
    void doWrite( void );
    void doException( void );

    int _offset;

    CxBuffer _wireBuffer;  
    CxSList< CxBuffer *>  _workList;  
    CxSList< CxBuffer *>  _fromNetworkQueue;
    CxSList< CxBuffer *>  _toNetworkQueue;

};




#endif
