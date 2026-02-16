//-------------------------------------------------------------------------------------------------
//
//  sheetModel.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetModel Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sheetModel.h"
#include "sheetVariableDatabase.h"
#include <cx/expression/expression.h>
#include <cx/base/file.h>
#include <cx/json/json_factory.h>
#include <cx/json/json_member.h>
#include <cx/json/json_string.h>
#include <cx/json/json_number.h>
#include <cx/json/json_boolean.h>
#include <cx/json/json_null.h>
#include <cx/json/json_object.h>
#include <cx/json/json_array.h>


//-------------------------------------------------------------------------
// CxSheetModel::CxSheetModel
//
// Constructor
//-------------------------------------------------------------------------
CxSheetModel::CxSheetModel(void)
: readOnly(0)
, touched(0)
, maxRowUsed(0)
, maxColUsed(0)
, variableDatabase(NULL)
, loadingInProgress(0)
{
    variableDatabase = new CxSheetVariableDatabase(this);
}


//-------------------------------------------------------------------------
// CxSheetModel::CxSheetModel
//
// Copy constructor
//-------------------------------------------------------------------------
CxSheetModel::CxSheetModel(const CxSheetModel& other)
: currentCellPosition(other.currentCellPosition)
, sheetPath(other.sheetPath)
, readOnly(other.readOnly)
, touched(other.touched)
, maxRowUsed(other.maxRowUsed)
, maxColUsed(other.maxColUsed)
, variableDatabase(NULL)
, loadingInProgress(0)
{
    // Create our own variable database pointing to this model
    variableDatabase = new CxSheetVariableDatabase(this);

    // Copy cells using iterator (we'll rebuild dependencies after)
    loadingInProgress = 1;  // Prevent recalculation during cell insert

    CxHashmapIterator<CxSheetCellCoordinate, CxSheetCell> iter(
        (CxHashmap<CxSheetCellCoordinate, CxSheetCell>*)&other.cellHashMap);

    while (iter.next()) {
        const CxSheetCellCoordinate* key = iter.getKey();
        CxSheetCell* entry = iter.getEntry();
        if (key != NULL && entry != NULL) {
            // Use setCell to properly set up dependencies
            setCell(*key, *entry);
        }
    }

    loadingInProgress = 0;

    // Recalculate all formulas now that everything is loaded
    recalculateAll();
}


//-------------------------------------------------------------------------
// CxSheetModel::~CxSheetModel
//
// Destructor
//-------------------------------------------------------------------------
CxSheetModel::~CxSheetModel(void)
{
    // Clean up the variable database
    if (variableDatabase != NULL) {
        delete variableDatabase;
        variableDatabase = NULL;
    }

    // CxHashmap destructor will clean up the cells
}


//-------------------------------------------------------------------------
// CxSheetModel::operator=
//
// Assignment operator
//-------------------------------------------------------------------------
CxSheetModel&
CxSheetModel::operator=(const CxSheetModel& other)
{
    if (this != &other) {
        // Reset and copy
        reset();

        currentCellPosition = other.currentCellPosition;
        sheetPath = other.sheetPath;
        readOnly = other.readOnly;
        touched = other.touched;
        maxRowUsed = other.maxRowUsed;
        maxColUsed = other.maxColUsed;

        // Variable database already exists and points to this model
        // (created in constructor, not changed here)

        // Copy cells using iterator
        CxHashmapIterator<CxSheetCellCoordinate, CxSheetCell> iter(
            (CxHashmap<CxSheetCellCoordinate, CxSheetCell>*)&other.cellHashMap);

        while (iter.next()) {
            const CxSheetCellCoordinate* key = iter.getKey();
            CxSheetCell* entry = iter.getEntry();
            if (key != NULL && entry != NULL) {
                cellHashMap.insert(*key, *entry);
            }
        }
    }
    return *this;
}


//-------------------------------------------------------------------------
// CxSheetModel::reset
//
// Reset the class to initial condition and empty sheet
//-------------------------------------------------------------------------
void
CxSheetModel::reset(void)
{
    // Create a new empty hashmap by reconstructing
    // Note: We need to be careful here as CxHashmap doesn't have a clear() method
    // The destructor will be called when we go out of scope

    currentCellPosition = CxSheetCellCoordinate(0, 0);
    sheetPath = CxString();
    readOnly = 0;
    touched = 0;
    maxRowUsed = 0;
    maxColUsed = 0;

    // Clear the dependency graph since all cells are being removed
    dependencyGraph.clear();

    // Note: To truly clear the hashmap, we would need to iterate and remove
    // or reconstruct it. For now, setting extents to 0 effectively marks it empty.
}


