//-------------------------------------------------------------------------------------------------
//
//  fileaccess.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  File availability handling class
//
//-------------------------------------------------------------------------------------------------

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
