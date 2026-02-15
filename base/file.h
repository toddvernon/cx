//-------------------------------------------------------------------------------------------------
//
//  file.h
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
#include <unistd.h>
#include <iostream>

#include <cx/base/handle.h>
#include <cx/base/string.h>
#include <cx/base/filei.h>


//-------------------------------------------------------------------------
// platform includes
//
//-------------------------------------------------------------------------

#if defined(_NT_)
#include <windows.h>
#endif

#if defined(_LINUX_) || defined(_SOLARIS_) || defined(_OSX_) || defined(_NETBSD_)
#undef getc
#endif



#ifndef _CxFILE_H_
#define _CxFILE_H_



//-------------------------------------------------------------------------
// class CxFile
//
//-------------------------------------------------------------------------
class CxFile
{
     
  public:

    enum flushMode { DONT_FLUSH, ALWAYS_FLUSH };
    enum fType     { ALLOW_EOF,  NO_EOF       };

    CxFile( void );
    // constructor

    CxFile( CxFileImpl *);
    // constructor taking a new implementation

    CxFile( const CxFile & );
    // copy constructor

    CxFile& operator=(const CxFile& );
    // assignment operator
     	
    virtual ~CxFile( void );
    // destructor

    int open( CxString path, CxString mode );
    // open the file with the supplied mode

    void close( void );
    // close the file

    int attach( FILE *fd_, CxString mode );
    // open the file with the supplied mode

    CxString getch( fType f );
    // get next character

    CxString getUntil(char c);
    // get until character or end fo file

    size_t fread( void *ptr, size_t s_, size_t num_ );
    // stdio read

    CxString fread( size_t s_);
    
    size_t fwrite( void *ptr, size_t s_, size_t num_ );
    // stdio read

    int eof(void);
    // return if we reached end of file

    void setFlushMode( flushMode );
    // set how the file pointer is flushed

    void printf( const char *format, ... );
    // class version of the old favorite

    void seek( long, int );
    // seek in the file

    long tell( void );
    // return the current file pointer position        

    CxString path(void);
    // return the path of the file

    struct stat  getStat();  
    // return the status structor

    int isOpen(void);
    // return if file is open or not

	static CxString tempName(const char* prefix = NULL);
	// generate a tempNamePath in the temp directory
	// if prefix is provided, use it as the template (e.g., "myapp_XXXXXX")

	static CxString tildaExpansion(CxString path);
	// if path name starts with tilda expand it

  protected:

    FILE *fd(void);
    // return the actual file pointer


  private:

    CxHandle< CxFileImpl > _impl;
	// handle to the implementation (guts)
};


#endif
