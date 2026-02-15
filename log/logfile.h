//-------------------------------------------------------------------------------------------------
//
//  logfile.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  File handling class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>


#include <cx/base/handle.h>
#include <cx/base/string.h>
#include <cx/base/filei.h>
#include <cx/base/prop.h>
#include <cx/base/time.h>

//-------------------------------------------------------------------------
// platform includes
//
//-------------------------------------------------------------------------

#if defined(_LINUX_) || defined(_SOLARIS_) || defined(_OSX_) || defined(_NETBSD_)
#undef getc
#include <pthread.h>
#endif

#if defined(_SUNOS_)
#include <sys/timeb.h>
int ftime(struct timeb *tp);
#endif

#if defined(_IRIX6_)
#include <pthread.h>
#endif


#ifndef _CxLOGFILE_H_
#define _CxLOGFILE_H_



//-------------------------------------------------------------------------
// class CxLogFile
//
//-------------------------------------------------------------------------
class CxLogFile
{
     
  public:

    enum logType    { CX_INFO, CX_ERROR, CX_DATA, CX_DETAIL };
    enum threadMode { NO_THREAD_ID,    INCLUDE_THREAD_ID };
    enum pidMode    { NO_PID,          INCLUDE_PID };
    enum timeMode   { NO_TIMESTAMP,    INCLUDE_TIMESTAMP };
    enum lineNoMode { NO_LINE_NUMBERS, INCLUDE_LINE_NUMBERS };

#define CXINFO    CxLogFile::CX_INFO,   __FILE__, __LINE__
#define CXERR     CxLogFile::CX_ERROR,  __FILE__, __LINE__
#define CXDATA    CxLogFile::CX_DATA,   __FILE__, __LINE__
#define CXDETAIL  CxLogFile::CX_DETAIL, __FILE__, __LINE__

	CxLogFile( void );
	// constructor

	CxLogFile( CxPropertyList initProps );
	// constructor with options

    CxLogFile( CxFileImpl *);
    // constructor taking a new implementation

	CxLogFile( const CxLogFile & );
	// copy constructor

	CxLogFile& operator=(const CxLogFile& );
	// assignment operator
     	
    virtual ~CxLogFile( void );
    // destructor

    int open( CxString path );
    // open the file with the supplied mode

    int open( CxString path, CxString mode );
    // open the file with the supplied mode

	int isOpen( void );

    void close( void );
    // close the file

    void printf( 
		CxLogFile::logType type, 
		const char       *file, 
		int         line, 
	    const char* format, 
		... );

    CxString path(void);
    // return the path of the file

	void initialize( CxPropertyList );

    static CxLogFile* getGlobalLogFile( void );


  protected:

	CxPropertyList  setDefaults( void );

    threadMode  _threadFlag;
	pidMode     _pidFlag;
    timeMode    _timeStampFlag;
    lineNoMode  _lineNoFlag;
    int         _maxFileSize;

  private:

    void setMaxSize( int );
    void setThreadMode( threadMode );
    void setPIDMode( pidMode );
    void setTimeStampMode( timeMode );
    void setLineNoMode( lineNoMode );

    static void setGlobalLogFile( CxLogFile* );


    CxHandle< CxFileImpl > _impl;
	// handle to the implementation (guts)

    static CxLogFile * _globalLog;

	int logFlags[4];
};


#endif
