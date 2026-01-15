/***************************************************************************
 *
 *  error.h
 *
 *  CxError Class
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
