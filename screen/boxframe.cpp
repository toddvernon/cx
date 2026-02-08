//-------------------------------------------------------------------------------------------------
//
// CxBoxFrame.cpp
//
// A reusable frame-drawing class for modal windows with box-drawing borders.
// Uses UTF-8 box drawing characters on Linux/Mac, ASCII fallback on other platforms.
//
//-------------------------------------------------------------------------------------------------
//
// (c) Copyright 2024 T.Vernon
// ALL RIGHTS RESERVED
//
//-------------------------------------------------------------------------------------------------

#include "boxframe.h"

//-------------------------------------------------------------------------------------------------
// Platform-conditional box drawing characters
//-------------------------------------------------------------------------------------------------
#if defined(_LINUX_) || defined(_OSX_)
// UTF-8 box drawing characters
static const char *BOX_TOP_LEFT     = "\xe2\x94\x8c";  // ┌
static const char *BOX_TOP_RIGHT    = "\xe2\x94\x90";  // ┐
static const char *BOX_BOTTOM_LEFT  = "\xe2\x94\x94";  // └
static const char *BOX_BOTTOM_RIGHT = "\xe2\x94\x98";  // ┘
static const char *BOX_HORIZONTAL   = "\xe2\x94\x80";  // ─
static const char *BOX_VERTICAL     = "\xe2\x94\x82";  // │
static const char *BOX_TEE_LEFT     = "\xe2\x94\x9c";  // ├
static const char *BOX_TEE_RIGHT    = "\xe2\x94\xa4";  // ┤
#else
// ASCII fallback for other platforms
static const char *BOX_TOP_LEFT     = "+";
static const char *BOX_TOP_RIGHT    = "+";
static const char *BOX_BOTTOM_LEFT  = "+";
static const char *BOX_BOTTOM_RIGHT = "+";
static const char *BOX_HORIZONTAL   = "-";
static const char *BOX_VERTICAL     = "|";
static const char *BOX_TEE_LEFT     = "+";
static const char *BOX_TEE_RIGHT    = "+";
#endif


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::CxBoxFrame (constructor)
//
//-------------------------------------------------------------------------------------------------
CxBoxFrame::CxBoxFrame(CxScreen *screen)
{
    _screen = screen;
    _fg = NULL;
    _bg = NULL;
    _top = 0;
    _left = 0;
    _bottom = 0;
    _right = 0;
    _colorsSet = 0;
}


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::setBounds
//
// Set the frame bounds (0-indexed, inclusive)
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::setBounds(int top, int left, int bottom, int right)
{
    _top = top;
    _left = left;
    _bottom = bottom;
    _right = right;
}


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::resize
//
// Resize to new bounds (alias for setBounds)
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::resize(int top, int left, int bottom, int right)
{
    setBounds(top, left, bottom, right);
}


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::setFrameColor
//
// Set the colors for the frame
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::setFrameColor(CxColor *fg, CxColor *bg)
{
    _fg = fg;
    _bg = bg;
    _colorsSet = 1;
}


//-------------------------------------------------------------------------------------------------
// Content area accessors
// For drawWithTitleAndFooter layout:
//   contentTop = _top + 3 (after top border, title line, separator)
//   contentBottom = _bottom - 3 (before footer separator, footer line, bottom border)
//-------------------------------------------------------------------------------------------------
int CxBoxFrame::contentTop(void)    { return _top + 3; }
int CxBoxFrame::contentLeft(void)   { return _left + 1; }
int CxBoxFrame::contentBottom(void) { return _bottom - 3; }
int CxBoxFrame::contentRight(void)  { return _right - 1; }
int CxBoxFrame::contentWidth(void)  { return _right - _left - 1; }
int CxBoxFrame::contentHeight(void) { return _bottom - _top - 5; }  // total - 6 frame rows + 1


