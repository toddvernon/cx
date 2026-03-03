//-------------------------------------------------------------------------------------------------
//
//  sheetFunctionDatabase.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetFunctionDatabase Class
//
//  A function database for spreadsheet formulas that supports range arguments.
//  Functions like SUM, AVERAGE, COUNT, MIN, MAX can accept cell ranges (e.g., A1:A10).
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxSheetFunctionDatabase_
#define _CxSheetFunctionDatabase_

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/expression/funcdb.h>

// Forward declarations
class CxSheetModel;
class CxSheetCellRange;


//-------------------------------------------------------------------------------------------------
//
// CxSheetFunctionDatabase
//
// A function database that understands spreadsheet cell ranges.
// It can expand ranges like "A1:A10" to iterate over cells and compute
// aggregate functions like SUM, AVERAGE, COUNT, MIN, MAX.
//
// This class works in conjunction with CxSheetModel to resolve cell values.
//
//-------------------------------------------------------------------------------------------------

class CxSheetFunctionDatabase : public CxExpressionFunctionDatabase
{
  public:

    CxSheetFunctionDatabase(void);
    // Default constructor - model must be set before use

    CxSheetFunctionDatabase(CxSheetModel* model);
    // Constructor with model reference

    void setModel(CxSheetModel* model);
    // Set the sheet model used for resolving cell values

    virtual returnCode FunctionDefined(CxString name);
    // Check if a function name is defined (SUM, AVERAGE, COUNT, MIN, MAX)

    virtual returnCode FunctionEvaluate(
        CxString name,
        int numberOfArgs,
        double *args,
        double *result);
    // Evaluate function with scalar arguments only

    virtual returnCode FunctionEvaluateWithRanges(
        CxString name,
        int numberOfArgs,
        double *scalarArgs,
        CxString *rangeArgs,
        double *result);
    // Evaluate function with both scalar and range arguments

    void setRangeList(CxSList<CxString> rangeList);
    // Set the list of ranges from the current expression being evaluated
    // This allows the database to identify which arguments are ranges

  private:

    CxSheetModel* sheetModel;

    CxSList<CxString> currentRangeList;
    // List of ranges in the current expression (from GetRangeList())

    // Helper to expand a range and collect numeric values
    CxSList<double> expandRange(CxString rangeStr);

    // Aggregate functions
    double sum(CxSList<double>& values);
    double average(CxSList<double>& values);
    double count(CxSList<double>& values);
    double min(CxSList<double>& values);
    double max(CxSList<double>& values);

    // Check if this is a range-aware function
    int isRangeFunction(CxString name);
};


#endif
