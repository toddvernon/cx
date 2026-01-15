/***************************************************************************
 *
 * fileaccess.h
 *
 * File availability handling class
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


#include <cx/base/file.h>


//-------------------------------------------------------------------------
// platform includes
//
//-------------------------------------------------------------------------


#ifndef _CxFILEACCESS_H_
#define _CxFILEACCESS_H_



//-------------------------------------------------------------------------
// class CxFileAccess
//
//-------------------------------------------------------------------------
class CxFileAccess
{
     
  public:

	// returns one of the follow status

	enum status {
        NOT_A_REGULAR_FILE,   // the path is not a regular file
		FOUND_W,            // file was found and is writable only
		FOUND_R,            // file was found and is readable only
        FOUND_RW,           // file was found and is read/writable
        NOT_FOUND_W,        // file was not found but one can be written
        NOT_FOUND           // file was not found and can't be written to (or read)
    };

    CxFileAccess(void);
    // constructor

	static
	CxFileAccess::status
	checkStatus( CxString path );
	// returns the state of the situation

};


#endif
