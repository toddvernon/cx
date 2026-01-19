//-------------------------------------------------------------------------------------------------
//
//  sheetDependencyGraph.h
//
//  CxSheetDependencyGraph Class
//
//-------------------------------------------------------------------------------------------------
//
//  (c) Copyright 2026 T.Vernon
//  ALL RIGHTS RESERVED
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxSheetDependencyGraph_
#define _CxSheetDependencyGraph_

//-------------------------------------------------------------------------------------------------
// includes
//-------------------------------------------------------------------------------------------------
#include <cx/base/slist.h>
#include <cx/base/hashmap.h>
#include "sheetCellCoordinate.h"


//-------------------------------------------------------------------------------------------------
//
// CxSheetDependencyGraph
//
// PURPOSE:
// --------
// This class manages the dependency relationships between cells in a spreadsheet.
// When a cell contains a formula that references other cells, we need to know:
//   1. Which cells does this formula depend on? (so we can recalculate when they change)
//   2. Which formulas depend on this cell? (so we know what to recalculate)
//
//
// THE PROBLEM WE'RE SOLVING:
// --------------------------
// Consider this spreadsheet:
//
//     A1 = 10
//     B1 = A1 * 2          (B1 references A1, so B1 "depends on" A1)
//     C1 = B1 + 5          (C1 references B1, so C1 "depends on" B1)
//     D1 = A1 + C1         (D1 references A1 and C1)
//
// When A1 changes, we need to recalculate B1, C1, and D1. But ORDER MATTERS:
//   - B1 must be calculated before C1 (because C1 uses B1's value)
//   - C1 must be calculated before D1 (because D1 uses C1's value)
//
// Correct order: A1 changes -> recalc B1 -> recalc C1 -> recalc D1
// Wrong order:   A1 changes -> recalc D1 (gets stale C1!) -> recalc C1 -> recalc B1
//
//
// HOW IT WORKS:
// -------------
// We maintain a "reverse dependency" map called "dependents":
//
//     dependents[A1] = {B1, D1}    // B1 and D1 both reference A1
//     dependents[B1] = {C1}        // C1 references B1
//     dependents[C1] = {D1}        // D1 references C1
//
// When A1 changes:
//   1. FIND AFFECTED CELLS: Starting from A1, follow the dependents map to find
//      all cells that need recalculation (directly or indirectly).
//      Result: {B1, C1, D1}
//
//   2. TOPOLOGICAL SORT: Order these cells so that if X depends on Y, then Y
//      comes before X in the list. This uses Kahn's algorithm.
//      Result: [B1, C1, D1]  (B1 has no dependencies in our set, C1 depends on B1, etc.)
//
//   3. EVALUATE IN ORDER: Calculate each cell in the sorted order.
//      Now each cell's dependencies are guaranteed to be up-to-date.
//
//
// TOPOLOGICAL SORT (Kahn's Algorithm):
// ------------------------------------
// Kahn's algorithm finds an ordering where dependencies come before dependents:
//
//   1. Count how many dependencies each cell has (within our affected set)
//   2. Start with cells that have zero dependencies (nothing they need is changing)
//   3. "Process" each zero-dependency cell:
//      - Add it to the output list
//      - For each cell that depends on it, decrement their dependency count
//      - If any cell's count reaches zero, it's ready to process
//   4. Repeat until all cells are processed
//
// If we can't process all cells, there's a circular reference (handled elsewhere).
//
//
// EXAMPLE WALKTHROUGH:
// --------------------
// Given: A1=10, B1=A1*2, C1=B1+5, D1=A1+C1
// A1 changes to 20.
//
// Step 1 - Find affected cells starting from A1:
//   - A1's dependents: {B1, D1}
//   - B1's dependents: {C1}
//   - C1's dependents: {D1}
//   - D1's dependents: {}
//   - Affected set: {B1, C1, D1}
//
// Step 2 - Build dependency counts (only counting deps within affected set):
//   - B1 depends on: A1 (not in set) -> count = 0
//   - C1 depends on: B1 (in set) -> count = 1
//   - D1 depends on: A1 (not in set), C1 (in set) -> count = 1
//
// Step 3 - Kahn's algorithm:
//   - Initial queue: [B1] (only one with count=0)
//   - Process B1: output [B1], decrement C1's count -> C1 count = 0, queue: [C1]
//   - Process C1: output [B1, C1], decrement D1's count -> D1 count = 0, queue: [D1]
//   - Process D1: output [B1, C1, D1], queue empty
//   - Done! Evaluation order: B1, C1, D1
//
//
// INTERFACE DESIGN:
// -----------------
// The interface is intentionally simple. The caller (CxSheetModel) just needs to:
//   1. Tell us when dependencies change (formula added/changed/removed)
//   2. Ask "what cells need recalculating when X changes?"
//
// All the graph management and sorting complexity is hidden inside this class.
//
//-------------------------------------------------------------------------------------------------

class CxSheetDependencyGraph
{
  public:

    CxSheetDependencyGraph(void);
    // Default constructor - creates empty dependency graph

    ~CxSheetDependencyGraph(void);
    // Destructor

    //---------------------------------------------------------------------------------------------
    // DEPENDENCY MANAGEMENT
    //
    // These methods maintain the dependency graph as formulas are added, changed, or removed.
    //---------------------------------------------------------------------------------------------

