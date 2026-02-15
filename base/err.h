//-------------------------------------------------------------------------------------------------
//
//  err.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxError Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <errno.h>


//-------------------------------------------------------------------------
// standard includes
//
//------------------------------------------------------------------------- 
#if defined(_LINUX_) || defined(_OSX_)
#include <unistd.h>
#endif


#include <cx/base/string.h>



#ifndef _CxError_h_
#define _CxError_h_


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


//-------------------------------------------------------------------------
// class CxError
//
//------------------------------------------------------------------------- 
class CxError
{
  public:

    static CxString buildOSErrorString( CxString ); 
	// build an error string based on text and os error condition
};


#endif
