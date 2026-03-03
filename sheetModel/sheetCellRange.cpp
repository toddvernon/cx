//-------------------------------------------------------------------------------------------------
//
//  sheetCellRange.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetCellRange Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sheetCellRange.h"


//-------------------------------------------------------------------------
// CxSheetCellRange::CxSheetCellRange
//
// Default constructor - creates empty/invalid range
//-------------------------------------------------------------------------
CxSheetCellRange::CxSheetCellRange(void)
: valid(0)
{
}


//-------------------------------------------------------------------------
// CxSheetCellRange::CxSheetCellRange
//
// Construct from range string (e.g., "A1:D4")
//-------------------------------------------------------------------------
CxSheetCellRange::CxSheetCellRange(CxString rangeStr)
: valid(0)
{
    parseRange(rangeStr);
}


//-------------------------------------------------------------------------
// CxSheetCellRange::CxSheetCellRange
//
// Construct from two coordinates
//-------------------------------------------------------------------------
CxSheetCellRange::CxSheetCellRange(CxSheetCellCoordinate start, CxSheetCellCoordinate end)
: startCell(start)
, endCell(end)
, valid(1)
{
    normalize();
}


//-------------------------------------------------------------------------
// CxSheetCellRange::CxSheetCellRange
//
// Copy constructor
//-------------------------------------------------------------------------
CxSheetCellRange::CxSheetCellRange(const CxSheetCellRange& other)
: startCell(other.startCell)
, endCell(other.endCell)
, valid(other.valid)
{
}


//-------------------------------------------------------------------------
// CxSheetCellRange::operator=
//
// Assignment operator
//-------------------------------------------------------------------------
CxSheetCellRange&
CxSheetCellRange::operator=(const CxSheetCellRange& other)
{
    if (this != &other) {
        startCell = other.startCell;
        endCell = other.endCell;
        valid = other.valid;
    }
    return *this;
}


//-------------------------------------------------------------------------
// CxSheetCellRange::parseRange
//
// Parse a range string like "A1:D4" or "$A$1:$B$2"
// Returns 1 on success, 0 on failure
//-------------------------------------------------------------------------
int
CxSheetCellRange::parseRange(CxString rangeStr)
{
    valid = 0;

    // Find the colon separator
    const char* str = rangeStr.data();
    int len = rangeStr.length();
    int colonPos = -1;

    for (int i = 0; i < len; i++) {
        if (str[i] == ':') {
            colonPos = i;
            break;
        }
    }

    // Must have exactly one colon, not at start or end
    if (colonPos <= 0 || colonPos >= len - 1) {
        return 0;
    }

    // Extract the two cell address strings
    CxString startStr;
    CxString endStr;

    // Copy characters before colon
    for (int i = 0; i < colonPos; i++) {
        startStr = startStr + CxString(str[i]);
    }

    // Copy characters after colon
    for (int i = colonPos + 1; i < len; i++) {
        endStr = endStr + CxString(str[i]);
    }

    // Parse both cell addresses
    if (!startCell.parseAddress(startStr)) {
        return 0;
    }

    if (!endCell.parseAddress(endStr)) {
        return 0;
    }

    // Normalize so start is upper-left, end is lower-right
    normalize();

    valid = 1;
    return 1;
}


