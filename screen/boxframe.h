//-------------------------------------------------------------------------------------------------
//
//  boxframe.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  A reusable frame-drawing class for modal windows with box-drawing borders.
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxBoxFrame_h_
#define _CxBoxFrame_h_

#include <cx/screen/screen.h>
#include <cx/screen/color.h>
#include <cx/base/string.h>

class CxBoxFrame
{
public:
    CxBoxFrame(CxScreen *screen);
    // constructor

    // Set the frame bounds (0-indexed, inclusive)
    void setBounds(int top, int left, int bottom, int right);

    // Resize to new bounds
    void resize(int top, int left, int bottom, int right);

    // Get internal content area dimensions
    int contentTop(void);      // first row inside frame
    int contentLeft(void);     // first col inside frame
    int contentBottom(void);   // last row inside frame
    int contentRight(void);    // last col inside frame
    int contentWidth(void);    // width of content area
    int contentHeight(void);   // height of content area

    // Draw methods
    void draw(void);                                    // frame only
    void drawWithTitle(CxString title);                 // frame + title bar
    void drawWithTitleAndFooter(CxString title, CxString footer);
    void drawFooter(CxString footer);                   // redraw footer only

    // Optional: set colors (uses current screen colors if not set)
    void setFrameColor(CxColor *fg, CxColor *bg);

    // Clear the content area
    void clearContent(void);

    // Platform-conditional box characters (public for external use)
    static const char *boxTopLeft(void);
    static const char *boxTopRight(void);
    static const char *boxBottomLeft(void);
    static const char *boxBottomRight(void);
    static const char *boxHorizontal(void);
    static const char *boxVertical(void);
    static const char *boxTeeLeft(void);    // for separator lines
    static const char *boxTeeRight(void);   // for separator lines

private:
    CxScreen *_screen;
    CxColor *_fg;
    CxColor *_bg;
    int _top, _left, _bottom, _right;
    int _colorsSet;

    void drawHorizontalLine(int row, const char *leftChar, const char *fillChar, const char *rightChar);
    void drawTitleLine(int row, CxString title, const char *leftChar, const char *rightChar);
};

#endif