//-------------------------------------------------------------------------
// CxSheetModel::pasteFromCutBuffer
//
// Paste cell data into selected cell
//-------------------------------------------------------------------------
void
CxSheetModel::pasteFromCutBuffer(CxSheetCell cell)
{
    setCell(currentCellPosition, cell);
}


//-------------------------------------------------------------------------
// CxSheetModel::cursorRightRequest
//
// Request to move the cursor right in the sheet
//-------------------------------------------------------------------------
CxSheetModel::ACTION
CxSheetModel::cursorRightRequest(void)
{
    currentCellPosition.setCol(currentCellPosition.getCol() + 1);
    return CURSOR_RIGHT;
}


//-------------------------------------------------------------------------
// CxSheetModel::cursorLeftRequest
//
// Request to move the cursor left in the sheet
//-------------------------------------------------------------------------
CxSheetModel::ACTION
CxSheetModel::cursorLeftRequest(void)
{
    if (currentCellPosition.getCol() > 0) {
        currentCellPosition.setCol(currentCellPosition.getCol() - 1);
        return CURSOR_LEFT;
    }
    return NONE;
}


//-------------------------------------------------------------------------
// CxSheetModel::cursorUpRequest
//
// Request to move the cursor up in the sheet
//-------------------------------------------------------------------------
CxSheetModel::ACTION
CxSheetModel::cursorUpRequest(void)
{
    if (currentCellPosition.getRow() > 0) {
        currentCellPosition.setRow(currentCellPosition.getRow() - 1);
        return CURSOR_UP;
    }
    return NONE;
}


//-------------------------------------------------------------------------
// CxSheetModel::cursorDownRequest
//
// Request to move the cursor down in the sheet
//-------------------------------------------------------------------------
CxSheetModel::ACTION
CxSheetModel::cursorDownRequest(void)
{
    currentCellPosition.setRow(currentCellPosition.getRow() + 1);
    return CURSOR_DOWN;
}


//-------------------------------------------------------------------------
// CxSheetModel::jumpToCell
//
// Jump cursor directly to specified cell
//-------------------------------------------------------------------------
CxSheetModel::ACTION
CxSheetModel::jumpToCell(CxSheetCellCoordinate coord)
{
    currentCellPosition = coord;
    return JUMP_DIRECT;
}


//-------------------------------------------------------------------------
// CxSheetModel::getCurrentPosition
//
// Get the current cursor position
//-------------------------------------------------------------------------
CxSheetCellCoordinate
CxSheetModel::getCurrentPosition(void)
{
    return currentCellPosition;
}


//-------------------------------------------------------------------------
// CxSheetModel::getCell
//
// Gets a copy of the CxSheetCell at the cell coordinate
//-------------------------------------------------------------------------
CxSheetCell
CxSheetModel::getCell(CxSheetCellCoordinate coord)
{
    const CxSheetCell* cell = cellHashMap.find(coord);

    if (cell != NULL) {
        return *cell;
    }

    // Return empty cell if not found
    return CxSheetCell();
}


//-------------------------------------------------------------------------
// CxSheetModel::getCellPtr
//
// Gets a pointer to the CxSheetCell at the cell coordinate
// Returns NULL if the cell doesn't exist
//-------------------------------------------------------------------------
CxSheetCell*
CxSheetModel::getCellPtr(CxSheetCellCoordinate coord)
{
    return (CxSheetCell*)cellHashMap.find(coord);
}


