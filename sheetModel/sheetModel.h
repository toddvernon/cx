//-------------------------------------------------------------------------------------------------
//
//  sheetModel.h
//
//  CxSheetModel Class
//
//-------------------------------------------------------------------------------------------------
//
//  (c) Copyright 2026 T.Vernon
//  ALL RIGHTS RESERVED
//
//  The software and information contained herein are proprietary to, and
//  comprise valuable trade secrets of, T.Vernon , which
//  intends to preserve as trade secrets such software and information.
//  This software is furnished pursuant to a written license agreement and
//  may be used, copied, transmitted, and stored only in accordance with
//  the terms of such license and with the inclusion of the above copyright
//  notice.  This software and information or any other copies thereof may
//  not be provided or otherwise made available to any other person.
//
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// system includes
//-------------------------------------------------------------------------------------------------
#include <stdio.h>

//-------------------------------------------------------------------------------------------------
// cx library includes
//-------------------------------------------------------------------------------------------------
#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/file.h>
#include <cx/base/hashmap.h>

//-------------------------------------------------------------------------------------------------
// sheetModel includes
//-------------------------------------------------------------------------------------------------
#include <cx/sheetModel/sheetCellCoordinate.h>
#include <cx/sheetModel/sheetCell.h>
#include <cx/sheetModel/sheetDependencyGraph.h>

#ifndef _CxSheetModel_
#define _CxSheetModel_

// Forward declaration
class CxSheetVariableDatabase;


//-------------------------------------------------------------------------------------------------
//
// CxSheetModel
//
// The main spreadsheet model class for MVC design. Maintains a grid of cells stored
// in a hashmap, tracks the current cursor position, and handles cell operations.
//
//-------------------------------------------------------------------------------------------------

class CxSheetModel
{
  public:

    enum ACTION {

        NONE,               // No screen change
                            // caused by an action that can't be performed or does
                            // not result in a screen change

        CURSOR_RIGHT,       // move the cursor to the right on current line
                            // caused when the right arrow is typed and there is a
                            // a character to the right, or the append point is to
                            // to the right (ie appending characters to current line)

        CURSOR_LEFT,        // move the cursor left on the current line
                            // caused when the left arrow is typed and there is a
                            // character the left on the current line

        CURSOR_UP,          // move the cursor up to the previous line
                            // caused by typing the up arrow when there is an
                            // editable line above the current one

        CURSOR_DOWN,        // move the cursor down to the next line
                            // caused by typeing the down arrow when there is another
                            // editiable line below the current line

        JUMP_DIRECT         // move the cursor directly to the specified cell
    };

    CxSheetModel(void);
    // constructor

    CxSheetModel(const CxSheetModel& other);
    // copy constructor

    ~CxSheetModel(void);
    // destructor

    CxSheetModel& operator=(const CxSheetModel& other);
    // assignment operator

    void reset(void);
    // reset the class to initial condition and empty sheet

    void pasteFromCutBuffer(CxSheetCell cell);
    // paste cell data into selected cell

    ACTION cursorRightRequest(void);
    // request to move the cursor right in the sheet
    // returns the action that was performed

    ACTION cursorLeftRequest(void);
    // request to move the cursor left in the sheet
    // returns the action that was performed

    ACTION cursorUpRequest(void);
    // request to move the cursor up in the sheet
    // returns the action that was performed

    ACTION cursorDownRequest(void);
    // request to move the cursor down in the sheet
    // returns the action that was performed

    ACTION jumpToCell(CxSheetCellCoordinate coord);
    // jump cursor directly to specified cell
    // returns JUMP_DIRECT on success, NONE on failure

    CxSheetCellCoordinate getCurrentPosition(void);
    // get the current cursor position

    CxSheetCell getCell(CxSheetCellCoordinate coord);
    // gets a copy of the CxSheetCell at the cell coordinate

    CxSheetCell* getCellPtr(CxSheetCellCoordinate coord);
    // gets a pointer to the CxSheetCell at the cell coordinate (or NULL if not found)
    // note: the pointer may become invalid if the hashmap is modified

    void setCell(CxSheetCellCoordinate coord, CxSheetCell cell);
    // copies the passed in cell value into the cell at the referenced coordinate

    int loadSheet(CxString filepath);
    // load the sheet from disk. Sheet is stored in json format
    // returns 1 on success, 0 on failure

    int saveSheet(CxString filepath);
    // save the sheet to a file in json format
    // returns 1 on success, 0 on failure

    unsigned long numberOfRows(void);
    // returns the number of rows that contain data (highest row + 1)

    unsigned long numberOfColumns(void);
    // returns the number of columns that contain data (highest column + 1)

    CxSheetCellCoordinate getSheetExtents(void);
    // returns the coordinate of the most right/lower cell that contains data

    int isTouched(void);
    // returns 1 if sheet has been modified since last save

    void clearTouched(void);
    // clear the touched flag (typically after save)

    int isReadOnly(void);
    // returns 1 if sheet is read only

    void setReadOnly(int readOnly);
    // set the read only flag


  private:

    //---------------------------------------------------------------------------------------------
    // RECALCULATION METHODS
    //
    // The spreadsheet uses dependency tracking for efficient recalculation.
    // When a cell changes, only cells that depend on it (directly or indirectly)
    // are recalculated, and they're evaluated in the correct order.
    //
    // See sheetDependencyGraph.h for detailed explanation of how this works.
    //---------------------------------------------------------------------------------------------

    void recalculate(void);
    // Called when a cell is changed to recalc dependent cells.
    // Uses the dependency graph to determine what needs recalculating.

    void recalculateCell(CxSheetCellCoordinate coord);
    // Evaluate a single formula cell and update its value.
    // Called by recalculate() for each cell that needs updating.

    void updateDependencies(CxSheetCellCoordinate coord, CxSheetCell* cell);
    // Update the dependency graph when a cell's formula changes.
    // Clears old dependencies and adds new ones based on the formula.

    void clearDependencies(CxSheetCellCoordinate coord);
    // Remove all dependencies for a cell (when it's cleared or changed).

    CxSheetCellCoordinate lastChangedCell;
    // Tracks which cell triggered the current recalculation.
    // Used by recalculate() to know where to start the dependency traversal.

    CxSheetDependencyGraph dependencyGraph;
    // Tracks which cells depend on which other cells.
    // Used to efficiently recalculate only affected cells in correct order.

    int loadingInProgress;
    // Flag set during loadSheet() to defer recalculation.
    // When loading, we don't want to recalculate after every cell insert.
    // Instead, we do one full recalculation at the end of loading.

    void recalculateAll(void);
    // Recalculate ALL formula cells in dependency order.
    // Used after loading a sheet (can't use targeted recalc since everything is new).

    CxSheetCellCoordinate currentCellPosition;
    // the current location of cursor in the sheet

    CxHashmap<CxSheetCellCoordinate, CxSheetCell> cellHashMap;
    // storage for the cells in hashmap format

    CxString sheetPath;
    // file path to the current sheet path or empty if new sheet

    int readOnly;
    // flag that buffer is read only

    int touched;
    // has file been changed

    unsigned long maxRowUsed;
    // tracks the highest row number that has data

    unsigned long maxColUsed;
    // tracks the highest column number that has data

    CxSheetVariableDatabase* variableDatabase;
    // variable database for formula evaluation (owned pointer)
};


#endif
