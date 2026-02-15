//-------------------------------------------------------------------------------------------------
//
//  ansicolor.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxAnsiColor Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>

#include <cx/screen/color.h>


//-------------------------------------------------------------------------
// CxAnsiColor:: (constructor)
//
//-------------------------------------------------------------------------
CxAnsiColor::CxAnsiColor(void)
{
	_colorType  = ANSI_BASE;
	_colorValue = NONE;
}

CxString 
CxAnsiColor::terminalString(void)
{
    CxString s;
    return(s);
}

CxString 
CxAnsiColor::resetTerminalString(void)
{
    CxString s;
    return(s);
    
}



//=================================================================================================




//-------------------------------------------------------------------------
// CxAnsiForegroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxAnsiForegroundColor::CxAnsiForegroundColor(void)
{
    _colorType  = ANSI_FOREGROUND;
    _colorValue = NONE;
}


CxAnsiForegroundColor::CxAnsiForegroundColor( CxAnsiColor::color c_ )
{
	_colorValue = c_;
}


CxAnsiForegroundColor::CxAnsiForegroundColor( CxString colorName )
{
    _colorType  = ANSI_FOREGROUND;
	_colorValue = NONE;
   
	_colorMap.insert(CxString::toUpper("none"),0);
	_colorMap.insert(CxString::toUpper("reverse"),7);
	_colorMap.insert(CxString::toUpper("black"),30);
 	_colorMap.insert(CxString::toUpper("red"),31);
 	_colorMap.insert(CxString::toUpper("green"),32);
	_colorMap.insert(CxString::toUpper("yellow"),33);
	_colorMap.insert(CxString::toUpper("blue"),34);
	_colorMap.insert(CxString::toUpper("magenta"),35);
 	_colorMap.insert(CxString::toUpper("cyan"),36);
 	_colorMap.insert(CxString::toUpper("white"),37);

	_colorMap.insert(CxString::toUpper("bright_black"),90);
	_colorMap.insert(CxString::toUpper("bright_red"),91);
	_colorMap.insert(CxString::toUpper("bright_green"),92);
 	_colorMap.insert(CxString::toUpper("bright_yellow"),93);
 	_colorMap.insert(CxString::toUpper("bright_blue"),94);
	_colorMap.insert(CxString::toUpper("bright_magenta"),95);
	_colorMap.insert(CxString::toUpper("bright_cyan"),96);
	_colorMap.insert(CxString::toUpper("bright_white"),97);

	const int* v = _colorMap.find( CxString::toUpper(colorName));

    if (v != NULL) {
		_colorValue = *v;
	}
}



//-------------------------------------------------------------------------
// CxAnsiForegroundColor:: (copy constructor)
//
//-------------------------------------------------------------------------
CxAnsiForegroundColor::CxAnsiForegroundColor( const CxAnsiForegroundColor& c_ )
{
    if (&c_ != this) {
        _colorType    = c_._colorType;   // BUG FIX: was not copying _colorType
        _colorValue   = c_._colorValue;
    }
}


//-------------------------------------------------------------------------
// CxAnsiForegroundColor:: (assignment operator)
//
//-------------------------------------------------------------------------
CxAnsiForegroundColor
CxAnsiForegroundColor::operator=( const CxAnsiForegroundColor& c_)
{
    if (&c_ != this) {
        _colorType    = c_._colorType;   // BUG FIX: was not copying _colorType
        _colorValue   = c_._colorValue;
    }

    return( *this );
}


//-------------------------------------------------------------------------
// CxAnsiForegroundColor::value
//
//-------------------------------------------------------------------------
int
CxAnsiForegroundColor::value(void)
{
    return( _colorValue );
}


//-------------------------------------------------------------------------
// CxAnsiForegroundColor::valueAsInt
//
//-------------------------------------------------------------------------
int
CxAnsiForegroundColor::valueAsInt(void)
{
    return( (int) _colorValue );
}


