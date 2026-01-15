/***************************************************************************
 *
 * file.h
 *
 * File handling class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, , Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/
//

//-------------------------------------------------------------------------
// standard includes
//
//-------------------------------------------------------------------------
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

	static CxString tempName(void);
	// generate a tempNamePath in the temp directory

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