//-------------------------------------------------------------------------
// CxSheetModel::setCell
//
// Copies the passed in cell value into the cell at the referenced coordinate.
//
// This method also maintains the dependency graph:
// 1. Clears any old dependencies for this cell (if it had a formula)
// 2. Inserts the new cell
// 3. If the new cell is a formula, extracts its references and registers dependencies
// 4. Triggers recalculation of cells that depend on this cell
//-------------------------------------------------------------------------
void
CxSheetModel::setCell(CxSheetCellCoordinate coord, CxSheetCell cell)
{
    //-------------------------------------------------------------------------
    // STEP 1: Clear old dependencies
    //
    // If this cell previously had a formula, we need to remove it from the
    // dependency graph. The new cell (whether value or new formula) will
    // have different dependencies.
    //-------------------------------------------------------------------------
    clearDependencies(coord);

    //-------------------------------------------------------------------------
    // STEP 2: Insert the cell into the hashmap
    //
    // For formula cells, we need to re-parse with our variableDatabase
    // so that cell references like "A:1" can be recognized.
    //-------------------------------------------------------------------------
    if (cell.cellType == CxSheetCell::FORMULA && cell.formula != NULL) {
        // Get the formula text before inserting
        CxString formulaText = cell.text;

        // Clear the formula from the cell we're about to insert
        // (it was parsed without the variableDatabase)
        delete cell.formula;
        cell.formula = NULL;

        // Insert the cell first
        cellHashMap.insert(coord, cell);

        // Now get a pointer to the inserted cell and re-create/parse the formula
        // with the variableDatabase
        CxSheetCell* insertedCell = (CxSheetCell*)cellHashMap.find(coord);
        if (insertedCell != NULL) {
            insertedCell->formula = new CxExpression(formulaText, variableDatabase, NULL);
            insertedCell->formula->Parse();

            // STEP 3: Register new dependencies for this formula
            updateDependencies(coord, insertedCell);
        }
    } else {
        cellHashMap.insert(coord, cell);
    }

    // Update extents
    if (coord.getRow() > maxRowUsed) {
        maxRowUsed = coord.getRow();
    }
    if (coord.getCol() > maxColUsed) {
        maxColUsed = coord.getCol();
    }

    touched = 1;

    //-------------------------------------------------------------------------
    // STEP 4: Trigger recalculation
    //
    // Track which cell changed so recalculate() knows where to start.
    //-------------------------------------------------------------------------
    lastChangedCell = coord;
    recalculate();
}


