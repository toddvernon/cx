//-------------------------------------------------------------------------------------------------
//
//  cursor.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  cursor.h
//
//-------------------------------------------------------------------------------------------------

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
