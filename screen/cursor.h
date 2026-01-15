
/***************************************************************************
 *
 *  CxCursor.h
 *
 *  CxCursor Class
 *
 ***************************************************************************
 *
 * (c) Copyright 2018 T.Vernon
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

#include <stdio.h>

#include <cx/base/string.h>

#ifndef _CxCursor_
#define _CxCursor_


//-------------------------------------------------------------------------------------------------
// CxCursor
//
//-------------------------------------------------------------------------------------------------
class CxCursor
{
    public:

    CxCursor( void );
    // constructor
        
    static CxString locateTerminalString(int r, int c);
    // return the place cursor terminal string

	static CxString hideTerminalString( void );
	// return the terminal string that hides the cursor

	static CxString showTerminalString( void );
	// return the terminal string that shows the cursor

    static CxString moveLeftTerminalString( int num );
    
    static CxString moveRightTerminalString( int num );
    
    static CxString moveToColumnTerminalString( int c );
    
    static CxString clearToEndOfLineTerminalString( void );
    
    static CxString clearFromPositionDownTerminalString( void );
    
    static CxString savePositionTerminalString(void);
    
    static CxString restorePositionTerminalString(void);
    
};

        
#endif