//-------------------------------------------------------------------------
// CxSheetModel::loadSheet
//
// Load the sheet from disk. Sheet is stored in json format
// Returns 1 on success, 0 on failure
//-------------------------------------------------------------------------
int
CxSheetModel::loadSheet(CxString filepath)
{
    // Open and read the file
    CxFile inFile;
    if (!inFile.open(filepath, "r")) {
        return 0;
    }

    // Read entire file contents
    CxString buffer;
    CxString line = inFile.getUntil('\n');
    while (line.length()) {
        buffer += line;
        line = inFile.getUntil('\n');
    }
    inFile.close();

    if (buffer.length() == 0) {
        return 0;
    }

    // Parse JSON
    CxJSONBase *parsed = CxJSONFactory::parse(buffer);
    if (parsed == NULL || parsed->type() != CxJSONBase::OBJECT) {
        if (parsed != NULL) {
            delete parsed;
        }
        return 0;
    }

    CxJSONObject *root = (CxJSONObject *)parsed;

    // Reset the model before loading
    reset();

    //-------------------------------------------------------------------------
    // Set loading flag to defer recalculation.
    // During loading, we don't want to recalculate after each cell insert:
    // 1. It's inefficient (O(n^2) instead of O(n))
    // 2. Formulas might reference cells not yet loaded
    //-------------------------------------------------------------------------
    loadingInProgress = 1;

    // Load current position if present (stored as cell address like "A:1")
    CxJSONMember *posMember = root->find("currentPosition");
    if (posMember != NULL && posMember->object()->type() == CxJSONBase::STRING) {
        CxString posAddr = ((CxJSONString *)posMember->object())->get();
        currentCellPosition.parseAddress(posAddr);
    }

    // Load cells
    CxJSONMember *cellsMember = root->find("cells");
    if (cellsMember != NULL && cellsMember->object()->type() == CxJSONBase::ARRAY) {
        CxJSONArray *cellsArray = (CxJSONArray *)cellsMember->object();

        for (int i = 0; i < cellsArray->entries(); i++) {
            CxJSONBase *cellBase = cellsArray->at(i);

            if (cellBase->type() != CxJSONBase::OBJECT) {
                continue;
            }

            CxJSONObject *cellObj = (CxJSONObject *)cellBase;

            // Get cell address (e.g., "A:1", "B:2")
            CxJSONMember *cellMember = cellObj->find("cell");
            CxJSONMember *typeMember = cellObj->find("type");

            if (cellMember == NULL || typeMember == NULL) {
                continue;
            }

            if (cellMember->object()->type() != CxJSONBase::STRING ||
                typeMember->object()->type() != CxJSONBase::STRING) {
                continue;
            }

            CxString cellAddr = ((CxJSONString *)cellMember->object())->get();
            CxString type = ((CxJSONString *)typeMember->object())->get();

            CxSheetCellCoordinate coord;
            if (!coord.parseAddress(cellAddr)) {
                continue;  // Skip invalid addresses
            }

            CxSheetCell cell;

            if (type == "text") {
                CxJSONMember *textMember = cellObj->find("text");
                if (textMember != NULL && textMember->object()->type() == CxJSONBase::STRING) {
                    cell.setText(((CxJSONString *)textMember->object())->get());
                }
            }
            else if (type == "double") {
                CxJSONMember *valueMember = cellObj->find("value");
                if (valueMember != NULL && valueMember->object()->type() == CxJSONBase::NUMBER) {
                    cell.setDouble(CxDouble(((CxJSONNumber *)valueMember->object())->get()));
                }

                // Load display settings
                CxJSONMember *decMember = cellObj->find("decimalPlaces");
                if (decMember != NULL && decMember->object()->type() == CxJSONBase::NUMBER) {
                    cell.displayDecimalPlaces = (int)((CxJSONNumber *)decMember->object())->get();
                }

                CxJSONMember *currMember = cellObj->find("currency");
                if (currMember != NULL && currMember->object()->type() == CxJSONBase::BOOLEAN) {
                    cell.displayCurrency = ((CxJSONBoolean *)currMember->object())->get();
                }

                CxJSONMember *commaMember = cellObj->find("commas");
                if (commaMember != NULL && commaMember->object()->type() == CxJSONBase::BOOLEAN) {
                    cell.displayCommas = ((CxJSONBoolean *)commaMember->object())->get();
                }
            }
            else if (type == "formula") {
                CxJSONMember *formulaMember = cellObj->find("formula");
                if (formulaMember != NULL && formulaMember->object()->type() == CxJSONBase::STRING) {
                    CxString formulaText = ((CxJSONString *)formulaMember->object())->get();
                    // Strip leading "=" if present (added for readability in saved files)
                    if (formulaText.length() > 0 && formulaText.data()[0] == '=') {
                        formulaText = CxString(formulaText.data() + 1);
                    }
                    cell.setFormula(formulaText);
                }

                // Load display settings
                CxJSONMember *decMember = cellObj->find("decimalPlaces");
                if (decMember != NULL && decMember->object()->type() == CxJSONBase::NUMBER) {
                    cell.displayDecimalPlaces = (int)((CxJSONNumber *)decMember->object())->get();
                }

                CxJSONMember *currMember = cellObj->find("currency");
                if (currMember != NULL && currMember->object()->type() == CxJSONBase::BOOLEAN) {
                    cell.displayCurrency = ((CxJSONBoolean *)currMember->object())->get();
                }

                CxJSONMember *commaMember = cellObj->find("commas");
                if (commaMember != NULL && commaMember->object()->type() == CxJSONBase::BOOLEAN) {
                    cell.displayCommas = ((CxJSONBoolean *)commaMember->object())->get();
                }
            }

            // Load formatting attributes (common to all cell types)
            CxJSONMember *boldMember = cellObj->find("bold");
            if (boldMember != NULL && boldMember->object()->type() == CxJSONBase::BOOLEAN) {
                cell.bold = ((CxJSONBoolean *)boldMember->object())->get() ? 1 : 0;
            }

            CxJSONMember *fgColorMember = cellObj->find("fgColor");
            if (fgColorMember != NULL && fgColorMember->object()->type() == CxJSONBase::STRING) {
                cell.fgColor = ((CxJSONString *)fgColorMember->object())->get();
            }

            CxJSONMember *bgColorMember = cellObj->find("bgColor");
            if (bgColorMember != NULL && bgColorMember->object()->type() == CxJSONBase::STRING) {
                cell.bgColor = ((CxJSONString *)bgColorMember->object())->get();
            }

            // Insert cell into model (setCell will update maxRowUsed/maxColUsed)
            setCell(coord, cell);
        }
    }

    // Clean up
    delete root;

    //-------------------------------------------------------------------------
    // Loading complete - now recalculate all formulas.
    // This is done once at the end rather than after each cell insert.
    //-------------------------------------------------------------------------
    loadingInProgress = 0;
    recalculateAll();

    sheetPath = filepath;
    touched = 0;
    return 1;
}