//-------------------------------------------------------------------------
// CxSheetCellRange::normalize
//
// Ensures startCell is upper-left and endCell is lower-right
// Handles inverted ranges like "D4:A1"
//-------------------------------------------------------------------------
void
CxSheetCellRange::normalize(void)
{
    unsigned long startRow = startCell.getRow();
    unsigned long startCol = startCell.getCol();
    unsigned long endRow = endCell.getRow();
    unsigned long endCol = endCell.getCol();

    // Preserve absolute flags from original cells
    int startRowAbs = startCell.isRowAbsolute();
    int startColAbs = startCell.isColAbsolute();
    int endRowAbs = endCell.isRowAbsolute();
    int endColAbs = endCell.isColAbsolute();

    // Calculate normalized bounds
    unsigned long minRow = (startRow < endRow) ? startRow : endRow;
    unsigned long maxRow = (startRow > endRow) ? startRow : endRow;
    unsigned long minCol = (startCol < endCol) ? startCol : endCol;
    unsigned long maxCol = (startCol > endCol) ? startCol : endCol;

    // Update coordinates
    startCell.setRow(minRow);
    startCell.setCol(minCol);
    endCell.setRow(maxRow);
    endCell.setCol(maxCol);

    // Preserve absolute flags on the normalized cells
    // Start cell gets the flags from whichever original cell is now the start
    if (startRow <= endRow) {
        startCell.setRowAbsolute(startRowAbs);
    } else {
        startCell.setRowAbsolute(endRowAbs);
    }

    if (startCol <= endCol) {
        startCell.setColAbsolute(startColAbs);
    } else {
        startCell.setColAbsolute(endColAbs);
    }

    // End cell gets the flags from whichever original cell is now the end
    if (startRow <= endRow) {
        endCell.setRowAbsolute(endRowAbs);
    } else {
        endCell.setRowAbsolute(startRowAbs);
    }

    if (startCol <= endCol) {
        endCell.setColAbsolute(endColAbs);
    } else {
        endCell.setColAbsolute(startColAbs);
    }
}


//-------------------------------------------------------------------------
// CxSheetCellRange::cellCount
//
// Returns total number of cells in the range
//-------------------------------------------------------------------------
unsigned long
CxSheetCellRange::cellCount(void) const
{
    if (!valid) {
        return 0;
    }

    unsigned long numRows = endCell.getRow() - startCell.getRow() + 1;
    unsigned long numCols = endCell.getCol() - startCell.getCol() + 1;

    return numRows * numCols;
}


//-------------------------------------------------------------------------
// CxSheetCellRange::cellAt
//
// Get the nth cell in the range (0-indexed, row-major order)
// For A1:C2: index 0=A1, 1=B1, 2=C1, 3=A2, 4=B2, 5=C2
//-------------------------------------------------------------------------
CxSheetCellCoordinate
CxSheetCellRange::cellAt(unsigned long index) const
{
    if (!valid || index >= cellCount()) {
        return CxSheetCellCoordinate();
    }

    unsigned long numCols = endCell.getCol() - startCell.getCol() + 1;

    // Calculate row and column offsets
    unsigned long rowOffset = index / numCols;
    unsigned long colOffset = index % numCols;

    // Create coordinate for this cell
    CxSheetCellCoordinate coord(
        startCell.getRow() + rowOffset,
        startCell.getCol() + colOffset
    );

    // Inherit absolute flags from the range's start cell
    coord.setRowAbsolute(startCell.isRowAbsolute());
    coord.setColAbsolute(startCell.isColAbsolute());

    return coord;
}


//-------------------------------------------------------------------------
// CxSheetCellRange::toAddress
//
// Returns range as string without $ markers (e.g., "A1:D4")
//-------------------------------------------------------------------------
CxString
CxSheetCellRange::toAddress(void) const
{
    if (!valid) {
        return CxString("");
    }

    return startCell.toAddress() + CxString(":") + endCell.toAddress();
}


//-------------------------------------------------------------------------
// CxSheetCellRange::toAbsoluteAddress
//
// Returns range with $ markers preserved (e.g., "$A$1:$D$4")
//-------------------------------------------------------------------------
CxString
CxSheetCellRange::toAbsoluteAddress(void) const
{
    if (!valid) {
        return CxString("");
    }

    return startCell.toAbsoluteAddress() + CxString(":") + endCell.toAbsoluteAddress();
}


//-------------------------------------------------------------------------
// CxSheetCellRange::getStartCell
//
// Returns the start cell of the range (normalized - upper-left)
//-------------------------------------------------------------------------
CxSheetCellCoordinate
CxSheetCellRange::getStartCell(void) const
{
    return startCell;
}


//-------------------------------------------------------------------------
// CxSheetCellRange::getEndCell
//
// Returns the end cell of the range (normalized - lower-right)
//-------------------------------------------------------------------------
CxSheetCellCoordinate
CxSheetCellRange::getEndCell(void) const
{
    return endCell;
}


//-------------------------------------------------------------------------
// CxSheetCellRange::isValid
//
// Returns 1 if the range was successfully parsed, 0 otherwise
//-------------------------------------------------------------------------
int
CxSheetCellRange::isValid(void) const
{
    return valid;
}
