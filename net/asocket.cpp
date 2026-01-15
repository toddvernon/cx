/***************************************************************************
 *
 *  asocket.cpp
 *
 *
 *
 ***************************************************************************
 *
 *
 *
 **************************************************************************/
    


#include "asocket.h"



//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
CxAsyncSocket::CxAsyncSocket( CxSocket socket_ ):
    _socket( socket_ ),
    _wireBuffer( 5000 ),
    _offset( 0 )
{    
}



//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
CxAsyncSocket::~CxAsyncSocket( void )
{
}


//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
int
CxAsyncSocket::writeBacklog( void )
{
    return( _toNetworkQueue.entries() );
}

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
int
CxAsyncSocket::readBacklog( void )
{
    return( _fromNetworkQueue.entries() );
}



//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
int
CxAsyncSocket::write( CxBuffer *buffer_ )
{   
    _toNetworkQueue.append( buffer_ );
    return( TRUE );
}



//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
CxBuffer *
CxAsyncSocket::read( void )
{
    CxBuffer *buffer = NULL;

    if (_fromNetworkQueue.entries()) {
        buffer = _fromNetworkQueue.at(0);
        _fromNetworkQueue.removeAt(0);
    }

    return( buffer );
}
 


//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
unsigned int
CxAsyncSocket::processIO( void )
{
    unsigned int flags = 0;

    _socket.select( 0, 0, &flags);

    if (flags & 1) this->doRead( );
    if (flags & 2) this->doWrite( );
    if (flags & 4) this->doException( );

    return( flags );
}    


//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
void
CxAsyncSocket::doRead( void )
{
    int len = 0;
        
    len = _socket.recv( (char*) _wireBuffer.data(), _wireBuffer.length() );

    if (len) {
        CxBuffer *nextBuffer = new CxBuffer( _wireBuffer.data(), len );
        _fromNetworkQueue.append( nextBuffer );
    }
}



//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
void
CxAsyncSocket::doWrite( void )
{
    int len = 0;
    CxBuffer *buffer = NULL;

    // look in the worklist to see if we have a buffer we are already working with
    if (_workList.entries()) {

        // we have a buffer, just pop it off the list
        buffer = _workList.pop();

    } else {

        // get a new buffer from the queue
        if (_toNetworkQueue.entries()) {

            buffer = _toNetworkQueue.at(0);
            _toNetworkQueue.removeAt(0);

            _offset = 0;

        } else {

            return;
        }
    }

    // push the current buffer onto the work list
    _workList.push( buffer );
        
    len = _socket.send( ((char*) buffer->data()) + _offset, buffer->length() - _offset );

    // see what we just sent
    if (len == buffer->length() - _offset) {

        CxBuffer *delBuffer = _workList.pop();
        delete delBuffer;

    } else {
        _offset += len;
    }
}


//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
void
CxAsyncSocket::doException( void )
{
}