//-------------------------------------------------------------------------------------------------
// Static box character accessors
//-------------------------------------------------------------------------------------------------
const char *CxBoxFrame::boxTopLeft(void)     { return BOX_TOP_LEFT; }
const char *CxBoxFrame::boxTopRight(void)    { return BOX_TOP_RIGHT; }
const char *CxBoxFrame::boxBottomLeft(void)  { return BOX_BOTTOM_LEFT; }
const char *CxBoxFrame::boxBottomRight(void) { return BOX_BOTTOM_RIGHT; }
const char *CxBoxFrame::boxHorizontal(void)  { return BOX_HORIZONTAL; }
const char *CxBoxFrame::boxVertical(void)    { return BOX_VERTICAL; }
const char *CxBoxFrame::boxTeeLeft(void)     { return BOX_TEE_LEFT; }
const char *CxBoxFrame::boxTeeRight(void)    { return BOX_TEE_RIGHT; }


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::drawHorizontalLine
//
// Draw a horizontal line at the specified row with given corner/edge characters
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::drawHorizontalLine(int row, const char *leftChar, const char *fillChar, const char *rightChar)
{
    CxString line;
    int width = _right - _left + 1;

    // Build the line
    line += leftChar;
    for (int c = 1; c < width - 1; c++) {
        line += fillChar;
    }
    line += rightChar;

    // Write to screen
    _screen->placeCursor(row, _left);
    _screen->writeText(line);
}


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::drawTitleLine
//
// Draw a line with centered title text
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::drawTitleLine(int row, CxString title, const char *leftChar, const char *rightChar)
{
    CxString line;
    int width = _right - _left + 1;
    int titleLen = title.length();
    int padding = (width - 2 - titleLen) / 2;  // -2 for left/right chars

    // Build the line
    line += leftChar;

    // Left padding
    for (int c = 0; c < padding; c++) {
        line += BOX_HORIZONTAL;
    }

    // Title with spaces
    line += " ";
    line += title;
    line += " ";

    // Right padding (fill remaining space)
    int used = 1 + padding + 1 + titleLen + 1;  // leftChar + padding + space + title + space
    for (int c = used; c < width - 1; c++) {
        line += BOX_HORIZONTAL;
    }

    line += rightChar;

    // Write to screen
    _screen->placeCursor(row, _left);
    _screen->writeText(line);
}


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::draw
//
// Draw the frame only (no title or footer)
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::draw(void)
{
    // Set colors if specified
    if (_colorsSet) {
        CxScreen::setForegroundColor(_fg);
        CxScreen::setBackgroundColor(_bg);
    }

    // Draw top border
    drawHorizontalLine(_top, BOX_TOP_LEFT, BOX_HORIZONTAL, BOX_TOP_RIGHT);

    // Draw side borders for each row
    for (int row = _top + 1; row < _bottom; row++) {
        _screen->placeCursor(row, _left);
        _screen->writeText(BOX_VERTICAL);
        _screen->placeCursor(row, _right);
        _screen->writeText(BOX_VERTICAL);
    }

    // Draw bottom border
    drawHorizontalLine(_bottom, BOX_BOTTOM_LEFT, BOX_HORIZONTAL, BOX_BOTTOM_RIGHT);

    // Reset colors if we set them
    if (_colorsSet) {
        CxScreen::resetColors();
    }
}


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::drawWithTitle
//
// Draw frame with a title bar
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::drawWithTitle(CxString title)
{
    // Set colors if specified
    if (_colorsSet) {
        CxScreen::setForegroundColor(_fg);
        CxScreen::setBackgroundColor(_bg);
    }

    // Draw top border with title
    drawTitleLine(_top, title, BOX_TOP_LEFT, BOX_TOP_RIGHT);

    // Draw separator line below title
    drawHorizontalLine(_top + 1, BOX_TEE_LEFT, BOX_HORIZONTAL, BOX_TEE_RIGHT);

    // Draw side borders for content area
    for (int row = _top + 2; row < _bottom; row++) {
        _screen->placeCursor(row, _left);
        _screen->writeText(BOX_VERTICAL);
        _screen->placeCursor(row, _right);
        _screen->writeText(BOX_VERTICAL);
    }

    // Draw bottom border
    drawHorizontalLine(_bottom, BOX_BOTTOM_LEFT, BOX_HORIZONTAL, BOX_BOTTOM_RIGHT);

    // Reset colors if we set them
    if (_colorsSet) {
        CxScreen::resetColors();
    }
}


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::drawWithTitleAndFooter
//
// Draw frame with title bar and footer bar.
// Layout:
//   Row 0: ┌───────────────────┐  (top border)
//   Row 1: │ Title text        │  (title line)
//   Row 2: ├───────────────────┤  (separator)
//   ...  : │ content           │  (content rows)
//   Row N-2: ├─────────────────┤  (footer separator)
//   Row N-1: │ footer text     │  (footer line)
//   Row N: └───────────────────┘  (bottom border)
//
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::drawWithTitleAndFooter(CxString title, CxString footer)
{
    // Set colors if specified
    if (_colorsSet) {
        CxScreen::setForegroundColor(_fg);
        CxScreen::setBackgroundColor(_bg);
    }

    int width = _right - _left + 1;

    // Row 0: Draw top border (plain horizontal line)
    drawHorizontalLine(_top, BOX_TOP_LEFT, BOX_HORIZONTAL, BOX_TOP_RIGHT);

    // Row 1: Draw title line (text between vertical bars)
    {
        CxString line;
        line += BOX_VERTICAL;
        line += " ";
        line += title;

        // Pad to fill width
        int used = 1 + 1 + title.length();
        for (int c = used; c < width - 1; c++) {
            line += " ";
        }
        line += BOX_VERTICAL;

        _screen->placeCursor(_top + 1, _left);
        _screen->writeText(line);
    }

    // Row 2: Draw separator line below title
    drawHorizontalLine(_top + 2, BOX_TEE_LEFT, BOX_HORIZONTAL, BOX_TEE_RIGHT);

    // Content rows: Draw side borders only (content filled by caller)
    for (int row = _top + 3; row < _bottom - 2; row++) {
        _screen->placeCursor(row, _left);
        _screen->writeText(BOX_VERTICAL);
        _screen->placeCursor(row, _right);
        _screen->writeText(BOX_VERTICAL);
    }

    // Row N-2: Draw separator line above footer
    drawHorizontalLine(_bottom - 2, BOX_TEE_LEFT, BOX_HORIZONTAL, BOX_TEE_RIGHT);

    // Row N-1: Draw footer line (side borders with footer text inside)
    {
        CxString line;
        line += BOX_VERTICAL;
        line += "  ";
        line += footer;

        // Pad to fill width
        int used = 1 + 2 + footer.length();
        for (int c = used; c < width - 1; c++) {
            line += " ";
        }
        line += BOX_VERTICAL;

        _screen->placeCursor(_bottom - 1, _left);
        _screen->writeText(line);
    }

    // Row N: Draw bottom border
    drawHorizontalLine(_bottom, BOX_BOTTOM_LEFT, BOX_HORIZONTAL, BOX_BOTTOM_RIGHT);

    // Reset colors if we set them
    if (_colorsSet) {
        CxScreen::resetColors();
    }
}


//-------------------------------------------------------------------------------------------------
// CxBoxFrame::clearContent
//
// Clear the content area inside the frame
//-------------------------------------------------------------------------------------------------
void
CxBoxFrame::clearContent(void)
{
    int startRow = _top + 1;
    int endRow = _bottom - 1;
    int startCol = _left + 1;
    int width = _right - _left - 1;

    for (int row = startRow; row <= endRow; row++) {
        _screen->placeCursor(row, startCol);
        for (int c = 0; c < width; c++) {
            _screen->writeText(" ");
        }
    }
}
