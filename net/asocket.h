/***************************************************************************
 *
 *  
 *
 *
 *
 ***************************************************************************
 *
 * (c) Copyright 2000 Evoke Communications
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Evoke Communications , which
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
