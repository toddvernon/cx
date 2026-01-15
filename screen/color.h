
/***************************************************************************
 *
 *  CxColor.h
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

#include <cx/base/string.h>
#include <cx/base/hashmap.h>

#ifndef _CxColor_
#define _CxColor_

enum CxColorType {
    BASE, ANSI_BASE, RGB_BASE,
    ANSI_FOREGROUND, ANSI_BACKGROUND,
    RGB_FOREGROUND,
    RGB_BACKGROUND,
    XTERM256_BASE,
    XTERM256_FOREGROUND,
    XTERM256_BACKGROUND
};


//-------------------------------------------------------------------------------------------------
// CxColor (Base Class)
//
//-------------------------------------------------------------------------------------------------
class CxColor
{
  public:

    CxColor( void );
    // base constructor
        
    CxColorType getColorType(void);
    // get the subclass
    
    virtual CxString terminalString(void);
    // get the terminal escape codes
    
    virtual CxString resetTerminalString(void);
    // reset the terminal
    
    CxColorType _colorType;
    int	_colorValid;

};


//-------------------------------------------------------------------------------------------------
// CxRGBColor
//
//-------------------------------------------------------------------------------------------------
class CxRGBColor: public CxColor
{
  public:

	enum color {
		INVALID = 0,
		VALID   = 1
	};
    

    CxRGBColor(void);
    // default constructor

	CxRGBColor( CxRGBColor::color c_);
	// constructor
    
    CxRGBColor( int red_, int green_, int blue_);
    // constructor
    
    CxRGBColor( const CxRGBColor& c_);
    // copy constructor
    
    CxRGBColor operator=( const CxRGBColor& c_);
    // assignment operator
        
    int red(void);
    // return red part
    
    int green(void);
    // return green part
    
    int blue(void);
    // return blue part
    
    virtual CxString terminalString(void);
    // return the string to send to the terminal
    
    virtual CxString resetTerminalString(void);

  protected:
  
    int _red;
    int _green;
    int _blue;
    
};


class CxRGBForegroundColor: public CxRGBColor
{
  public:
    
    CxRGBForegroundColor(void);

	CxRGBForegroundColor(CxRGBColor::color c_);
    
    CxRGBForegroundColor( int red_, int green_, int blue_);
    // constructor
    
    CxRGBForegroundColor( const CxRGBForegroundColor& c_ );
    // copy constructor
    
    CxRGBForegroundColor operator=( const CxRGBForegroundColor& c_);
    // assignment operator
    
    CxString terminalString(void);
    // return the string to send to the terminal
    
    CxString resetTerminalString(void);
    
};


class CxRGBBackgroundColor: public CxRGBColor
{
    public:
    
    CxRGBBackgroundColor(void);

	CxRGBBackgroundColor(CxRGBColor::color c_);
    
    CxRGBBackgroundColor( int red_, int green_, int blue_);
    // constructor
    
    CxRGBBackgroundColor( const CxRGBBackgroundColor& c_ );
    // copy constructor
    
    CxRGBBackgroundColor operator=( const CxRGBBackgroundColor& c_);
    // assignment operator
    
    CxString terminalString(void);
    // return the string to send to the terminal
    
    CxString resetTerminalString(void);

};


//-------------------------------------------------------------------------------------------------
// CxAnsiColor
//
//-------------------------------------------------------------------------------------------------
class CxAnsiColor: public CxColor
{
  public:

    enum color {
        NONE = -1
	};

    CxAnsiColor( void );
    
    CxString terminalString(void);
    
    CxString resetTerminalString(void);

    int _colorValue;

};


class CxAnsiForegroundColor: public CxAnsiColor
{
  public:
    
    CxAnsiForegroundColor( void );

	CxAnsiForegroundColor( CxAnsiColor::color c_);

    CxAnsiForegroundColor( CxString colorName );

    CxAnsiForegroundColor( const CxAnsiForegroundColor& c_);
    // copy constructor

    CxAnsiForegroundColor operator=( const CxAnsiForegroundColor& c_);
    // assignment operator
    
    int value(void);
    // return the value
    
    virtual int valueAsInt(void);
    // return the value as int
    
    virtual CxString terminalString(void);
    // return the string to send to the terminal
    
    virtual CxString resetTerminalString(void);

    
private:
    
    CxHashmap< CxString, int > _colorMap;    

};


class CxAnsiBackgroundColor: public CxAnsiColor
{
  public:

    CxAnsiBackgroundColor( void );

	CxAnsiBackgroundColor( CxAnsiColor::color colorValue);

    CxAnsiBackgroundColor( CxString colorName );

    CxAnsiBackgroundColor( const CxAnsiBackgroundColor& c_);
    // copy constructor

    CxAnsiBackgroundColor operator=( const CxAnsiBackgroundColor& c_);
    // assignment operator
    
    int value(void);
    // return the value
    
    virtual int valueAsInt(void);
    // return the value as int
    
    virtual CxString terminalString(void);
    // return the string to send to the terminalprivate:
    
    virtual CxString resetTerminalString(void);

  private:

    CxHashmap< CxString, int > _colorMap;  
};
   


// 256 color mode xterm     
// To use foreground and background colors from the extension, you only have to remember two escape codes:
// Set the foreground color to index N:	\033[38;5;${N}m
// Set the background color to index M:	\033[48;5;${M}m
// Check the "vim" section below for a small C program that matches RGB colors to xterm escape sequences.


//-------------------------------------------------------------------------------------------------
// CxXterm256Color
//
//-------------------------------------------------------------------------------------------------
class CxXterm256Color: public CxColor
{
    public:

    enum color {
        NONE            = -1
	};
    
    CxXterm256Color( void );
    
    CxString terminalString(void);
    
    CxString resetTerminalString(void);
    
    CxHashmap< CxString, int > _colorMap;

    int _value;


};


class CxXterm256ForegroundColor: public CxXterm256Color
{
public:
    
    CxXterm256ForegroundColor( void );

	CxXterm256ForegroundColor( CxXterm256Color::color c_);
   
    CxXterm256ForegroundColor( CxString colorName );

    CxXterm256ForegroundColor( const CxXterm256ForegroundColor& c_);
    // copy constructor

    CxXterm256ForegroundColor operator=( const CxXterm256ForegroundColor& c_);
    // assignment operator
    
    int value(void);
    // return the value
    
    virtual int valueAsInt(void);
    // return the value as int
    
    virtual CxString terminalString(void);
    // return the string to send to the terminal
    
    virtual CxString resetTerminalString(void);

   
    
};

class CxXterm256BackgroundColor: public CxXterm256Color
{
    
    public:
    
    CxXterm256BackgroundColor( void );

	CxXterm256BackgroundColor( CxXterm256Color::color c_);
   
    CxXterm256BackgroundColor( CxString colorName );

    CxXterm256BackgroundColor( const CxXterm256BackgroundColor& c_);
    // copy constructor

    CxXterm256BackgroundColor operator=( const CxXterm256BackgroundColor& c_);
    // assignment operator
    
    int value(void);
    // return the value
    
    virtual int valueAsInt(void);
    // return the value as int
    
    virtual CxString terminalString(void);
    // return the string to send to the terminalprivate:
    
    virtual CxString resetTerminalString(void);

};
        
#endif
