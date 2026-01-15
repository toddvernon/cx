/***************************************************************************
 *
 *  CxColor
 *
 *  CxColor Class
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


#include <cx/screen/color.h>


//-------------------------------------------------------------------------------------------------
// CxColor
//
//-------------------------------------------------------------------------------------------------

CxColor::CxColor(void)
{
    _colorType = BASE;
}
 
 

CxColorType
CxColor::getColorType(void)
{
    return( _colorType );
}


CxString
CxColor::terminalString(void)
{
    CxString s;
    return( s );
}

CxString 
CxColor::resetTerminalString(void)
{
    CxString s;
    return( s );
}



