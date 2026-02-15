//-------------------------------------------------------------------------------------------------
//
//  sheetCellCoordinate.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetCellCoordinate Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>

//-------------------------------------------------------------------------------------------------
// cx library includes
//-------------------------------------------------------------------------------------------------
#include <cx/base/string.h>

#ifndef _CxSheetCellCoordinate_
#define _CxSheetCellCoordinate_


//-------------------------------------------------------------------------------------------------
//
// CxSheetCellCoordinate
//
// Represents a cell address in the spreadsheet. Supports both numeric (row, col) and
// classic spreadsheet notation (A:1, $C:$6, AA:100). Tracks whether row and column
// references are absolute ($) or relative for copy/paste operations.
//
//-------------------------------------------------------------------------------------------------

class CxSheetCellCoordinate
{
  public:

    CxSheetCellCoordinate(void);
    // default constructor - creates coordinate at A:1 (0,0)

    CxSheetCellCoordinate(unsigned long row, unsigned long col);
    // construct from numeric row and column (0-based)

    CxSheetCellCoordinate(CxString address);
    // construct from spreadsheet address string (e.g., "C:6", "$AA:$100")
    // parses absolute markers ($) and stores them

    CxSheetCellCoordinate(const CxSheetCellCoordinate& other);
    // copy constructor

    CxSheetCellCoordinate& operator=(const CxSheetCellCoordinate& other);
    // assignment operator

    int operator==(const CxSheetCellCoordinate& other) const;
    // equality comparison (required for CxHashmap key)

    unsigned int hashValue(void) const;
    // hash function (required for CxHashmap key)

    // Accessors for numeric coordinates (0-based)
    unsigned long getRow(void) const;
    unsigned long getCol(void) const;
    void setRow(unsigned long row);
    void setCol(unsigned long col);

    // Accessors for absolute/relative flags
    int isRowAbsolute(void) const;
    int isColAbsolute(void) const;
    void setRowAbsolute(int absolute);
    void setColAbsolute(int absolute);

    // String representation methods
    CxString toAddress(void) const;
    // returns address without $ markers (e.g., "C:6")

    CxString toAbsoluteAddress(void) const;
    // returns address with $ markers where set (e.g., "$C:$6")

    CxString colToLetters(unsigned long col) const;
    // converts column number to letters (0=A, 25=Z, 26=AA, etc.)

    unsigned long lettersToCol(CxString letters) const;
    // converts column letters to number (A=0, Z=25, AA=26, etc.)

    int parseAddress(CxString address);
    // parses address string, returns 1 on success, 0 on failure

  private:

    unsigned long rowNum;       // 0-based row number
    unsigned long colNum;       // 0-based column number
    int rowAbsolute;            // 1 if row is absolute ($), 0 if relative
    int colAbsolute;            // 1 if column is absolute ($), 0 if relative
};


#endif
