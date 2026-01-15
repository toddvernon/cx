/***************************************************************************
 *
 * filei.h
 *
 * File handling class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, VStream, Inc., which
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
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

#include <cx/base/string.h>


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



//-------------------------------------------------------------------------
// local includes
//
//-------------------------------------------------------------------------
#include <cx/base/exception.h>
#include <cx/base/string.h>



#ifndef _CxFILEIMPL_
#define _CxFILEIMPL_


#define CxDEFAULT_FILESIZE 1000000


//-------------------------------------------------------------------------
// class CxFileImpl
//
//-------------------------------------------------------------------------
class CxFileImpl
{
    public:

        enum flushMode { DONT_FLUSH, ALWAYS_FLUSH };
        enum fType     { ALLOW_EOF,  NO_EOF       };

        CxFileImpl( void );
        // constructor

        virtual ~CxFileImpl( void );
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

    CxString fread( size_t s_ );
    
	size_t fwrite( void *ptr, size_t s_, size_t num_ );
	// stdio read

        int eof(void);
        // return if we reached end of file

        void setFlushMode( flushMode );
        // set how the file pointer is flushed

        void printf( const char *format, va_list& marker );
        // class version of the old favorite

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

        FILE *fd(void);
        // return the actual file pointer

	int isOpen(void);
	// return the open status

    protected:

        CxString    _path;
        CxString    _mode;
        FILE    *   _fd;
        flushMode   _flushFlag;
        int         _open;
};

#endif