//-------------------------------------------------------------------------
// CxSheetModel::saveSheet
//
// Save the sheet to a file in json format
// Returns 1 on success, 0 on failure
//
// JSON format:
// {
//   "version": 1,
//   "currentPosition": "A:1",
//   "cells": [
//     {"cell": "A:1", "type": "double", "value": 42.5,
//      "decimalPlaces": 2, "currency": false, "commas": false},
//     {"cell": "B:1", "type": "text", "text": "Hello",
//      "bold": true, "fgColor": "RGB:255,0,0", "bgColor": "ANSI:WHITE"},
//     {"cell": "A:2", "type": "formula", "formula": "=A:1+10"}
//   ]
// }
//
// Formatting attributes (optional, compatible with .cmrc color format):
//   - bold: true/false
//   - fgColor: "ANSI:COLOR_NAME", "RGB:r,g,b", or "XTERM256:COLOR_NAME"
//   - bgColor: "ANSI:COLOR_NAME", "RGB:r,g,b", or "XTERM256:COLOR_NAME"
//-------------------------------------------------------------------------
int
CxSheetModel::saveSheet(CxString filepath)
{
    // Create the root JSON object
    CxJSONObject *root = new CxJSONObject();

    // Add version
    root->append(new CxJSONMember("version", new CxJSONNumber(1)));

    // Add current position as cell address (e.g., "A:1")
    root->append(new CxJSONMember("currentPosition",
        new CxJSONString(currentCellPosition.toAddress())));

    // Add cells array
    CxJSONArray *cellsArray = new CxJSONArray();

    // Iterate through all cells and add to array
    CxHashmapIterator<CxSheetCellCoordinate, CxSheetCell> iter(&cellHashMap);

    while (iter.next()) {
        const CxSheetCellCoordinate* key = iter.getKey();
        CxSheetCell* cell = iter.getEntry();

        if (key == NULL || cell == NULL) {
            continue;
        }

        // Skip empty cells
        if (cell->getType() == CxSheetCell::EMPTY) {
            continue;
        }

        CxJSONObject *cellObj = new CxJSONObject();

        // Add cell address (e.g., "A:1", "B:2")
        cellObj->append(new CxJSONMember("cell", new CxJSONString(key->toAddress())));

        // Add type-specific data
        switch (cell->getType()) {

            case CxSheetCell::TEXT:
                cellObj->append(new CxJSONMember("type", new CxJSONString("text")));
                cellObj->append(new CxJSONMember("text", new CxJSONString(cell->getText())));
                break;

            case CxSheetCell::DOUBLE:
                cellObj->append(new CxJSONMember("type", new CxJSONString("double")));
                cellObj->append(new CxJSONMember("value", new CxJSONNumber(cell->getDouble().value)));
                cellObj->append(new CxJSONMember("decimalPlaces", new CxJSONNumber(cell->displayDecimalPlaces)));
                cellObj->append(new CxJSONMember("currency", new CxJSONBoolean(cell->displayCurrency)));
                cellObj->append(new CxJSONMember("commas", new CxJSONBoolean(cell->displayCommas)));
                break;

            case CxSheetCell::FORMULA:
                cellObj->append(new CxJSONMember("type", new CxJSONString("formula")));
                // Prepend "=" for readability (like Excel)
                cellObj->append(new CxJSONMember("formula",
                    new CxJSONString(CxString("=") + cell->getFormulaText())));
                cellObj->append(new CxJSONMember("decimalPlaces", new CxJSONNumber(cell->displayDecimalPlaces)));
                cellObj->append(new CxJSONMember("currency", new CxJSONBoolean(cell->displayCurrency)));
                cellObj->append(new CxJSONMember("commas", new CxJSONBoolean(cell->displayCommas)));
                break;

            default:
                // Skip unknown types
                delete cellObj;
                continue;
        }

        // Add formatting attributes (only if set to non-default values)
        if (cell->bold) {
            cellObj->append(new CxJSONMember("bold", new CxJSONBoolean(1)));
        }
        if (cell->fgColor.length() > 0) {
            cellObj->append(new CxJSONMember("fgColor", new CxJSONString(cell->fgColor)));
        }
        if (cell->bgColor.length() > 0) {
            cellObj->append(new CxJSONMember("bgColor", new CxJSONString(cell->bgColor)));
        }

        cellsArray->append(cellObj);
    }

    root->append(new CxJSONMember("cells", cellsArray));

    // Write to file
    CxFile outFile;
    if (!outFile.open(filepath, "w")) {
        delete root;
        return 0;
    }

    // Serialize to string using portable toJsonString() method
    CxString jsonStr = root->toJsonString();
    outFile.printf("%s\n", jsonStr.data());
    outFile.close();

    // Clean up
    delete root;

    sheetPath = filepath;
    touched = 0;
    return 1;
}


