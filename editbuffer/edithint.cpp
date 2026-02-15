//-------------------------------------------------------------------------------------------------
//
//  edithint.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  Created by Todd Vernon on 3/12/22.
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>

#include <cx/base/string.h>
#include <cx/editbuffer/edithint.h>


//-------------------------------------------------------------------------
// CxEditHint:: (constructor)
//
//-------------------------------------------------------------------------
CxEditHint::CxEditHint( void )
:_startRow(0), _startCol(0), _updateHint(UPDATE_HINT_NONE), _cursorHint(CURSOR_HINT_NONE)
{
}

//-------------------------------------------------------------------------
// CxEditHint:: (constructor)
//
//-------------------------------------------------------------------------
CxEditHint::CxEditHint( CxEditHint::UPDATE_HINT updateHint_)
:_startRow(0), _startCol(0), _updateHint(UPDATE_HINT_NONE), _cursorHint(CURSOR_HINT_NONE)
{
    _startRow = 0;
    _startCol = 0;
    _updateHint = updateHint_;
}

//-------------------------------------------------------------------------
// CxEditHint:: (constructor)
//
//-------------------------------------------------------------------------
CxEditHint::CxEditHint( CxEditHint::CURSOR_HINT cursorHint_)
:_startRow(0), _startCol(0), _updateHint(UPDATE_HINT_NONE), _cursorHint(CURSOR_HINT_NONE)
{
    _startRow = 0;
    _startCol = 0;
    _cursorHint = cursorHint_;
}

//-------------------------------------------------------------------------
// CxEditHint:: (constructor)
//
//-------------------------------------------------------------------------
CxEditHint::CxEditHint( CxEditHint::UPDATE_HINT updateHint_, CxEditHint::CURSOR_HINT cursorHint_)
:_startRow(0), _startCol(0), _updateHint(UPDATE_HINT_NONE), _cursorHint(CURSOR_HINT_NONE)
{
    _startRow = 0;
    _startCol = 0;
    _cursorHint = cursorHint_;
    _updateHint = updateHint_;
}


//-------------------------------------------------------------------------
// CxEditHint:: (constructor)
//
//-------------------------------------------------------------------------
CxEditHint::CxEditHint(
    unsigned long startRow_,
    unsigned long startCol_,
    CxEditHint::UPDATE_HINT updateHint_,
    CxEditHint::CURSOR_HINT cursorHint_)
:_startRow(0), _startCol(0), _updateHint(UPDATE_HINT_LINE), _cursorHint(CURSOR_HINT_NONE)
{
    _startRow = startRow_;
    _startCol = startCol_;
    _updateHint = updateHint_;
    _cursorHint = cursorHint_;
}


//-------------------------------------------------------------------------
// CxEditHint:: (copy constructor)
//
//-------------------------------------------------------------------------
CxEditHint::CxEditHint( const CxEditHint& hint_ )
:_startRow(0), _startCol(0), _updateHint(UPDATE_HINT_NONE), _cursorHint(CURSOR_HINT_NONE)
{
    if (&hint_ != this) {
        _startRow    = hint_._startRow;
        _startCol    = hint_._startCol;
        _updateHint  = hint_._updateHint;
        _cursorHint  = hint_._cursorHint;
    }
}


//-------------------------------------------------------------------------
// CxEditHint:: (assignment operator)
//
//-------------------------------------------------------------------------
CxEditHint
CxEditHint::operator=( const CxEditHint& hint_)
{
    if (&hint_ != this) {
        _startRow     = hint_._startRow;
        _startCol     = hint_._startCol;
        _updateHint   = hint_._updateHint;
        _cursorHint   = hint_._cursorHint;
    }
    
    return( *this );
}


CxEditHint::UPDATE_HINT
CxEditHint::updateHint(void)
{
    return( _updateHint );
}

CxEditHint::CURSOR_HINT
CxEditHint::cursorHint(void)
{
    return( _cursorHint );
}

int
CxEditHint::startRow(void)
{
    return( _startRow );
}

int
CxEditHint::startCol(void)
{
    return( _startCol );
}

void
CxEditHint::printHint(void)
{
    CxString updateStr;
    CxString cursorStr;
    
    switch( _updateHint ) {
        case UPDATE_HINT_NONE:
            updateStr.append("UPDATE_HINT_NONE");
            break;
        case UPDATE_HINT_LINE:                  // update entire row line
            updateStr.append("UPDATE_HINT_LINE");
            break;
        case UPDATE_HINT_LINE_PAST_POINT:       // update row line past col
            updateStr.append("UPDATE_HINT_LINE_PAST_POINT");
            break;
        case UPDATE_HINT_SCREEN:                // update entire screen
            updateStr.append("UPDATE_HINT_SCREEN");
            break;
        case UPDATE_HINT_SCREEN_PAST_POINT:     // update screen past row, col point
            updateStr.append("UPDATE_HINT_SCREEN_PAST_POINT");
    }
    
    switch( _cursorHint ) {
  
        case CURSOR_HINT_NONE:                     // cursor did not move
            cursorStr.append("CURSOR_HINT_NONE     ");
            break;
        case CURSOR_HINT_RIGHT:                    // cursor moved right one space
            cursorStr.append("CURSOR_HINT_RIGHT    ");
            break;
        case CURSOR_HINT_LEFT:                     // cursor moved left one space
            cursorStr.append("CURSOR_HINT_LEFT     ");
            break;
        case CURSOR_HINT_UP:                       // cursor moved up one space
            cursorStr.append("CURSOR_HINT_UP       ");
            break;
        case CURSOR_HINT_DOWN:                   // cursor moved down one space
            cursorStr.append("CURSOR_HINT_DOWN     ");
            break;
        case CURSOR_HINT_JUMP:                   // cursor jumped
            cursorStr.append("CURSOR_HINT_JUMP     ");
            break;
        case CURSOR_HINT_WRAP_DOWN:
            cursorStr.append("CURSOR_HINT_WRAP_DOWN");
            break;
        case CURSOR_HINT_WRAP_UP:
            cursorStr.append("CURSOR_HINT_WRAP_UP  ");
            break;
    }
    
    printf("%s:%s", updateStr.data(), cursorStr.data() );

}
