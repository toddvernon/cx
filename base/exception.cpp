//-------------------------------------------------------------------------------------------------
//
//  exception.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxException and derived classes
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>


//-------------------------------------------------------------------------
// local includes
//
//-------------------------------------------------------------------------
#include <cx/base/string.h>
#include <cx/base/exception.h>


//-------------------------------------------------------------------------
// CxException::
//
//-------------------------------------------------------------------------
CxException::CxException( CxString s )
{
	_desc = s;
}


//-------------------------------------------------------------------------
// CxException::
//
//-------------------------------------------------------------------------
CxString
CxException::why( void ) const
{
	return( _desc );
}


//-------------------------------------------------------------------------
// CxIndexException::
//
//-------------------------------------------------------------------------
CxIndexException::CxIndexException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxSocketException::
//
//-------------------------------------------------------------------------
CxSocketException::CxSocketException( int e, CxString s )
:CxException( s )
{
    _errno = e;
}


//-------------------------------------------------------------------------
// CxSocketException::
//
//-------------------------------------------------------------------------
int
CxSocketException::error( void ) const
{   
    return(_errno);
}


//-------------------------------------------------------------------------
// CxFileException::
//
//-------------------------------------------------------------------------
CxFileException::CxFileException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxMutexException::
//
//-------------------------------------------------------------------------
CxMutexException::CxMutexException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxConditionTimeoutException::
//
//-------------------------------------------------------------------------
CxConditionTimeoutException::CxConditionTimeoutException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxCallbackException::
//
//-------------------------------------------------------------------------
CxCallbackException::CxCallbackException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxFtpParseException::
//
//-------------------------------------------------------------------------
CxFtpParseException::CxFtpParseException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxFtpReplyException::
//
//-------------------------------------------------------------------------
CxNetReplyException::CxNetReplyException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxFtpFileSystemException::
//
//-------------------------------------------------------------------------
CxFtpFileSystemException::CxFtpFileSystemException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxHttpClientException:
//
//-------------------------------------------------------------------------
CxHttpClientException::CxHttpClientException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxNutClientException:
//
//-------------------------------------------------------------------------
CxNutClientException::CxNutClientException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxPop3ClientException:
//
//-------------------------------------------------------------------------
CxPop3ClientException::CxPop3ClientException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxSmtpClientException:
//
//-------------------------------------------------------------------------
CxSmtpClientException::CxSmtpClientException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxDateTimeClientException:
//
//-------------------------------------------------------------------------
CxDateTimeClientException::CxDateTimeClientException( CxString s )
:CxException( s )
{
}


//-------------------------------------------------------------------------
// CxDBException:
//
//-------------------------------------------------------------------------
CxDBException::CxDBException( CxString s )
:CxException( s )
{
}
