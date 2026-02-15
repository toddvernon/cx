//-------------------------------------------------------------------------------------------------
//
//  sheetCell.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetCell Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sheetCell.h"


//-------------------------------------------------------------------------
// CxSheetCell::CxSheetCell
//
// Default constructor - creates empty cell
//-------------------------------------------------------------------------
CxSheetCell::CxSheetCell(void)
: cellType(EMPTY)
, formula(NULL)
, displayDecimalPlaces(2)
, displayCurrency(0)
, displayCommas(0)
, bold(0)
{
}


//-------------------------------------------------------------------------
// CxSheetCell::CxSheetCell
//
// Construct as text cell
//-------------------------------------------------------------------------
CxSheetCell::CxSheetCell(CxString textValue)
: cellType(TEXT)
, text(textValue)
, formula(NULL)
, displayDecimalPlaces(2)
, displayCurrency(0)
, displayCommas(0)
, bold(0)
{
}


//-------------------------------------------------------------------------
// CxSheetCell::CxSheetCell
//
// Construct as double cell
//-------------------------------------------------------------------------
CxSheetCell::CxSheetCell(CxDouble numericValue)
: cellType(DOUBLE)
, formula(NULL)
, doubleValue(numericValue)
, evaluatedValue(numericValue)
, displayDecimalPlaces(2)
, displayCurrency(0)
, displayCommas(0)
, bold(0)
{
}


//-------------------------------------------------------------------------
// CxSheetCell::CxSheetCell
//
// Copy constructor
//-------------------------------------------------------------------------
CxSheetCell::CxSheetCell(const CxSheetCell& other)
: cellType(other.cellType)
, text(other.text)
, formula(NULL)
, doubleValue(other.doubleValue)
, evaluatedValue(other.evaluatedValue)
, displayDecimalPlaces(other.displayDecimalPlaces)
, displayCurrency(other.displayCurrency)
, displayCommas(other.displayCommas)
, bold(other.bold)
, fgColor(other.fgColor)
, bgColor(other.bgColor)
{
    // Deep copy of formula if present
    if (other.formula != NULL) {
        // Reconstruct from the formula text
        formula = new CxExpression(other.text);
        formula->Parse();
    }
}


//-------------------------------------------------------------------------
// CxSheetCell::~CxSheetCell
//
// Destructor
//-------------------------------------------------------------------------
CxSheetCell::~CxSheetCell(void)
{
    if (formula != NULL) {
        delete formula;
        formula = NULL;
    }
}


//-------------------------------------------------------------------------
// CxSheetCell::operator=
//
// Assignment operator
//-------------------------------------------------------------------------
CxSheetCell&
CxSheetCell::operator=(const CxSheetCell& other)
{
    if (this != &other) {
        // Clean up existing formula
        if (formula != NULL) {
            delete formula;
            formula = NULL;
        }

        cellType = other.cellType;
        text = other.text;
        doubleValue = other.doubleValue;
        evaluatedValue = other.evaluatedValue;
        displayDecimalPlaces = other.displayDecimalPlaces;
        displayCurrency = other.displayCurrency;
        displayCommas = other.displayCommas;
        bold = other.bold;
        fgColor = other.fgColor;
        bgColor = other.bgColor;

        // Deep copy of formula if present
        if (other.formula != NULL) {
            formula = new CxExpression(other.text);
            formula->Parse();
        }
    }
    return *this;
}


//-------------------------------------------------------------------------
// CxSheetCell::clear
//
// Reset cell to EMPTY state
//-------------------------------------------------------------------------
void
CxSheetCell::clear(void)
{
    cellType = EMPTY;
    text = CxString();

    if (formula != NULL) {
        delete formula;
        formula = NULL;
    }

    doubleValue = CxDouble();
    evaluatedValue = CxDouble();

    // Reset formatting (but preserve display settings like decimalPlaces)
    bold = 0;
    fgColor = CxString();
    bgColor = CxString();
}


//-------------------------------------------------------------------------
// CxSheetCell::setText
//
// Set cell as text type
//-------------------------------------------------------------------------
void
CxSheetCell::setText(CxString textValue)
{
    clear();
    cellType = TEXT;
    text = textValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::setDouble
//
// Set cell as double type
//-------------------------------------------------------------------------
void
CxSheetCell::setDouble(CxDouble numericValue)
{
    clear();
    cellType = DOUBLE;
    doubleValue = numericValue;
    evaluatedValue = numericValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::setFormula
//
// Set cell as formula type
// Parses the formula; variable database is set by CxSheetModel before evaluation
//-------------------------------------------------------------------------
void
CxSheetCell::setFormula(CxString formulaText)
{
    clear();
    cellType = FORMULA;
    text = formulaText;

    formula = new CxExpression(formulaText);
    formula->Parse();
}


//-------------------------------------------------------------------------
// CxSheetCell::getType
//
// Get the cell type
//-------------------------------------------------------------------------
CxSheetCell::TYPE
CxSheetCell::getType(void) const
{
    return cellType;
}


//-------------------------------------------------------------------------
// CxSheetCell::getText
//
// Get text value (valid if type is TEXT)
//-------------------------------------------------------------------------
CxString
CxSheetCell::getText(void) const
{
    return text;
}


//-------------------------------------------------------------------------
// CxSheetCell::getDouble
//
// Get double value (valid if type is DOUBLE)
//-------------------------------------------------------------------------
CxDouble
CxSheetCell::getDouble(void) const
{
    return doubleValue;
}


//-------------------------------------------------------------------------
// CxSheetCell::getFormulaText
//
// Get formula as text (valid if type is FORMULA)
//-------------------------------------------------------------------------
CxString
CxSheetCell::getFormulaText(void) const
{
    return text;
}


//-------------------------------------------------------------------------
// CxSheetCell::getEvaluatedValue
//
// Get the evaluated result (valid if type is DOUBLE or FORMULA)
//-------------------------------------------------------------------------
CxDouble
CxSheetCell::getEvaluatedValue(void) const
{
    return evaluatedValue;
}