//-------------------------------------------------------------------------
// CxAnsiForegroundColor::terminalString
//
//-------------------------------------------------------------------------
CxString
CxAnsiForegroundColor::terminalString(void)
{
    char buffer[30];
    
    CxString s;

    if (_colorValue == NONE ) {
        return( s );
    }

    sprintf(buffer, "\033[%dm", (int) _colorValue);
    return( buffer );
}


CxString
CxAnsiForegroundColor::resetTerminalString(void)
{
    char buffer[30];
    sprintf(buffer, "\033[39m");
    return( buffer );
}


//=================================================================================================


//-------------------------------------------------------------------------
// CxAnsiBackgroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxAnsiBackgroundColor::CxAnsiBackgroundColor(void)
{
    _colorType  = ANSI_BACKGROUND;
	_colorValue = NONE; 
}


CxAnsiBackgroundColor::CxAnsiBackgroundColor( CxAnsiColor::color c_ )
{
	_colorValue = c_;
}


CxAnsiBackgroundColor::CxAnsiBackgroundColor( CxString colorName )
{
    _colorType  = ANSI_BACKGROUND;
	_colorValue = NONE;

	_colorMap.insert(CxString::toUpper("none"),0);
	_colorMap.insert(CxString::toUpper("reverse"),7);
	_colorMap.insert(CxString::toUpper("black"),40);
 	_colorMap.insert(CxString::toUpper("red"),41);
 	_colorMap.insert(CxString::toUpper("green"),42);
	_colorMap.insert(CxString::toUpper("yellow"),43);
	_colorMap.insert(CxString::toUpper("blue"),44);
	_colorMap.insert(CxString::toUpper("magenta"),45);
 	_colorMap.insert(CxString::toUpper("cyan"),46);
 	_colorMap.insert(CxString::toUpper("white"),47);

	_colorMap.insert(CxString::toUpper("bright_black"),100);
	_colorMap.insert(CxString::toUpper("bright_red"),101);
	_colorMap.insert(CxString::toUpper("bright_green"),102);
 	_colorMap.insert(CxString::toUpper("bright_yellow"),103);
 	_colorMap.insert(CxString::toUpper("bright_blue"),104);
	_colorMap.insert(CxString::toUpper("bright_magenta"),105);
	_colorMap.insert(CxString::toUpper("bright_cyan"),106);
 	_colorMap.insert(CxString::toUpper("bright_white"),107);

	const int* v = _colorMap.find( CxString::toUpper(colorName));

    if (v != NULL) {
		_colorValue = *v;
	}
}



//-------------------------------------------------------------------------
// CxAnsiBackgroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxAnsiBackgroundColor::CxAnsiBackgroundColor( const CxAnsiBackgroundColor& c_ )
{
    if (&c_ != this) {
        _colorValue   = c_._colorValue;
    }
}


//-------------------------------------------------------------------------
// CxAnsiBackgroundColor:: (assignment operator)
//
//-------------------------------------------------------------------------
CxAnsiBackgroundColor
CxAnsiBackgroundColor::operator=( const CxAnsiBackgroundColor& c_)
{
    if (&c_ != this) {
        _colorValue   = c_._colorValue;
    }
    
    return( *this );
}


//-------------------------------------------------------------------------
// CxAnsiBackgroundColor::value
//
//-------------------------------------------------------------------------
int
CxAnsiBackgroundColor::value(void)
{
    return( _colorValue );
}


//-------------------------------------------------------------------------
// CxAnsiBackgroundColor::valueAsInt
//
//-------------------------------------------------------------------------
int
CxAnsiBackgroundColor::valueAsInt(void)
{
    return( (int) _colorValue );
}


//-------------------------------------------------------------------------
// CxAnsiBackgroundColor::terminalString
//
//-------------------------------------------------------------------------
CxString
CxAnsiBackgroundColor::terminalString(void)
{
    char buffer[30];

    CxString s;
    if (_colorValue ==  NONE) {
        return( s );
    }
    
    sprintf(buffer, "\033[%dm", (int) _colorValue);
    return( buffer );
}


CxString
CxAnsiBackgroundColor::resetTerminalString(void)
{
    char buffer[30];
    sprintf(buffer, "\033[49m");
    return( buffer );
}
