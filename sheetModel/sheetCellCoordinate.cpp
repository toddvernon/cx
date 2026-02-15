//-------------------------------------------------------------------------------------------------
//
//  sheetCellCoordinate.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetCellCoordinate Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sheetCellCoordinate.h"


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::CxSheetCellCoordinate
//
// Default constructor - creates coordinate at A:1 (0,0)
//-------------------------------------------------------------------------
CxSheetCellCoordinate::CxSheetCellCoordinate(void)
: rowNum(0)
, colNum(0)
, rowAbsolute(0)
, colAbsolute(0)
{
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::CxSheetCellCoordinate
//
// Construct from numeric row and column (0-based)
//-------------------------------------------------------------------------
CxSheetCellCoordinate::CxSheetCellCoordinate(unsigned long row, unsigned long col)
: rowNum(row)
, colNum(col)
, rowAbsolute(0)
, colAbsolute(0)
{
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::CxSheetCellCoordinate
//
// Construct from spreadsheet address string
//-------------------------------------------------------------------------
CxSheetCellCoordinate::CxSheetCellCoordinate(CxString address)
: rowNum(0)
, colNum(0)
, rowAbsolute(0)
, colAbsolute(0)
{
    parseAddress(address);
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::CxSheetCellCoordinate
//
// Copy constructor
//-------------------------------------------------------------------------
CxSheetCellCoordinate::CxSheetCellCoordinate(const CxSheetCellCoordinate& other)
: rowNum(other.rowNum)
, colNum(other.colNum)
, rowAbsolute(other.rowAbsolute)
, colAbsolute(other.colAbsolute)
{
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::operator=
//
// Assignment operator
//-------------------------------------------------------------------------
CxSheetCellCoordinate&
CxSheetCellCoordinate::operator=(const CxSheetCellCoordinate& other)
{
    if (this != &other) {
        rowNum = other.rowNum;
        colNum = other.colNum;
        rowAbsolute = other.rowAbsolute;
        colAbsolute = other.colAbsolute;
    }
    return *this;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::operator==
//
// Equality comparison (required for CxHashmap key)
// Note: Only compares row/col, not absolute flags
//-------------------------------------------------------------------------
int
CxSheetCellCoordinate::operator==(const CxSheetCellCoordinate& other) const
{
    return (rowNum == other.rowNum && colNum == other.colNum);
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::hashValue
//
// Hash function (required for CxHashmap key)
// Combines row and column into a single hash value
//-------------------------------------------------------------------------
unsigned int
CxSheetCellCoordinate::hashValue(void) const
{
    // Simple hash combining row and column
    // Multiply row by a prime to spread values
    return (unsigned int)((rowNum * 65537) + colNum);
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::getRow
//
//-------------------------------------------------------------------------
unsigned long
CxSheetCellCoordinate::getRow(void) const
{
    return rowNum;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::getCol
//
//-------------------------------------------------------------------------
unsigned long
CxSheetCellCoordinate::getCol(void) const
{
    return colNum;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::setRow
//
//-------------------------------------------------------------------------
void
CxSheetCellCoordinate::setRow(unsigned long row)
{
    rowNum = row;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::setCol
//
//-------------------------------------------------------------------------
void
CxSheetCellCoordinate::setCol(unsigned long col)
{
    colNum = col;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::isRowAbsolute
//
//-------------------------------------------------------------------------
int
CxSheetCellCoordinate::isRowAbsolute(void) const
{
    return rowAbsolute;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::isColAbsolute
//
//-------------------------------------------------------------------------
int
CxSheetCellCoordinate::isColAbsolute(void) const
{
    return colAbsolute;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::setRowAbsolute
//
//-------------------------------------------------------------------------
void
CxSheetCellCoordinate::setRowAbsolute(int absolute)
{
    rowAbsolute = absolute ? 1 : 0;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::setColAbsolute
//
//-------------------------------------------------------------------------
void
CxSheetCellCoordinate::setColAbsolute(int absolute)
{
    colAbsolute = absolute ? 1 : 0;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::colToLetters
//
// Converts column number to letters (0=A, 25=Z, 26=AA, etc.)
//-------------------------------------------------------------------------
CxString
CxSheetCellCoordinate::colToLetters(unsigned long col) const
{
    char buffer[16];
    int pos = 15;
    buffer[pos] = '\0';

    // Handle the conversion similar to base-26, but 1-indexed (A=1, not A=0)
    unsigned long remaining = col + 1;  // Convert to 1-based for calculation

    while (remaining > 0) {
        pos--;
        remaining--;  // Adjust for 1-based
        buffer[pos] = 'A' + (remaining % 26);
        remaining = remaining / 26;
    }

    return CxString(&buffer[pos]);
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::lettersToCol
//
// Converts column letters to number (A=0, Z=25, AA=26, etc.)
//-------------------------------------------------------------------------
unsigned long
CxSheetCellCoordinate::lettersToCol(CxString letters) const
{
    unsigned long result = 0;
    int len = letters.length();

    for (int i = 0; i < len; i++) {
        char c = letters.charAt(i);
        if (c >= 'A' && c <= 'Z') {
            result = result * 26 + (c - 'A' + 1);
        } else if (c >= 'a' && c <= 'z') {
            result = result * 26 + (c - 'a' + 1);
        }
    }

    // Convert from 1-based to 0-based
    return result > 0 ? result - 1 : 0;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::toAddress
//
// Returns address without $ markers (e.g., "C:6")
// Row is displayed as 1-based (row 0 displays as "1")
//-------------------------------------------------------------------------
CxString
CxSheetCellCoordinate::toAddress(void) const
{
    CxString result;

    result = colToLetters(colNum);
    result = result + ":";
    result = result + CxString((unsigned long)(rowNum + 1));

    return result;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::toAbsoluteAddress
//
// Returns address with $ markers where set (e.g., "$C:$6")
//-------------------------------------------------------------------------
CxString
CxSheetCellCoordinate::toAbsoluteAddress(void) const
{
    CxString result;

    if (colAbsolute) {
        result = "$";
    }
    result = result + colToLetters(colNum);
    result = result + ":";
    if (rowAbsolute) {
        result = result + "$";
    }
    result = result + CxString((unsigned long)(rowNum + 1));

    return result;
}


//-------------------------------------------------------------------------
// CxSheetCellCoordinate::parseAddress
//
// Parses address string, returns 1 on success, 0 on failure
// Handles formats: "C:6", "$C:6", "C:$6", "$C:$6", "AA:100"
//-------------------------------------------------------------------------
int
CxSheetCellCoordinate::parseAddress(CxString address)
{
    const char* str = address.data();
    int len = address.length();
    int pos = 0;

    // Reset values
    rowNum = 0;
    colNum = 0;
    rowAbsolute = 0;
    colAbsolute = 0;

    if (len == 0) {
        return 0;
    }

    // Check for column absolute marker
    if (str[pos] == '$') {
        colAbsolute = 1;
        pos++;
    }

    // Parse column letters
    CxString colLetters;
    while (pos < len && ((str[pos] >= 'A' && str[pos] <= 'Z') ||
                         (str[pos] >= 'a' && str[pos] <= 'z'))) {
        colLetters = colLetters + CxString(str[pos]);
        pos++;
    }

    if (colLetters.length() == 0) {
        return 0;  // No column letters found
    }

    colNum = lettersToCol(colLetters);

    // Expect colon separator
    if (pos >= len || str[pos] != ':') {
        return 0;
    }
    pos++;

    // Check for row absolute marker
    if (pos < len && str[pos] == '$') {
        rowAbsolute = 1;
        pos++;
    }

    // Parse row number (1-based in display, convert to 0-based internal)
    unsigned long rowDisplay = 0;
    int hasDigits = 0;
    while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
        rowDisplay = rowDisplay * 10 + (str[pos] - '0');
        hasDigits = 1;
        pos++;
    }

    if (!hasDigits || rowDisplay == 0) {
        return 0;  // No row number or invalid row 0
    }

    rowNum = rowDisplay - 1;  // Convert to 0-based

    return 1;
}
