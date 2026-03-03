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
#include <cx/json/json_utf_object.h>

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
// APP ATTRIBUTES (appAttributes)
// ------------------------------
// The appAttributes field provides a mechanism for application-layer data to be stored
// with cells without sheetModel needing to understand or interpret that data. This keeps
// the sheetModel library focused on computation (values, formulas, dependencies) while
// allowing applications to store display/formatting attributes.
//
// How it works:
//   - appAttributes is a CxJSONObject* that stores arbitrary key-value pairs
//   - During save, sheetModel merges these keys directly into the cell's JSON object
//   - During load, sheetModel collects any unknown keys into appAttributes
//   - sheetModel never interprets these values - they are opaque
//
// Round-trip preservation:
//   A processing engine that only uses sheetModel for computation will automatically
//   preserve app attributes it doesn't understand. For example:
//     1. App "ss" saves a cell with {"bold": true, "fgColor": "RGB:255,0,0"}
//     2. A computation engine loads the sheet, does calculations, saves it
//     3. The bold and fgColor attributes are preserved even though the engine
//        never touched them
//
// File format example:
//   {
//     "cells": [
//       {"cell": "A1", "type": "double", "value": 42.5, "bold": true, "fgColor": "RGB:255,0,0"},
//       {"cell": "B1", "type": "text", "text": "Hello", "bgColor": "ANSI:YELLOW"}
//     ]
//   }
//
// Note: The keys appear as first-class JSON members (not a nested string), making
// files easy to hand-edit and process with standard JSON tools.
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

    CxJSONUTFObject* appAttributes;  // application-defined attributes (see class comment above)

    // Helper methods for appAttributes
    void setAppAttribute(const char* key, const char* value);
    void setAppAttribute(const char* key, int value);
    void setAppAttribute(const char* key, double value);
    void setAppAttribute(const char* key, bool value);

    CxString getAppAttributeString(const char* key, const char* defaultValue = "") const;
    int getAppAttributeInt(const char* key, int defaultValue = 0) const;
    double getAppAttributeDouble(const char* key, double defaultValue = 0.0) const;
    bool getAppAttributeBool(const char* key, bool defaultValue = false) const;

    bool hasAppAttribute(const char* key) const;
    void removeAppAttribute(const char* key);
};


#endif