//-------------------------------------------------------------------------
// CxSheetModel::numberOfRows
//
// Returns the number of rows that contain data (highest row + 1)
//-------------------------------------------------------------------------
unsigned long
CxSheetModel::numberOfRows(void)
{
    return maxRowUsed + 1;
}


//-------------------------------------------------------------------------
// CxSheetModel::numberOfColumns
//
// Returns the number of columns that contain data (highest column + 1)
//-------------------------------------------------------------------------
unsigned long
CxSheetModel::numberOfColumns(void)
{
    return maxColUsed + 1;
}


//-------------------------------------------------------------------------
// CxSheetModel::getSheetExtents
//
// Returns the coordinate of the most right/lower cell that contains data
//-------------------------------------------------------------------------
CxSheetCellCoordinate
CxSheetModel::getSheetExtents(void)
{
    return CxSheetCellCoordinate(maxRowUsed, maxColUsed);
}


//-------------------------------------------------------------------------
// CxSheetModel::isTouched
//
// Returns 1 if sheet has been modified since last save
//-------------------------------------------------------------------------
int
CxSheetModel::isTouched(void)
{
    return touched;
}


//-------------------------------------------------------------------------
// CxSheetModel::clearTouched
//
// Clear the touched flag (typically after save)
//-------------------------------------------------------------------------
void
CxSheetModel::clearTouched(void)
{
    touched = 0;
}


//-------------------------------------------------------------------------
// CxSheetModel::isReadOnly
//
// Returns 1 if sheet is read only
//-------------------------------------------------------------------------
int
CxSheetModel::isReadOnly(void)
{
    return readOnly;
}


//-------------------------------------------------------------------------
// CxSheetModel::setReadOnly
//
// Set the read only flag
//-------------------------------------------------------------------------
void
CxSheetModel::setReadOnly(int readOnlyFlag)
{
    readOnly = readOnlyFlag ? 1 : 0;
}


//-------------------------------------------------------------------------
// CxSheetModel::recalculate
//
// Called when a cell is changed to recalculate dependent cells.
//
// HOW THIS WORKS:
// ---------------
// Instead of recalculating ALL formulas (expensive!), we use the dependency
// graph to find only the cells affected by the change, in the correct order.
//
// 1. Ask the dependency graph for cells that depend on lastChangedCell
//    (The graph returns them in topological order - dependencies first)
//
// 2. Evaluate each cell in that order
//    (Because of the ordering, each cell's dependencies are already updated)
//
// EXAMPLE:
// --------
// If A1 changes and: B1=A1*2, C1=B1+5, D1=A1+C1
//
// Old approach: Evaluate ALL formulas in arbitrary order (might be wrong!)
// New approach: Graph returns [B1, C1, D1], evaluate in that order (correct!)
//
// CIRCULAR REFERENCES:
// --------------------
// Circular references are detected at two levels:
// 1. Dependency graph: Cycles are detected during topological sort
// 2. Evaluation time: The evaluation stack catches references to cells
//    currently being evaluated, returning 0 to break the cycle
//-------------------------------------------------------------------------
void
CxSheetModel::recalculate(void)
{
    //-------------------------------------------------------------------------
    // During sheet loading, we defer recalculation until all cells are loaded.
    // This is both for efficiency (avoid recalculating after each cell insert)
    // and correctness (formulas might reference cells not yet loaded).
    //-------------------------------------------------------------------------
    if (loadingInProgress) {
        return;
    }

    // Clear the evaluation stack before starting a new recalculation batch
    variableDatabase->clearEvaluationStack();

    //-------------------------------------------------------------------------
    // Get the list of cells that need recalculation, in correct order.
    //
    // The dependency graph handles the complexity of:
    // - Finding all directly and indirectly affected cells
    // - Sorting them so dependencies are evaluated before dependents
    //-------------------------------------------------------------------------
    CxSList<CxSheetCellCoordinate> cellsToRecalc =
        dependencyGraph.getCellsToRecalculate(lastChangedCell);

    //-------------------------------------------------------------------------
    // Also need to recalculate the changed cell itself if it's a formula.
    // (The dependency graph returns cells that DEPEND ON the changed cell,
    // not the changed cell itself.)
    //-------------------------------------------------------------------------
    CxSheetCell* changedCell = getCellPtr(lastChangedCell);
    if (changedCell != NULL && changedCell->cellType == CxSheetCell::FORMULA) {
        recalculateCell(lastChangedCell);
    }

    //-------------------------------------------------------------------------
    // Evaluate each dependent cell in topological order.
    //
    // Because of the ordering, when we evaluate cell X, all cells that X
    // references have already been evaluated (or are the original changed
    // cell which already has its new value).
    //-------------------------------------------------------------------------
    for (int i = 0; i < (int)cellsToRecalc.entries(); i++) {
        CxSheetCellCoordinate coord = cellsToRecalc.at(i);
        recalculateCell(coord);
    }
}


