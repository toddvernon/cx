/***************************************************************************
 *
 *  CxRGBColor
 *
 *  CxRGBColor Class
 *
 ***************************************************************************
 *
 * (c) Copyright 2023 T.Vernon
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


//-------------------------------------------------------------------------
// CxRGBColor:: (constructor)
//
//-------------------------------------------------------------------------
CxRGBColor::CxRGBColor(void)
:_red(0), _blue(0), _green(0)
{
    _colorType  = RGB_BASE;
	_colorValid = INVALID;
}

CxRGBColor::CxRGBColor(CxRGBColor::color c_)
:_red(0), _blue(0), _green(0)
{
	_colorType  = RGB_BASE;
	_colorValid = c_;
}

CxRGBColor::CxRGBColor(int red_, int green_, int blue_)
{
	_red   = red_;
	_green = green_;
	_blue  = blue_;
	_colorValid = VALID;
}


//-------------------------------------------------------------------------
// CxRGBColor:: (constructor)
//
//-------------------------------------------------------------------------
CxRGBColor::CxRGBColor( const CxRGBColor& c_ )
{
    if (&c_ != this) {
        _red   = c_._red;
        _green = c_._green;
        _blue  = c_._blue;
		_colorValid = c_._colorValid;
    }
}


//-------------------------------------------------------------------------
// CxRGBColor:: (assignment operator)
//
//-------------------------------------------------------------------------
CxRGBColor
CxRGBColor::operator=( const CxRGBColor& c_)
{
    if (&c_ != this) {
        _red   = c_._red;
        _green = c_._green;
        _blue  = c_._blue;
		_colorValid = c_._colorValid;
    }
    
    return( *this );
}


//-------------------------------------------------------------------------
// CxRGBColor::red
//
//-------------------------------------------------------------------------
int
CxRGBColor::red(void)
{
    return(_red );
}


//-------------------------------------------------------------------------
// CxColor::green
//
//-------------------------------------------------------------------------
int
CxRGBColor::green(void)
{
    return(_green );
}


//-------------------------------------------------------------------------
// CxRGBColor::blue
//
//-------------------------------------------------------------------------
int
CxRGBColor::blue(void)
{
    return(_blue );
}


//-------------------------------------------------------------------------
// CxRGBColor::terminalString
//
//-------------------------------------------------------------------------
CxString
CxRGBColor::terminalString(void)
{
    CxString s;
    return( s );
}


//-------------------------------------------------------------------------
// CxRGBColor::resetTerminalString
//
//-------------------------------------------------------------------------
CxString
CxRGBColor::resetTerminalString(void)
{
    CxString s;
    return( s );
}


//=================================================================================================


//-------------------------------------------------------------------------
// CxRGBForegroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxRGBForegroundColor::CxRGBForegroundColor(void)
{
	_red = 0;
	_green = 0;
	_blue = 0;
    _colorType = RGB_FOREGROUND;
	_colorValid = INVALID;
}

CxRGBForegroundColor::CxRGBForegroundColor( CxRGBColor::color c_)
{
	_red = 0;
	_green = 0;
	_blue = 0;
	_colorType  = RGB_FOREGROUND;
	_colorValid = c_;
} 

CxRGBForegroundColor::CxRGBForegroundColor(int red_, int green_, int blue_)
{
    _colorType = RGB_FOREGROUND;
    _red   = red_;
    _green = green_;
    _blue  = blue_;
	_colorValid = VALID;
}


//-------------------------------------------------------------------------
// CxRGBForegroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxRGBForegroundColor::CxRGBForegroundColor( const CxRGBForegroundColor& c_ )
{
    if (&c_ != this) {
        _red   = c_._red;
        _green = c_._green;
        _blue  = c_._blue;
		_colorValid = c_._colorValid;
    }
}


//-------------------------------------------------------------------------
// CxRGBForegroundColor:: (assignment operator)
//
//-------------------------------------------------------------------------
CxRGBForegroundColor
CxRGBForegroundColor::operator=( const CxRGBForegroundColor& c_)
{
    if (&c_ != this) {
        _red   = c_._red;
        _green = c_._green;
        _blue  = c_._blue;
		_colorValid = c_._colorValid;
    }
    
    return( *this );
}

//-------------------------------------------------------------------------
// CxRGBForegroundColor::terminalString
//
//-------------------------------------------------------------------------
CxString
CxRGBForegroundColor::terminalString(void)
{
	if (_colorValid == INVALID) {
		CxString s;
		return(s);
	}

    char buffer[30];
    sprintf(buffer, "\033[38;2;%d;%d;%dm", _red, _green, _blue);
    return( buffer );
}

CxString
CxRGBForegroundColor::resetTerminalString(void)
{
    char buffer[30];
    sprintf(buffer, "\033[39m");
    return( buffer );
}



//-------------------------------------------------------------------------
// CxRGBBackgroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxRGBBackgroundColor::CxRGBBackgroundColor(void)
{
	_red = 0;
	_green = 0;
	_blue = 0;
    _colorType = RGB_BACKGROUND;
	_colorValid = INVALID;
}


CxRGBBackgroundColor::CxRGBBackgroundColor( CxRGBColor::color c_)
{
	_red = 0;
	_green = 0;
	_blue = 0;
	_colorType  = RGB_FOREGROUND;
	_colorValid = c_;
} 


CxRGBBackgroundColor::CxRGBBackgroundColor(int red_, int green_, int blue_)
{
    _red   = red_;
    _green = green_;
    _blue  = blue_;
	_colorValid = VALID;
}


//-------------------------------------------------------------------------
// CxRGBBackgroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxRGBBackgroundColor::CxRGBBackgroundColor( const CxRGBBackgroundColor& c_ )
{
    if (&c_ != this) {
        _red   = c_._red;
        _green = c_._green;
        _blue  = c_._blue;
		_colorValid = c_._colorValid;
    }
}


//-------------------------------------------------------------------------
// CxRGBBackgroundColor:: (assignment operator)
//
//-------------------------------------------------------------------------
CxRGBBackgroundColor
CxRGBBackgroundColor::operator=( const CxRGBBackgroundColor& c_)
{
    if (&c_ != this) {
        _red   = c_._red;
        _green = c_._green;
        _blue  = c_._blue;
		_colorValid = c_._colorValid;
    }
    
    return( *this );
}


//-------------------------------------------------------------------------
// CxRGBBackgroundColor::terminalString
//
//-------------------------------------------------------------------------
CxString
CxRGBBackgroundColor::terminalString(void)
{
	if (_colorValid == INVALID) {
		CxString s;
		return( s );
	}

    char buffer[30];
    sprintf(buffer, "\033[48;2;%d;%d;%dm", _red, _green, _blue);
    return( buffer );
}


CxString
CxRGBBackgroundColor::resetTerminalString(void)
{
    char buffer[30];
    sprintf(buffer, "\033[49m");
    return( buffer );
}



