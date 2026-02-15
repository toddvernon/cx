//-------------------------------------------------------------------------------------------------
//
//  sheetVariableDatabase.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetVariableDatabase Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>

//-------------------------------------------------------------------------------------------------
// cx library includes
//-------------------------------------------------------------------------------------------------
#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/expression/vardb.h>

//-------------------------------------------------------------------------------------------------
// sheetModel includes
//-------------------------------------------------------------------------------------------------
#include <cx/sheetModel/sheetCellCoordinate.h>

#ifndef _CxSheetVariableDatabase_
#define _CxSheetVariableDatabase_

// Forward declaration to avoid circular dependency
class CxSheetModel;


//-------------------------------------------------------------------------------------------------
//
// CxSheetVariableDatabase
//
// A variable database implementation for CxExpression that resolves cell references
// (like "A:1", "$B:$2") to their evaluated values from the spreadsheet model.
//
// Variable names are expected to be cell coordinates in spreadsheet notation.
// When a formula contains a reference like "A:1 + B:2", this class looks up
// those cells and returns their evaluated values.
//
// NOTE: Circular reference detection is handled by CxSheetDependencyGraph during
// topological sort, not at evaluation time. If a formula creates a cycle, the
// dependency graph's topological sort will detect it and the affected cells
// will evaluate to 0.
//
//-------------------------------------------------------------------------------------------------

class CxSheetVariableDatabase : public CxExpressionVariableDatabase
{
  public:

    CxSheetVariableDatabase(void);
    // default constructor - creates database with no model (must call setModel)

    CxSheetVariableDatabase(CxSheetModel* model);
    // constructor with model pointer

    void setModel(CxSheetModel* model);
    // set the sheet model to use for cell lookups

    CxSheetModel* getModel(void);
    // get the current sheet model

    virtual returnCode VariableDefined(CxString name);
    // returns VARIABLE_DEFINED if name is a valid cell coordinate
    // returns VARIABLE_UNDEFINED otherwise

    virtual returnCode VariableEvaluate(CxString name, double* result);
    // parses name as a cell coordinate, looks up the cell in the model,
    // and returns its evaluated value
    // returns VARIABLE_DEFINED on success, VARIABLE_UNDEFINED on failure

    //---------------------------------------------------------------------------------------------
    // CIRCULAR REFERENCE TRACKING (used by sheetModel during recalculation)
    //
    // While the dependency graph handles cycle detection for ordering purposes,
    // we still need runtime tracking to properly return 0 when a formula references
    // a cell that is currently being evaluated (either directly or through a chain).
    //
    // This is a simple stack-based approach:
    // - Before evaluating a formula, push its coordinate onto the stack
    // - When VariableEvaluate is called, check if the requested cell is on the stack
    // - If so, it's a circular reference - return 0
    // - After evaluation, pop the coordinate from the stack
    //---------------------------------------------------------------------------------------------

    void clearEvaluationStack(void);
    // Clear the evaluation stack (call before starting a new recalculation batch)

    void pushEvaluationStack(CxSheetCellCoordinate coord);
    // Push a cell coordinate onto the evaluation stack (call before evaluating its formula)

    void popEvaluationStack(void);
    // Pop the top coordinate from the evaluation stack (call after evaluation completes)

    int hasCircularReference(void);
    // Returns 1 if a circular reference was detected during the most recent evaluation.
    // Call this after Evaluate() to check if the cell should be set to error/0.

  private:

    CxSheetModel* sheetModel;
    // pointer to the sheet model (not owned)

    CxSList<CxSheetCellCoordinate> evaluationStack;
    // stack of cells currently being evaluated (for circular reference detection)

    int circularReferenceDetected;
    // flag set to 1 when a circular reference is detected during evaluation

    int isOnEvaluationStack(CxSheetCellCoordinate coord);
    // Returns 1 if the coordinate is on the evaluation stack (circular reference)
};


#endif
