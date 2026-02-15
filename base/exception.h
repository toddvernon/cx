//-------------------------------------------------------------------------------------------------
//
//  exception.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxException Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>


//-------------------------------------------------------------------------
// local includes
//
//-------------------------------------------------------------------------
#include <cx/base/string.h>


#ifndef _CxException_
#define _CxException_


//-------------------------------------------------------------------------
// class CxException
//
//-------------------------------------------------------------------------
class CxException
{
  public:
	CxException( CxString s );
	CxString why( void ) const;
  private:
	CxString _desc;
};


//-------------------------------------------------------------------------
// class CxIndexException
//
//-------------------------------------------------------------------------
class CxIndexException: public CxException
{
  public:
	CxIndexException( CxString s );
};


//-------------------------------------------------------------------------
// class CxSocketException
//
//-------------------------------------------------------------------------
class CxSocketException: public CxException
{
  public:
	CxSocketException( int e, CxString s );
    int  error( void ) const;
    int _errno;
};


//-------------------------------------------------------------------------
// class CxFileException
//
//-------------------------------------------------------------------------
class CxFileException: public CxException
{
  public:
	CxFileException( CxString s );
};

//-------------------------------------------------------------------------
// class CxMutexException
//
//-------------------------------------------------------------------------
class CxMutexException: public CxException
{
  public:
    CxMutexException( CxString s );
};

//-------------------------------------------------------------------------
// class CxConditionTimeoutException
//
//-------------------------------------------------------------------------
class CxConditionTimeoutException: public CxException
{
  public:
    CxConditionTimeoutException( CxString s );
};

//-------------------------------------------------------------------------
// class CxCallbackException
//
//-------------------------------------------------------------------------
class CxCallbackException: public CxException
{
  public:
    CxCallbackException( CxString s );
};

//-------------------------------------------------------------------------
// class CxFtpParseException
//
//-------------------------------------------------------------------------
class CxFtpParseException: public CxException
{
  public:
    CxFtpParseException( CxString s );
};

//-------------------------------------------------------------------------
// class CxNetReplyException
//
//-------------------------------------------------------------------------
class CxNetReplyException: public CxException
{
  public:
    CxNetReplyException( CxString s );
};

//-------------------------------------------------------------------------
// class CxFtpFileSystemException
//
//-------------------------------------------------------------------------
class CxFtpFileSystemException: public CxException
{
  public:
    CxFtpFileSystemException( CxString s );
};

//-------------------------------------------------------------------------
// class CxHttpClientException
//
//-------------------------------------------------------------------------
class CxHttpClientException: public CxException
{
  public:
    CxHttpClientException( CxString s );
};

//-------------------------------------------------------------------------
// class CxNutClientException
//
//-------------------------------------------------------------------------
class CxNutClientException: public CxException
{
  public:
    CxNutClientException( CxString s );
};

//-------------------------------------------------------------------------
// class CxPop3ClientException
//
//-------------------------------------------------------------------------
class CxPop3ClientException: public CxException
{
  public:
    CxPop3ClientException( CxString s );
};

//-------------------------------------------------------------------------
// class CxSmtpClientException
//
//-------------------------------------------------------------------------
class CxSmtpClientException: public CxException
{
  public:
    CxSmtpClientException( CxString s );
};

//-------------------------------------------------------------------------
// class CxDateTimeClientException
//
//-------------------------------------------------------------------------
class CxDateTimeClientException: public CxException
{
  public:
    CxDateTimeClientException( CxString s );
};

//-------------------------------------------------------------------------
// class CxDBException
//
//-------------------------------------------------------------------------
class CxDBException: public CxException
{
  public:
    CxDBException( CxString s );
};

#endif
