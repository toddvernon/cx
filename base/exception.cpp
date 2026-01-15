/***************************************************************************
 *
 *  exception.cpp
 *
 *  CxException and derived classes
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, T.Vernon , which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/


//-------------------------------------------------------------------------
// standard includes 
//
//-------------------------------------------------------------------------
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
