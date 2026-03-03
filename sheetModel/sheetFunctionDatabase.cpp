//-------------------------------------------------------------------------------------------------
//
//  sheetFunctionDatabase.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetFunctionDatabase Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "sheetFunctionDatabase.h"
#include "sheetCellRange.h"
#include "sheetModel.h"
#include "sheetCell.h"


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::CxSheetFunctionDatabase
//
// Default constructor
//-------------------------------------------------------------------------
CxSheetFunctionDatabase::CxSheetFunctionDatabase(void)
: sheetModel(0)
{
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::CxSheetFunctionDatabase
//
// Constructor with model reference
//-------------------------------------------------------------------------
CxSheetFunctionDatabase::CxSheetFunctionDatabase(CxSheetModel* model)
: sheetModel(model)
{
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::setModel
//
// Set the sheet model used for resolving cell values
//-------------------------------------------------------------------------
void
CxSheetFunctionDatabase::setModel(CxSheetModel* model)
{
    sheetModel = model;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::setRangeList
//
// Set the list of ranges from the current expression
//-------------------------------------------------------------------------
void
CxSheetFunctionDatabase::setRangeList(CxSList<CxString> rangeList)
{
    currentRangeList = rangeList;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::isRangeFunction
//
// Check if this is a function that accepts range arguments
// Function names are case-insensitive (SUM, Sum, sum all work)
//-------------------------------------------------------------------------
int
CxSheetFunctionDatabase::isRangeFunction(CxString name)
{
    if (name.equalsIgnoreCase("SUM"))     return 1;
    if (name.equalsIgnoreCase("AVERAGE")) return 1;
    if (name.equalsIgnoreCase("COUNT"))   return 1;
    if (name.equalsIgnoreCase("MIN"))     return 1;
    if (name.equalsIgnoreCase("MAX"))     return 1;
    return 0;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::FunctionDefined
//
// Check if a function name is defined
// Function names are case-insensitive (SUM, Sum, sum all work)
//-------------------------------------------------------------------------
CxExpressionFunctionDatabase::returnCode
CxSheetFunctionDatabase::FunctionDefined(CxString name)
{
    if (name.equalsIgnoreCase("SUM"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("AVERAGE")) return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("COUNT"))   return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("MIN"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("MAX"))     return FUNCTION_DEFINED;

    return FUNCTION_UNDEFINED;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::expandRange
//
// Expand a range string to a list of numeric values from the cells
//-------------------------------------------------------------------------
CxSList<double>
CxSheetFunctionDatabase::expandRange(CxString rangeStr)
{
    CxSList<double> values;

    if (sheetModel == 0) {
        return values;
    }

    CxSheetCellRange range(rangeStr);
    if (!range.isValid()) {
        return values;
    }

    unsigned long count = range.cellCount();
    for (unsigned long i = 0; i < count; i++) {
        CxSheetCellCoordinate coord = range.cellAt(i);
        CxSheetCell cell = sheetModel->getCell(coord);

        // Get the numeric value from the cell
        switch (cell.getType()) {
            case CxSheetCell::DOUBLE:
                values.append(cell.getDouble().value);
                break;

            case CxSheetCell::FORMULA:
                values.append(cell.getEvaluatedValue().value);
                break;

            case CxSheetCell::TEXT:
                // Text cells are treated as 0 in numeric contexts
                values.append(0.0);
                break;

            case CxSheetCell::EMPTY:
            default:
                // Empty cells are treated as 0
                values.append(0.0);
                break;
        }
    }

    return values;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::sum
//
// Sum all values
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::sum(CxSList<double>& values)
{
    double total = 0.0;
    for (int i = 0; i < values.entries(); i++) {
        total += values.at(i);
    }
    return total;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::average
//
// Average of all values
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::average(CxSList<double>& values)
{
    if (values.entries() == 0) {
        return 0.0;
    }
    return sum(values) / values.entries();
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::count
//
// Count of values (including zeros from empty cells)
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::count(CxSList<double>& values)
{
    return (double)values.entries();
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::min
//
// Minimum value
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::min(CxSList<double>& values)
{
    if (values.entries() == 0) {
        return 0.0;
    }

    double minVal = values.at(0);
    for (int i = 1; i < values.entries(); i++) {
        if (values.at(i) < minVal) {
            minVal = values.at(i);
        }
    }
    return minVal;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::max
//
// Maximum value
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::max(CxSList<double>& values)
{
    if (values.entries() == 0) {
        return 0.0;
    }

    double maxVal = values.at(0);
    for (int i = 1; i < values.entries(); i++) {
        if (values.at(i) > maxVal) {
            maxVal = values.at(i);
        }
    }
    return maxVal;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::FunctionEvaluate
//
// Evaluate function with scalar arguments only
// For range-aware functions, this checks if any arguments might be ranges
// by looking at the currentRangeList
//-------------------------------------------------------------------------
CxExpressionFunctionDatabase::returnCode
CxSheetFunctionDatabase::FunctionEvaluate(
    CxString name,
    int numberOfArgs,
    double *args,
    double *result)
{
    // For functions that support ranges, check if we have ranges
    // in the current expression. If so, expand them.
    if (isRangeFunction(name) && currentRangeList.entries() > 0) {
        // Collect all values (from ranges and scalars)
        CxSList<double> allValues;

        int rangeIndex = 0;
        for (int i = 0; i < numberOfArgs; i++) {
            // Check if this argument was a range (value is 0.0 placeholder)
            // and we have more ranges to process
            if (rangeIndex < currentRangeList.entries() && args[i] == 0.0) {
                // This might be a range - expand it
                CxSList<double> rangeValues = expandRange(currentRangeList.at(rangeIndex));
                for (int j = 0; j < rangeValues.entries(); j++) {
                    allValues.append(rangeValues.at(j));
                }
                rangeIndex++;
            } else {
                // Scalar value
                allValues.append(args[i]);
            }
        }

        // Now compute the function
        if (name.equalsIgnoreCase("SUM")) {
            *result = sum(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("AVERAGE")) {
            *result = average(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("COUNT")) {
            *result = count(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("MIN")) {
            *result = min(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("MAX")) {
            *result = max(allValues);
            return FUNCTION_DEFINED;
        }
    }

    // No ranges or not a range function - just compute with scalar args
    if (name.equalsIgnoreCase("SUM")) {
        double total = 0.0;
        for (int i = 0; i < numberOfArgs; i++) {
            total += args[i];
        }
        *result = total;
        return FUNCTION_DEFINED;
    }

    if (name.equalsIgnoreCase("AVERAGE")) {
        if (numberOfArgs == 0) {
            *result = 0.0;
            return FUNCTION_DEFINED;
        }
        double total = 0.0;
        for (int i = 0; i < numberOfArgs; i++) {
            total += args[i];
        }
        *result = total / numberOfArgs;
        return FUNCTION_DEFINED;
    }

    if (name.equalsIgnoreCase("COUNT")) {
        *result = (double)numberOfArgs;
        return FUNCTION_DEFINED;
    }

    if (name.equalsIgnoreCase("MIN")) {
        if (numberOfArgs == 0) {
            *result = 0.0;
            return FUNCTION_DEFINED;
        }
        double minVal = args[0];
        for (int i = 1; i < numberOfArgs; i++) {
            if (args[i] < minVal) minVal = args[i];
        }
        *result = minVal;
        return FUNCTION_DEFINED;
    }

    if (name.equalsIgnoreCase("MAX")) {
        if (numberOfArgs == 0) {
            *result = 0.0;
            return FUNCTION_DEFINED;
        }
        double maxVal = args[0];
        for (int i = 1; i < numberOfArgs; i++) {
            if (args[i] > maxVal) maxVal = args[i];
        }
        *result = maxVal;
        return FUNCTION_DEFINED;
    }

    return FUNCTION_UNDEFINED;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::FunctionEvaluateWithRanges
//
// Evaluate function with both scalar and range arguments
//-------------------------------------------------------------------------
CxExpressionFunctionDatabase::returnCode
CxSheetFunctionDatabase::FunctionEvaluateWithRanges(
    CxString name,
    int numberOfArgs,
    double *scalarArgs,
    CxString *rangeArgs,
    double *result)
{
    if (!isRangeFunction(name)) {
        // Not a range function - just use scalar values
        return FunctionEvaluate(name, numberOfArgs, scalarArgs, result);
    }

    // Collect all values from both ranges and scalars
    CxSList<double> allValues;

    for (int i = 0; i < numberOfArgs; i++) {
        if (rangeArgs[i].length() > 0) {
            // This is a range argument - expand it
            CxSList<double> rangeValues = expandRange(rangeArgs[i]);
            for (int j = 0; j < rangeValues.entries(); j++) {
                allValues.append(rangeValues.at(j));
            }
        } else {
            // Scalar value
            allValues.append(scalarArgs[i]);
        }
    }

    // Compute the function
    if (name.equalsIgnoreCase("SUM")) {
        *result = sum(allValues);
        return FUNCTION_DEFINED;
    }
    if (name.equalsIgnoreCase("AVERAGE")) {
        *result = average(allValues);
        return FUNCTION_DEFINED;
    }
    if (name.equalsIgnoreCase("COUNT")) {
        *result = count(allValues);
        return FUNCTION_DEFINED;
    }
    if (name.equalsIgnoreCase("MIN")) {
        *result = min(allValues);
        return FUNCTION_DEFINED;
    }
    if (name.equalsIgnoreCase("MAX")) {
        *result = max(allValues);
        return FUNCTION_DEFINED;
    }

    return FUNCTION_UNDEFINED;
}
