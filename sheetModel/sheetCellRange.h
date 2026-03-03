//-------------------------------------------------------------------------------------------------
//
//  sheetCellRange.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetCellRange Class
//
//  Represents a range of cells in the spreadsheet (e.g., "A1:B4").
//  Used by formula evaluation to expand range arguments for functions like SUM, AVERAGE, etc.
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>

//-------------------------------------------------------------------------------------------------
// cx library includes
//-------------------------------------------------------------------------------------------------
#include <cx/base/string.h>
#include <cx/sheetModel/sheetCellCoordinate.h>

#ifndef _CxSheetCellRange_
#define _CxSheetCellRange_


//-------------------------------------------------------------------------------------------------
//
// CxSheetCellRange
//
// Represents a rectangular range of cells (e.g., "A1:D4", "$A$1:$B$2").
// Provides methods to:
//   - Parse range strings like "A1:D4"
//   - Count total cells in the range
//   - Iterate over cells in row-major order (A1, B1, C1, D1, A2, B2, ...)
//
// ITERATION ORDER:
// Cells are accessed in row-major order. For range A1:C2:
//   cellAt(0) = A1, cellAt(1) = B1, cellAt(2) = C1
//   cellAt(3) = A2, cellAt(4) = B2, cellAt(5) = C2
//
// INVERTED RANGES:
// If the end cell is "before" the start cell (e.g., "D4:A1"), the range
// is normalized so iteration still works correctly.
//
//-------------------------------------------------------------------------------------------------

class CxSheetCellRange
{
  public:

    CxSheetCellRange(void);
    // Default constructor - creates empty range

    CxSheetCellRange(CxString rangeStr);
    // Construct from range string (e.g., "A1:D4", "$A$1:$B$2")

    CxSheetCellRange(CxSheetCellCoordinate start, CxSheetCellCoordinate end);
    // Construct from two coordinates

    CxSheetCellRange(const CxSheetCellRange& other);
    // Copy constructor

    CxSheetCellRange& operator=(const CxSheetCellRange& other);
    // Assignment operator

    int parseRange(CxString rangeStr);
    // Parse a range string like "A1:D4" or "$A$1:$B$2"
    // Returns 1 on success, 0 on failure

    unsigned long cellCount(void) const;
    // Returns total number of cells in the range
    // For A1:C2, returns 6 (3 columns * 2 rows)

    CxSheetCellCoordinate cellAt(unsigned long index) const;
    // Get the nth cell in the range (0-indexed, row-major order)
    // For A1:C2: index 0=A1, 1=B1, 2=C1, 3=A2, 4=B2, 5=C2

    CxString toAddress(void) const;
    // Returns range as string without $ markers (e.g., "A1:D4")

    CxString toAbsoluteAddress(void) const;
    // Returns range with $ markers preserved (e.g., "$A$1:$D$4")

    CxSheetCellCoordinate getStartCell(void) const;
    // Returns the start cell of the range (normalized - upper-left)

    CxSheetCellCoordinate getEndCell(void) const;
    // Returns the end cell of the range (normalized - lower-right)

    int isValid(void) const;
    // Returns 1 if the range was successfully parsed, 0 otherwise

  private:

    void normalize(void);
    // Ensures startCell is upper-left and endCell is lower-right

    CxSheetCellCoordinate startCell;
    CxSheetCellCoordinate endCell;
    int valid;
};


#endif
