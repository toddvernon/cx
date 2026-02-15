//-------------------------------------------------------------------------------------------------
//
//  color.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxColor Class
//
//-------------------------------------------------------------------------------------------------

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