    void addDependency(CxSheetCellCoordinate formula, CxSheetCellCoordinate referencedCell);
    // Record that 'formula' depends on 'referencedCell'.
    //
    // Example: If C1 contains "=A1+B1", call:
    //   addDependency(C1, A1);
    //   addDependency(C1, B1);
    //
    // This adds C1 to the dependents list of both A1 and B1.
    // When A1 or B1 changes, we'll know to recalculate C1.

    void removeDependency(CxSheetCellCoordinate formula, CxSheetCellCoordinate referencedCell);
    // Remove the dependency record. Used when a formula changes or is deleted.
    //
    // Example: If C1 changes from "=A1+B1" to "=A1*2", call:
    //   removeDependency(C1, B1);  // C1 no longer depends on B1

    void clearDependenciesFor(CxSheetCellCoordinate formula);
    // Remove ALL dependencies for a cell. Call this before setting up new dependencies
    // when a formula changes, or when a cell is cleared/deleted.
    //
    // Example: If C1's formula is being replaced:
    //   clearDependenciesFor(C1);  // Remove C1 from all dependents lists
    //   // Then add new dependencies...

    //---------------------------------------------------------------------------------------------
    // RECALCULATION SUPPORT
    //
    // These methods determine what needs recalculating and in what order.
    //---------------------------------------------------------------------------------------------

    CxSList<CxSheetCellCoordinate> getCellsToRecalculate(CxSheetCellCoordinate changedCell);
    // Returns an ordered list of cells that need recalculation when 'changedCell' changes.
    //
    // The returned list is in TOPOLOGICAL ORDER: if cell X depends on cell Y,
    // then Y appears before X in the list. This guarantees that when we evaluate
    // each cell in order, all of its dependencies have already been updated.
    //
    // Example: If A1 changes and B1=A1*2, C1=B1+5, D1=A1+C1
    //   Returns: [B1, C1, D1]  (in this order)
    //
    // The caller should then evaluate B1, then C1, then D1.
    //
    // NOTE: The changed cell itself (A1 in this example) is NOT in the returned list.
    // It's assumed the caller has already updated that cell's value.

    CxSList<CxSheetCellCoordinate> getCellsToRecalculateMultiple(
        CxSList<CxSheetCellCoordinate> changedCells);
    // Same as above, but for multiple cells changing at once.
    // Useful for paste operations or initial load.

    //---------------------------------------------------------------------------------------------
    // DEBUGGING / DIAGNOSTICS
    //---------------------------------------------------------------------------------------------

    int getDependentCount(CxSheetCellCoordinate cell);
    // Returns how many formulas directly depend on this cell.
    // Useful for debugging and testing.

    void clear(void);
    // Remove all dependency information. Used when resetting the spreadsheet.

  private:

    //---------------------------------------------------------------------------------------------
    // INTERNAL DATA STRUCTURE
    //
    // We store "reverse dependencies" - for each cell, we track which formulas reference it.
    //
    // Key:   A cell coordinate (e.g., A1)
    // Value: List of cells whose formulas reference the key cell
    //
    // Example state for: B1=A1*2, C1=B1+5, D1=A1+C1
    //
    //   dependents[A1] = [B1, D1]   // B1 and D1 both reference A1
    //   dependents[B1] = [C1]       // C1 references B1
    //   dependents[C1] = [D1]       // D1 references C1
    //
    // We DON'T store forward dependencies (what each formula references) because:
    //   1. We can get that from CxExpression::GetVariableList() when needed
    //   2. It would be redundant data to keep in sync
    //---------------------------------------------------------------------------------------------

    CxHashmap<CxSheetCellCoordinate, CxSList<CxSheetCellCoordinate> > dependents;

    //---------------------------------------------------------------------------------------------
    // INTERNAL HELPER METHODS
    //---------------------------------------------------------------------------------------------

    CxSList<CxSheetCellCoordinate> findAllAffectedCells(CxSheetCellCoordinate startCell);
    // Starting from 'startCell', find all cells that are affected (directly or indirectly).
    // Uses breadth-first search through the dependents map.
    //
    // Example: If A1 changes and dependents[A1]={B1}, dependents[B1]={C1}
    //   Returns: [B1, C1]

    CxSList<CxSheetCellCoordinate> findAllAffectedCellsMultiple(
        CxSList<CxSheetCellCoordinate> startCells);
    // Same as above but starting from multiple cells.

    CxSList<CxSheetCellCoordinate> topologicalSort(CxSList<CxSheetCellCoordinate> cells);
    // Given a set of cells, return them in topological order.
    // Uses Kahn's algorithm (see class documentation above).
    //
    // If there's a cycle (circular reference), returns cells in arbitrary order.
    // Circular references are detected elsewhere (during evaluation).

    int isInList(CxSList<CxSheetCellCoordinate>& list, CxSheetCellCoordinate coord);
    // Helper: returns 1 if coord is in list, 0 otherwise.
    // Used to avoid duplicates when building affected cell sets.

    void addToListIfNotPresent(CxSList<CxSheetCellCoordinate>& list, CxSheetCellCoordinate coord);
    // Helper: adds coord to list only if not already present.
};


#endif