//-------------------------------------------------------------------------
// CxSheetModel::recalculateCell
//
// Evaluate a single formula cell and update its value.
//
// This is the core evaluation logic for formula cells. It handles:
// - Setting up the variable database for cell reference resolution
// - Pushing/popping the evaluation stack for circular reference detection
// - Evaluating the formula expression
// - Updating the cell's evaluatedValue with the result
//
// CIRCULAR REFERENCE DETECTION:
// -----------------------------
// Before evaluating, we push this cell's coordinate onto the evaluation
// stack. If the formula references a cell that's already on the stack
// (including itself), VariableEvaluate will detect the cycle, set the
// circularReferenceDetected flag, and return 0. After evaluation, we
// check this flag and set the cell's value to 0 for circular references.
//
// This handles:
// - Self-references: A1 = A:1 + 1 → evaluates to 0
// - Mutual references: A1 = B:1 + 1, B1 = A:1 + 1 → both evaluate to 0
// - Chain references: A1 = B:1, B1 = C:1, C1 = A:1 → all evaluate to 0
//-------------------------------------------------------------------------
void
CxSheetModel::recalculateCell(CxSheetCellCoordinate coord)
{
    CxSheetCell* cell = getCellPtr(coord);

    if (cell == NULL || cell->cellType != CxSheetCell::FORMULA || cell->formula == NULL) {
        return;  // Not a formula cell - nothing to evaluate
    }

    // Set our variable database for cell reference resolution
    cell->formula->setVariableDatabase(variableDatabase);

    // Push this cell onto the evaluation stack.
    // If the formula tries to reference this cell (directly or through
    // a chain of references), the stack check will detect the cycle.
    variableDatabase->pushEvaluationStack(coord);

    // Evaluate the formula
    double result = 0.0;
    CxExpression::expressionStatus status = cell->formula->Evaluate(&result);

    // Pop from the evaluation stack
    variableDatabase->popEvaluationStack();

    // Update the cell's evaluated value
    if (variableDatabase->hasCircularReference()) {
        // Circular reference detected - set to 0 (like Excel shows #REF! error)
        cell->evaluatedValue = CxDouble(0.0);
    } else if (status == CxExpression::EVALUATION_SUCCESS) {
        cell->evaluatedValue = CxDouble(result);
    }
    // Note: If evaluation failed for other reasons, we leave the old value.
    // Could add error handling here if needed.
}


