//-------------------------------------------------------------------------------------------------
//
//  cursor.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxCursor Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <cx/screen/cursor.h>


//-------------------------------------------------------------------------------------------------
// CxCursor
//
//
//-------------------------------------------------------------------------------------------------
CxCursor::CxCursor(void)
{
}
 
 
//-------------------------------------------------------------------------------------------------
// CxCursor::placeTerminalString
//
//
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::locateTerminalString(int r, int c)
{
	char buffer[100];
    sprintf(buffer, "\033[%d;%dH", r+1, c+1);
    CxString temp = buffer;
	return( temp );
}


//-------------------------------------------------------------------------------------------------
// CxCursor::showTerminalString
//
//
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::showTerminalString(void)
{
    CxString temp = "\033[?25h";
    return( temp );
}


//-------------------------------------------------------------------------------------------------
// CxCursor::hideTerminalString
//
//
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::hideTerminalString(void)
{    
    CxString temp = "\033[?25l";
    return( temp );
}


//-------------------------------------------------------------------------------------------------
// CxCursor::moveLeftTerminalString
//
//
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::moveLeftTerminalString( int num )
{
    char buffer[100];
    sprintf(buffer, "\033[%dD",  num );
    CxString temp = buffer;
    return( temp );
}


//-------------------------------------------------------------------------------------------------
// CxCursor::moveRightTerminalString
//
//
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::moveRightTerminalString( int num )
{
    char buffer[100];
    sprintf(buffer, "\033[%dC", num );
    CxString temp = buffer;
    return( temp );
}


//-------------------------------------------------------------------------------------------------
// CxCursor::moveToColumnTerminalString
//
//
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::moveToColumnTerminalString( int c )
{
    char buffer[100];
    sprintf(buffer, "\033[%dG", c+1 );
    CxString temp = buffer;
    return( temp );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::clearToEndOfLineTerminalString
//
//
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::clearToEndOfLineTerminalString( void )
{
    CxString temp = "\033[K";
    return( temp );
}


//-------------------------------------------------------------------------------------------------
// CxCursor::clearFromPositionDownTerminalString
//
// clears the remainder of the visible screen from cursor position right
// and down to end.
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::clearFromPositionDownTerminalString( void )
{
    CxString temp = "\033[J";
    return(temp);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::savePositionTerminalString
//
// Tell the terminal to save its current position
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::savePositionTerminalString(void)
{
    CxString temp = "\033[s";
    return(temp);
}


//-------------------------------------------------------------------------------------------------
// CxCursor::restorePositionTerminalString
//
// Tell the terminal to restore the cursor position to its saved location
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxCursor::restorePositionTerminalString(void)
{
    CxString temp = "\033[u";
    return(temp);
}
 
