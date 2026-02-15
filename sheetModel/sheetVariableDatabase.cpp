//-------------------------------------------------------------------------------------------------
//
//  sheetVariableDatabase.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetVariableDatabase Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sheetVariableDatabase.h"
#include "sheetModel.h"


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::CxSheetVariableDatabase
//
// Default constructor
//-------------------------------------------------------------------------
CxSheetVariableDatabase::CxSheetVariableDatabase(void)
: sheetModel(NULL)
, circularReferenceDetected(0)
{
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::CxSheetVariableDatabase
//
// Constructor with model pointer
//-------------------------------------------------------------------------
CxSheetVariableDatabase::CxSheetVariableDatabase(CxSheetModel* model)
: sheetModel(model)
, circularReferenceDetected(0)
{
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::setModel
//
// Set the sheet model to use for cell lookups
//-------------------------------------------------------------------------
void
CxSheetVariableDatabase::setModel(CxSheetModel* model)
{
    sheetModel = model;
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::getModel
//
// Get the current sheet model
//-------------------------------------------------------------------------
CxSheetModel*
CxSheetVariableDatabase::getModel(void)
{
    return sheetModel;
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::VariableDefined
//
// Returns VARIABLE_DEFINED if name is a valid cell coordinate
//-------------------------------------------------------------------------
CxExpressionVariableDatabase::returnCode
CxSheetVariableDatabase::VariableDefined(CxString name)
{
    // Try to parse the name as a cell coordinate
    CxSheetCellCoordinate coord;

    if (coord.parseAddress(name)) {
        return VARIABLE_DEFINED;
    }

    return VARIABLE_UNDEFINED;
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::VariableEvaluate
//
// Parses name as a cell coordinate, looks up the cell, returns its value.
//
// NOTE: For formula cells, returns the PRE-COMPUTED evaluatedValue.
// The dependency graph ensures cells are evaluated in the correct order.
//
// CIRCULAR REFERENCE DETECTION:
// While the dependency graph handles cycle detection for ordering,
// we still check the evaluation stack at runtime to properly handle
// circular references. If a formula references a cell that is currently
// being evaluated (on the stack), we return 0 to break the cycle.
//-------------------------------------------------------------------------
CxExpressionVariableDatabase::returnCode
CxSheetVariableDatabase::VariableEvaluate(CxString name, double* result)
{
    // Must have a model to look up cells
    if (sheetModel == NULL) {
        *result = 0.0;
        return VARIABLE_UNDEFINED;
    }

    // Try to parse the name as a cell coordinate
    CxSheetCellCoordinate coord;

    if (!coord.parseAddress(name)) {
        *result = 0.0;
        return VARIABLE_UNDEFINED;
    }

    //-------------------------------------------------------------------------
    // CIRCULAR REFERENCE CHECK:
    // If this cell is already on the evaluation stack, we have a circular
    // reference. Set the flag and return 0 to break the cycle.
    //
    // Examples that would hit this:
    // - A1 = A:1 + 1 (self-reference)
    // - A1 = B:1 + 1, B1 = A:1 + 1 (mutual reference)
    // - A1 = B:1, B1 = C:1, C1 = A:1 (chain reference)
    //
    // The circularReferenceDetected flag is checked by recalculateCell()
    // to set the cell's value to 0 (the expected behavior for circular refs).
    //-------------------------------------------------------------------------
    if (isOnEvaluationStack(coord)) {
        circularReferenceDetected = 1;
        *result = 0.0;
        return VARIABLE_DEFINED;  // Return 0 for circular reference
    }

    // Look up the cell in the model - use pointer to avoid copy constructor
    // which would re-parse the formula without the variable database
    CxSheetCell* cell = sheetModel->getCellPtr(coord);

    // NULL means empty cell - evaluate to 0
    if (cell == NULL) {
        *result = 0.0;
        return VARIABLE_DEFINED;
    }

    // Get the value based on cell type
    switch (cell->getType()) {

        case CxSheetCell::DOUBLE:
            *result = cell->getDouble().value;
            return VARIABLE_DEFINED;

        case CxSheetCell::FORMULA:
            //-------------------------------------------------------------
            // For formula cells, we need to do NESTED EVALUATION to properly
            // detect circular references. If we just returned the cached
            // value, we'd miss mutual references like A1=B1+1, B1=A1+1.
            //
            // How it works:
            // 1. Push this cell onto the evaluation stack
            // 2. Evaluate the formula
            // 3. If during evaluation we hit a cell already on the stack,
            //    that's a circular reference
            // 4. Pop and return the result
            //
            // Example for mutual reference (A1=B:1+1, B1=A:1+1):
            // - Evaluating A1: stack=[A1]
            // - A1 refs B1, evaluate B1: stack=[A1,B1]
            // - B1 refs A1, but A1 is on stack! → circular reference
            //-------------------------------------------------------------
            if (cell->formula != NULL) {
                // Push this cell onto the evaluation stack
                pushEvaluationStack(coord);

                // Set our database for nested cell references
                cell->formula->setVariableDatabase(this);

                double formulaResult = 0.0;
                CxExpression::expressionStatus status = cell->formula->Evaluate(&formulaResult);

                // Pop from the evaluation stack
                popEvaluationStack();

                if (status == CxExpression::EVALUATION_SUCCESS) {
                    *result = formulaResult;
                    return VARIABLE_DEFINED;
                } else {
                    // Evaluation failed - might be due to circular reference
                    *result = 0.0;
                    return VARIABLE_DEFINED;
                }
            } else {
                // No formula object, return cached value
                *result = cell->getEvaluatedValue().value;
                return VARIABLE_DEFINED;
            }

        case CxSheetCell::TEXT:
            // Text cells evaluate to 0 (like Excel)
            *result = 0.0;
            return VARIABLE_DEFINED;

        case CxSheetCell::EMPTY:
        default:
            // Empty cells evaluate to 0
            *result = 0.0;
            return VARIABLE_DEFINED;
    }
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::clearEvaluationStack
//
// Clears the evaluation stack and resets the circular reference flag.
// Call this before starting a new recalculation batch.
//-------------------------------------------------------------------------
void
CxSheetVariableDatabase::clearEvaluationStack(void)
{
    evaluationStack.clear();
    circularReferenceDetected = 0;
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::hasCircularReference
//
// Returns 1 if a circular reference was detected during the most recent
// evaluation. Call this after Evaluate() completes to check if the cell
// should have its value set to 0/error.
//-------------------------------------------------------------------------
int
CxSheetVariableDatabase::hasCircularReference(void)
{
    return circularReferenceDetected;
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::pushEvaluationStack
//
// Push a coordinate onto the evaluation stack
//-------------------------------------------------------------------------
void
CxSheetVariableDatabase::pushEvaluationStack(CxSheetCellCoordinate coord)
{
    evaluationStack.append(coord);
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::popEvaluationStack
//
// Pop the top coordinate from the evaluation stack
//-------------------------------------------------------------------------
void
CxSheetVariableDatabase::popEvaluationStack(void)
{
    if (evaluationStack.entries() > 0) {
        evaluationStack.removeAt(evaluationStack.entries() - 1);
    }
}


//-------------------------------------------------------------------------
// CxSheetVariableDatabase::isOnEvaluationStack
//
// Returns 1 if the coordinate is already on the evaluation stack
// (indicates a circular reference)
//-------------------------------------------------------------------------
int
CxSheetVariableDatabase::isOnEvaluationStack(CxSheetCellCoordinate coord)
{
    for (int i = 0; i < (int)evaluationStack.entries(); i++) {
        CxSheetCellCoordinate stackCoord = evaluationStack.at(i);
        if (stackCoord == coord) {
            return 1;
        }
    }
    return 0;
}
