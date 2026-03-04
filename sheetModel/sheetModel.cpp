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
#include "sheetFunctionDatabase.h"
#include "sheetCellRange.h"
#include <cx/expression/expression.h>
#include <cx/base/file.h>
// Regular JSON for parsing (CxJSONFactory returns CxJSONBase*)
#include <cx/json/json_factory.h>
#include <cx/json/json_member.h>
#include <cx/json/json_string.h>
#include <cx/json/json_number.h>
#include <cx/json/json_boolean.h>
#include <cx/json/json_null.h>
#include <cx/json/json_object.h>
#include <cx/json/json_array.h>

// UTF-8 JSON for saving and appAttributes
#include <cx/json/json_utf_member.h>
#include <cx/json/json_utf_string.h>
#include <cx/json/json_utf_number.h>
#include <cx/json/json_utf_boolean.h>
#include <cx/json/json_utf_null.h>
#include <cx/json/json_utf_object.h>
#include <cx/json/json_utf_array.h>


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
, functionDatabase(NULL)
, loadingInProgress(0)
, _appData(NULL)
{
    variableDatabase = new CxSheetVariableDatabase(this);
    functionDatabase = new CxSheetFunctionDatabase(this);
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
, functionDatabase(NULL)
, loadingInProgress(0)
, _appData(NULL)
{
    // Create our own variable and function databases pointing to this model
    variableDatabase = new CxSheetVariableDatabase(this);
    functionDatabase = new CxSheetFunctionDatabase(this);

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

    // Clean up the function database
    if (functionDatabase != NULL) {
        delete functionDatabase;
        functionDatabase = NULL;
    }

    // Clean up app data
    if (_appData != NULL) {
        delete _appData;
        _appData = NULL;
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

    // Clear app data
    if (_appData != NULL) {
        delete _appData;
        _appData = NULL;
    }

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
// CxSheetModel::getLastAffectedCells
//
// Returns the list of cells affected by the last setCell() or load operation.
// This includes the directly changed cell and all cells that were recalculated
// due to formula dependencies. The list is in evaluation order.
//-------------------------------------------------------------------------
CxSList<CxSheetCellCoordinate>
CxSheetModel::getLastAffectedCells(void)
{
    return _lastAffectedCells;
}


//-------------------------------------------------------------------------
// CxSheetModel::getAppData
//
// Returns pointer to app data object. Apps can use this to read visual
// attributes like column widths. Returns NULL if no app data exists.
//-------------------------------------------------------------------------
CxJSONUTFObject*
CxSheetModel::getAppData(void)
{
    return _appData;
}


//-------------------------------------------------------------------------
// CxSheetModel::setAppData
//
// Set the app data object. sheetModel takes ownership of the pointer.
// Pass NULL to clear app data.
//-------------------------------------------------------------------------
void
CxSheetModel::setAppData(CxJSONUTFObject* data)
{
    if (_appData != NULL) {
        delete _appData;
    }
    _appData = data;
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
    // so that cell references like "A1" can be recognized.
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
        // with the variableDatabase and functionDatabase
        CxSheetCell* insertedCell = (CxSheetCell*)cellHashMap.find(coord);
        if (insertedCell != NULL) {
            insertedCell->formula = new CxExpression(formulaText, variableDatabase, functionDatabase);
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
//
// Unknown keys in cell objects are collected into appAttributes, allowing
// applications to store custom attributes that survive round-trips through
// processing engines that don't know about them.
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

    // Load current position if present (stored as cell address like "A1")
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

            // Get cell address (e.g., "A1", "B2")
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
            }

            //---------------------------------------------------------------------
            // Collect unknown keys into appAttributes
            // Known keys: "cell", "type", "value", "text", "formula"
            // Everything else is an app attribute that we preserve
            // Note: We convert from CxJSON* (parse result) to CxJSONUTF* (for storage)
            //---------------------------------------------------------------------
            for (int j = 0; j < cellObj->entries(); j++) {
                CxJSONMember* member = cellObj->at(j);
                if (member == NULL) continue;

                CxString key = member->var();

                // Skip known keys
                if (key == "cell" || key == "type" || key == "value" ||
                    key == "text" || key == "formula") {
                    continue;
                }

                // This is an app attribute - add to cell's appAttributes
                // Convert from parsed CxJSON* to CxJSONUTF* for storage
                CxJSONUTFBase* valueCopy = NULL;
                CxJSONBase* origValue = member->object();
                if (origValue != NULL) {
                    switch (origValue->type()) {
                        case CxJSONBase::STRING:
                            valueCopy = new CxJSONUTFString(((CxJSONString*)origValue)->get().data());
                            break;
                        case CxJSONBase::NUMBER:
                            valueCopy = new CxJSONUTFNumber(((CxJSONNumber*)origValue)->get());
                            break;
                        case CxJSONBase::BOOLEAN:
                            valueCopy = new CxJSONUTFBoolean(((CxJSONBoolean*)origValue)->get());
                            break;
                        default:
                            break;
                    }
                }

                if (valueCopy != NULL) {
                    if (cell.appAttributes == NULL) {
                        cell.appAttributes = new CxJSONUTFObject();
                    }
                    cell.appAttributes->append(new CxJSONUTFMember(key.data(), valueCopy));
                }
            }

            // Insert cell into model (setCell will update maxRowUsed/maxColUsed)
            setCell(coord, cell);
        }
    }

    //-------------------------------------------------------------------------
    // Preserve unknown top-level keys in _appData.
    // Known keys: "version", "currentPosition", "cells"
    // Everything else is app data (like "columns" for column widths)
    //-------------------------------------------------------------------------
    for (int i = 0; i < root->entries(); i++) {
        CxJSONMember* member = root->at(i);
        if (member == NULL) continue;

        CxString key = member->var();

        // Skip known keys
        if (key == "version" || key == "currentPosition" || key == "cells") {
            continue;
        }

        // This is app data - preserve it
        // Convert from parsed CxJSON* to CxJSONUTF* for storage
        if (_appData == NULL) {
            _appData = new CxJSONUTFObject();
        }

        CxJSONBase* origValue = member->object();
        if (origValue != NULL && origValue->type() == CxJSONBase::OBJECT) {
            // Deep copy the object
            CxJSONUTFObject* objCopy = new CxJSONUTFObject();
            CxJSONObject* origObj = (CxJSONObject*)origValue;
            for (int j = 0; j < origObj->entries(); j++) {
                CxJSONMember* subMember = origObj->at(j);
                if (subMember == NULL) continue;

                CxJSONBase* subValue = subMember->object();
                CxJSONUTFBase* subCopy = NULL;

                if (subValue != NULL && subValue->type() == CxJSONBase::OBJECT) {
                    // Nested object (e.g., {"width": 12})
                    CxJSONUTFObject* nestedCopy = new CxJSONUTFObject();
                    CxJSONObject* nestedObj = (CxJSONObject*)subValue;
                    for (int k = 0; k < nestedObj->entries(); k++) {
                        CxJSONMember* nestedMember = nestedObj->at(k);
                        if (nestedMember == NULL) continue;
                        CxJSONBase* nestedValue = nestedMember->object();
                        CxJSONUTFBase* nestedValueCopy = NULL;
                        if (nestedValue != NULL) {
                            switch (nestedValue->type()) {
                                case CxJSONBase::STRING:
                                    nestedValueCopy = new CxJSONUTFString(((CxJSONString*)nestedValue)->get().data());
                                    break;
                                case CxJSONBase::NUMBER:
                                    nestedValueCopy = new CxJSONUTFNumber(((CxJSONNumber*)nestedValue)->get());
                                    break;
                                case CxJSONBase::BOOLEAN:
                                    nestedValueCopy = new CxJSONUTFBoolean(((CxJSONBoolean*)nestedValue)->get());
                                    break;
                                default:
                                    break;
                            }
                        }
                        if (nestedValueCopy != NULL) {
                            nestedCopy->append(new CxJSONUTFMember(nestedMember->var().data(), nestedValueCopy));
                        }
                    }
                    subCopy = nestedCopy;
                } else if (subValue != NULL) {
                    switch (subValue->type()) {
                        case CxJSONBase::STRING:
                            subCopy = new CxJSONUTFString(((CxJSONString*)subValue)->get().data());
                            break;
                        case CxJSONBase::NUMBER:
                            subCopy = new CxJSONUTFNumber(((CxJSONNumber*)subValue)->get());
                            break;
                        case CxJSONBase::BOOLEAN:
                            subCopy = new CxJSONUTFBoolean(((CxJSONBoolean*)subValue)->get());
                            break;
                        default:
                            break;
                    }
                }

                if (subCopy != NULL) {
                    objCopy->append(new CxJSONUTFMember(subMember->var().data(), subCopy));
                }
            }
            _appData->append(new CxJSONUTFMember(key.data(), objCopy));
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
//   "currentPosition": "A1",
//   "cells": [
//     {"cell": "A1", "type": "double", "value": 42.5, "bold": true, "fgColor": "RGB:255,0,0"},
//     {"cell": "B1", "type": "text", "text": "Hello", "bgColor": "ANSI:YELLOW"},
//     {"cell": "A2", "type": "formula", "formula": "=A1+10"}
//   ]
// }
//
// App attributes are merged directly into each cell's JSON object as first-class keys.
// This allows applications to store display/formatting attributes without sheetModel
// needing to know about them. The keys appear as regular JSON members, making files
// easy to hand-edit and process with standard JSON tools.
//
//-------------------------------------------------------------------------
int
CxSheetModel::saveSheet(CxString filepath)
{
    // Create the root JSON object (using UTF-8 aware classes)
    CxJSONUTFObject *root = new CxJSONUTFObject();

    // Add version
    root->append(new CxJSONUTFMember("version", new CxJSONUTFNumber(1)));

    // Add current position as cell address (e.g., "A1")
    root->append(new CxJSONUTFMember("currentPosition",
        new CxJSONUTFString(currentCellPosition.toAddress().data())));

    // Add cells array
    CxJSONUTFArray *cellsArray = new CxJSONUTFArray();

    // Iterate through all cells and add to array
    CxHashmapIterator<CxSheetCellCoordinate, CxSheetCell> iter(&cellHashMap);

    while (iter.next()) {
        const CxSheetCellCoordinate* key = iter.getKey();
        CxSheetCell* cell = iter.getEntry();

        if (key == NULL || cell == NULL) {
            continue;
        }

        // Skip empty cells that have no appAttributes
        // (cells with appAttributes like symbolFill should be saved)
        if (cell->getType() == CxSheetCell::EMPTY && cell->appAttributes == NULL) {
            continue;
        }

        CxJSONUTFObject *cellObj = new CxJSONUTFObject();

        // Add cell address (e.g., "A1", "B2")
        cellObj->append(new CxJSONUTFMember("cell", new CxJSONUTFString(key->toAddress().data())));

        // Add type-specific data
        switch (cell->getType()) {

            case CxSheetCell::TEXT:
                cellObj->append(new CxJSONUTFMember("type", new CxJSONUTFString("text")));
                cellObj->append(new CxJSONUTFMember("text", new CxJSONUTFString(cell->getText().data())));
                break;

            case CxSheetCell::DOUBLE:
                cellObj->append(new CxJSONUTFMember("type", new CxJSONUTFString("double")));
                cellObj->append(new CxJSONUTFMember("value", new CxJSONUTFNumber(cell->getDouble().value)));
                break;

            case CxSheetCell::FORMULA:
                cellObj->append(new CxJSONUTFMember("type", new CxJSONUTFString("formula")));
                // Prepend "=" for readability (like Excel)
                cellObj->append(new CxJSONUTFMember("formula",
                    new CxJSONUTFString((CxString("=") + cell->getFormulaText()).data())));
                break;

            case CxSheetCell::EMPTY:
                // Empty cells with appAttributes (e.g., symbolFill) - just save the type
                // The appAttributes will be merged below
                cellObj->append(new CxJSONUTFMember("type", new CxJSONUTFString("empty")));
                break;

            default:
                // Skip unknown types
                delete cellObj;
                continue;
        }

        // Merge appAttributes into the cell object (if present)
        // Each key from appAttributes becomes a first-class member of the cell JSON
        if (cell->appAttributes != NULL) {
            for (int i = 0; i < cell->appAttributes->entries(); i++) {
                CxJSONUTFMember* member = cell->appAttributes->at(i);
                if (member != NULL) {
                    // Clone the member value based on its type
                    CxJSONUTFBase* valueCopy = NULL;
                    CxJSONUTFBase* origValue = member->object();
                    if (origValue != NULL) {
                        switch (origValue->type()) {
                            case CxJSONUTFBase::STRING:
                                valueCopy = new CxJSONUTFString(((CxJSONUTFString*)origValue)->get());
                                break;
                            case CxJSONUTFBase::NUMBER:
                                valueCopy = new CxJSONUTFNumber(((CxJSONUTFNumber*)origValue)->get());
                                break;
                            case CxJSONUTFBase::BOOLEAN:
                                valueCopy = new CxJSONUTFBoolean(((CxJSONUTFBoolean*)origValue)->get());
                                break;
                            default:
                                break;
                        }
                    }
                    if (valueCopy != NULL) {
                        cellObj->append(new CxJSONUTFMember(member->var().toBytes().data(), valueCopy));
                    }
                }
            }
        }

        cellsArray->append(cellObj);
    }

    root->append(new CxJSONUTFMember("cells", cellsArray));

    //-------------------------------------------------------------------------
    // Merge app data into root (if present)
    // This preserves top-level keys like "columns" that apps set
    //-------------------------------------------------------------------------
    if (_appData != NULL) {
        for (int i = 0; i < _appData->entries(); i++) {
            CxJSONUTFMember* member = _appData->at(i);
            if (member == NULL) continue;

            // Deep copy the member value
            CxJSONUTFBase* origValue = member->object();
            CxJSONUTFBase* valueCopy = NULL;

            if (origValue != NULL && origValue->type() == CxJSONUTFBase::OBJECT) {
                // Deep copy the object
                CxJSONUTFObject* objCopy = new CxJSONUTFObject();
                CxJSONUTFObject* origObj = (CxJSONUTFObject*)origValue;
                for (int j = 0; j < origObj->entries(); j++) {
                    CxJSONUTFMember* subMember = origObj->at(j);
                    if (subMember == NULL) continue;

                    CxJSONUTFBase* subValue = subMember->object();
                    CxJSONUTFBase* subCopy = NULL;

                    if (subValue != NULL && subValue->type() == CxJSONUTFBase::OBJECT) {
                        // Nested object
                        CxJSONUTFObject* nestedCopy = new CxJSONUTFObject();
                        CxJSONUTFObject* nestedObj = (CxJSONUTFObject*)subValue;
                        for (int k = 0; k < nestedObj->entries(); k++) {
                            CxJSONUTFMember* nestedMember = nestedObj->at(k);
                            if (nestedMember == NULL) continue;
                            CxJSONUTFBase* nestedValue = nestedMember->object();
                            CxJSONUTFBase* nestedValueCopy = NULL;
                            if (nestedValue != NULL) {
                                switch (nestedValue->type()) {
                                    case CxJSONUTFBase::STRING:
                                        nestedValueCopy = new CxJSONUTFString(((CxJSONUTFString*)nestedValue)->get());
                                        break;
                                    case CxJSONUTFBase::NUMBER:
                                        nestedValueCopy = new CxJSONUTFNumber(((CxJSONUTFNumber*)nestedValue)->get());
                                        break;
                                    case CxJSONUTFBase::BOOLEAN:
                                        nestedValueCopy = new CxJSONUTFBoolean(((CxJSONUTFBoolean*)nestedValue)->get());
                                        break;
                                    default:
                                        break;
                                }
                            }
                            if (nestedValueCopy != NULL) {
                                nestedCopy->append(new CxJSONUTFMember(nestedMember->var().toBytes().data(), nestedValueCopy));
                            }
                        }
                        subCopy = nestedCopy;
                    } else if (subValue != NULL) {
                        switch (subValue->type()) {
                            case CxJSONUTFBase::STRING:
                                subCopy = new CxJSONUTFString(((CxJSONUTFString*)subValue)->get());
                                break;
                            case CxJSONUTFBase::NUMBER:
                                subCopy = new CxJSONUTFNumber(((CxJSONUTFNumber*)subValue)->get());
                                break;
                            case CxJSONUTFBase::BOOLEAN:
                                subCopy = new CxJSONUTFBoolean(((CxJSONUTFBoolean*)subValue)->get());
                                break;
                            default:
                                break;
                        }
                    }

                    if (subCopy != NULL) {
                        objCopy->append(new CxJSONUTFMember(subMember->var().toBytes().data(), subCopy));
                    }
                }
                valueCopy = objCopy;
            }

            if (valueCopy != NULL) {
                root->append(new CxJSONUTFMember(member->var().toBytes().data(), valueCopy));
            }
        }
    }

    // Write to file
    CxFile outFile;
    if (!outFile.open(filepath, "w")) {
        delete root;
        return 0;
    }

    // Serialize to string using portable toPrettyJsonString() method
    CxString jsonStr = root->toPrettyJsonString();
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

    // Clear the affected cells list - we'll populate it as we go
    _lastAffectedCells.clear();

    // The directly changed cell is always affected
    _lastAffectedCells.append(lastChangedCell);

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
        _lastAffectedCells.append(coord);
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
// - Self-references: A1 = A1 + 1 → evaluates to 0
// - Mutual references: A1 = B1 + 1, B1 = A1 + 1 → both evaluate to 0
// - Chain references: A1 = B1, B1 = C1, C1 = A1 → all evaluate to 0
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

    // Set the range list for the function database so it knows about
    // any cell ranges (like A1:A10) used in this formula
    if (functionDatabase != NULL) {
        functionDatabase->setRangeList(cell->formula->GetRangeList());
    }

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
    // Clear the affected cells list - we'll populate it with all cells
    _lastAffectedCells.clear();

    // Clear the evaluation stack before starting
    variableDatabase->clearEvaluationStack();

    //-------------------------------------------------------------------------
    // Collect all cells - the affected list should include everything.
    // Also collect formula cells separately for recalculation.
    //-------------------------------------------------------------------------
    CxSList<CxSheetCellCoordinate> formulaCells;

    CxHashmapIterator<CxSheetCellCoordinate, CxSheetCell> iter(&cellHashMap);

    while (iter.next()) {
        const CxSheetCellCoordinate* key = iter.getKey();
        CxSheetCell* cell = iter.getEntry();

        if (key != NULL && cell != NULL) {
            // All cells are affected during a full recalc (e.g., after load)
            _lastAffectedCells.append(*key);

            if (cell->cellType == CxSheetCell::FORMULA) {
                formulaCells.append(*key);
            }
        }
    }

    //-------------------------------------------------------------------------
    // Sort formula cells in topological order using the dependency graph.
    // This ensures that if formula X depends on formula Y, Y is evaluated first.
    //-------------------------------------------------------------------------
    CxSList<CxSheetCellCoordinate> sortedFormulas =
        dependencyGraph.topologicalSort(formulaCells);

    //-------------------------------------------------------------------------
    // Evaluate each formula cell in topological order.
    // Because of the ordering, when we evaluate cell X, all cells that X
    // references have already been evaluated.
    //-------------------------------------------------------------------------
    for (int i = 0; i < (int)sortedFormulas.entries(); i++) {
        CxSheetCellCoordinate coord = sortedFormulas.at(i);
        recalculateCell(coord);
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
//   - formula->GetVariableList() returns ["A1", "B1"]
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
    // For "=A1+B1", this returns ["A1", "B1"]
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

    //-------------------------------------------------------------------------
    // Handle cell ranges (e.g., A1:A10 in "=SUM(A1:A10)")
    //
    // For each range in the formula, we need to add dependencies on ALL
    // cells in that range. This ensures that when any cell in the range
    // changes, formulas referencing the range get recalculated.
    //-------------------------------------------------------------------------
    CxSList<CxString> rangeList = cell->formula->GetRangeList();

    for (int i = 0; i < (int)rangeList.entries(); i++) {
        CxString rangeStr = rangeList.at(i);

        // Parse and expand the range
        CxSheetCellRange range(rangeStr);
        if (range.isValid()) {
            // Add a dependency for each cell in the range
            unsigned long cellCount = range.cellCount();
            for (unsigned long j = 0; j < cellCount; j++) {
                CxSheetCellCoordinate refCoord = range.cellAt(j);
                dependencyGraph.addDependency(coord, refCoord);
            }
        }
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
