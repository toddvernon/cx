//-------------------------------------------------------------------------------------------------
//
//  sheetCell.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetCell Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>

//-------------------------------------------------------------------------------------------------
// cx library includes
//-------------------------------------------------------------------------------------------------
#include <cx/base/string.h>
#include <cx/base/double.h>
#include <cx/expression/expression.h>

#ifndef _CxSheetCell_
#define _CxSheetCell_


//-------------------------------------------------------------------------------------------------
//
// CxSheetCell
//
// Represents a single cell in the spreadsheet. A cell can be empty, contain text,
// contain a numeric value (CxDouble), or contain a formula (CxExpression).
// The formula is parsed once when set; the CxSheetModel sets the variable database
// before evaluation during recalculate().
//
//-------------------------------------------------------------------------------------------------

class CxSheetCell
{
  public:

    enum TYPE {
        EMPTY,              // the cell type is empty
        TEXT,               // the cell type is text
        DOUBLE,             // the cell type is double
        FORMULA             // the cell type is formula
    };

    CxSheetCell(void);
    // default constructor - creates empty cell

    CxSheetCell(CxString textValue);
    // construct as text cell

    CxSheetCell(CxDouble numericValue);
    // construct as double cell

    CxSheetCell(const CxSheetCell& other);
    // copy constructor

    ~CxSheetCell(void);
    // destructor

    CxSheetCell& operator=(const CxSheetCell& other);
    // assignment operator

    void clear(void);
    // reset cell to EMPTY state

    void setText(CxString textValue);
    // set cell as text type

    void setDouble(CxDouble numericValue);
    // set cell as double type

    void setFormula(CxString formulaText);
    // set cell as formula type (parses the formula string)

    TYPE getType(void) const;
    // get the cell type

    CxString getText(void) const;
    // get text value (valid if type is TEXT)

    CxDouble getDouble(void) const;
    // get double value (valid if type is DOUBLE)

    CxString getFormulaText(void) const;
    // get formula as text (valid if type is FORMULA)

    CxDouble getEvaluatedValue(void) const;
    // get the evaluated result (valid if type is DOUBLE or FORMULA)

    TYPE cellType;              // current type of the cell

    CxString text;              // text content (TEXT type) or formula text (FORMULA type)
    CxExpression* formula;      // parsed formula expression (owned pointer, FORMULA type only)
    CxDouble doubleValue;       // if cell is double this is populated
    CxDouble evaluatedValue;    // cached result for DOUBLE or evaluated FORMULA

    int displayDecimalPlaces;   // number of decimal places to display
    int displayCurrency;        // show $ symbol
    int displayCommas;          // display thousands separators

    // Cell formatting attributes (compatible with .cmrc color format)
    // Color strings use format: "ANSI:COLOR_NAME", "RGB:r,g,b", or "XTERM256:COLOR_NAME"
    // Empty string means no color specified (use default)
    int bold;                   // 1 = bold text, 0 = normal
    CxString fgColor;           // foreground color (e.g., "RGB:255,0,0" or "ANSI:RED")
    CxString bgColor;           // background color (e.g., "RGB:255,255,255" or "ANSI:WHITE")
};


#endif
