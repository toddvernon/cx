/***************************************************************************
 *
 *  CxCursor
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
 