//-------------------------------------------------------------------------
// CxSheetModel::recalculateAll
//
// Recalculate ALL formula cells in the correct dependency order.
//
// WHEN TO USE:
// ------------
// This is used after loading a sheet or copying a model. In these cases,
// we can't use the targeted recalculation (based on what changed) because
// everything is new - we need to evaluate all formulas.
//
// HOW IT WORKS:
// -------------
// 1. Collect all formula cells
// 2. Use topological sort to order them by dependencies
// 3. Evaluate each in order
//
// This ensures that if formula B depends on formula A, we evaluate A first.
//
// CIRCULAR REFERENCES:
// --------------------
// Cells in circular references are detected at evaluation time via the
// evaluation stack. When a formula references a cell currently being
// evaluated, VariableEvaluate returns 0 to break the cycle.
//-------------------------------------------------------------------------
void
CxSheetModel::recalculateAll(void)
{
    // Clear the evaluation stack before starting
    variableDatabase->clearEvaluationStack();

    //-------------------------------------------------------------------------
    // Collect all formula cells and build a list of their coordinates.
    // We'll then get them in topological order from the dependency graph.
    //-------------------------------------------------------------------------
    CxSList<CxSheetCellCoordinate> formulaCells;

    CxHashmapIterator<CxSheetCellCoordinate, CxSheetCell> iter(&cellHashMap);

    while (iter.next()) {
        const CxSheetCellCoordinate* key = iter.getKey();
        CxSheetCell* cell = iter.getEntry();

        if (key != NULL && cell != NULL && cell->cellType == CxSheetCell::FORMULA) {
            formulaCells.append(*key);
        }
    }

    //-------------------------------------------------------------------------
    // Get all cells in topological order.
    // We use getCellsToRecalculateMultiple with all formula cells as the
    // "changed" cells - this gives us everything in the right order.
    //
    // Actually, for a full recalc, we just need to sort all formula cells
    // by their dependencies. We can use the topological sort directly
    // since we want all of them.
    //-------------------------------------------------------------------------

    // For each formula cell, we need to evaluate in dependency order.
    // The simplest approach: iterate until all are evaluated.
    // A cell can be evaluated if all its dependencies are either:
    // - Non-formula cells (always ready)
    // - Formula cells that have already been evaluated this round

    // For now, use a simpler approach: evaluate in multiple passes
    // until everything is done. This handles dependencies correctly
    // even without explicit sorting.

    int maxPasses = (int)formulaCells.entries() + 1;  // Safety limit
    int evaluated = 0;

    // Track which cells we've already evaluated this round
    CxSList<CxSheetCellCoordinate> evaluatedCells;

    for (int pass = 0; pass < maxPasses && evaluated < (int)formulaCells.entries(); pass++) {
        for (int i = 0; i < (int)formulaCells.entries(); i++) {
            CxSheetCellCoordinate coord = formulaCells.at(i);

            // Skip if already evaluated
            int alreadyDone = 0;
            for (int j = 0; j < (int)evaluatedCells.entries(); j++) {
                if (evaluatedCells.at(j) == coord) {
                    alreadyDone = 1;
                    break;
                }
            }
            if (alreadyDone) {
                continue;
            }

            // Try to evaluate this cell
            recalculateCell(coord);
            evaluatedCells.append(coord);
            evaluated++;
        }
    }
}


//-------------------------------------------------------------------------
// CxSheetModel::updateDependencies
//
// Update the dependency graph when a formula cell is added or changed.
//
// HOW IT WORKS:
// -------------
// 1. Get the list of cell references from the formula (via CxExpression)
// 2. For each referenced cell, tell the dependency graph:
//    "This formula cell depends on that referenced cell"
//
// EXAMPLE:
// --------
// If cell C1 has formula "=A1+B1":
//   - formula->GetVariableList() returns ["A:1", "B:1"]
//   - We call dependencyGraph.addDependency(C1, A1)
//   - We call dependencyGraph.addDependency(C1, B1)
//
// Later, when A1 or B1 changes, the graph knows to include C1 in
// the list of cells to recalculate.
//-------------------------------------------------------------------------
void
CxSheetModel::updateDependencies(CxSheetCellCoordinate coord, CxSheetCell* cell)
{
    if (cell == NULL || cell->formula == NULL) {
        return;  // No formula - no dependencies to register
    }

    // Get the list of variable names (cell references) from the formula.
    // For "=A1+B1", this returns ["A:1", "B:1"]
    CxSList<CxString> varList = cell->formula->GetVariableList();

    // Register each cell reference as a dependency
    for (int i = 0; i < (int)varList.entries(); i++) {
        CxString varName = varList.at(i);

        // Parse the variable name as a cell coordinate
        CxSheetCellCoordinate refCoord;
        if (refCoord.parseAddress(varName)) {
            // Successfully parsed - register the dependency
            // "coord depends on refCoord"
            dependencyGraph.addDependency(coord, refCoord);
        }
        // If parsing fails, it might be a named variable (not a cell reference).
        // We ignore those for dependency tracking.
    }
}


//-------------------------------------------------------------------------
// CxSheetModel::clearDependencies
//
// Remove all dependencies for a cell from the dependency graph.
//
// WHEN TO CALL:
// -------------
// Call this BEFORE changing a cell's value or formula. This ensures:
// - If the cell had a formula, it's removed from dependency lists
// - The graph stays in sync with the actual cell contents
//
// The next step is usually to insert the new cell, then call
// updateDependencies() if it's a formula.
//-------------------------------------------------------------------------
void
CxSheetModel::clearDependencies(CxSheetCellCoordinate coord)
{
    dependencyGraph.clearDependenciesFor(coord);
}
