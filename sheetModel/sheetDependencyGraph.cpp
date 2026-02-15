//-------------------------------------------------------------------------------------------------
//
//  sheetDependencyGraph.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetDependencyGraph Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sheetDependencyGraph.h"


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::CxSheetDependencyGraph
//
// Default constructor - creates an empty dependency graph.
// No cells have any dependents yet.
//-------------------------------------------------------------------------
CxSheetDependencyGraph::CxSheetDependencyGraph(void)
{
    // The hashmap is initialized empty by its constructor.
    // No additional setup needed.
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::~CxSheetDependencyGraph
//
// Destructor - clean up all dependency data.
//-------------------------------------------------------------------------
CxSheetDependencyGraph::~CxSheetDependencyGraph(void)
{
    // The hashmap destructor handles cleanup of its contents.
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::addDependency
//
// Record that 'formula' cell depends on 'referencedCell'.
//
// WHAT THIS DOES:
// ---------------
// Adds 'formula' to the dependents list of 'referencedCell'.
// This means: when referencedCell changes, formula needs recalculation.
//
// EXAMPLE:
// --------
// If cell C1 contains the formula "=A1+B1":
//   - Call addDependency(C1, A1)  -> Now dependents[A1] includes C1
//   - Call addDependency(C1, B1)  -> Now dependents[B1] includes C1
//
// Later, when A1 changes, we look up dependents[A1] and find C1,
// so we know to recalculate C1.
//
// DUPLICATE HANDLING:
// -------------------
// We check if the dependency already exists to avoid duplicates.
// This can happen if the same cell is referenced multiple times
// in a formula (e.g., "=A1+A1").
//-------------------------------------------------------------------------
void
CxSheetDependencyGraph::addDependency(CxSheetCellCoordinate formula,
                                       CxSheetCellCoordinate referencedCell)
{
    // Look up the current dependents list for the referenced cell.
    // If no entry exists yet, find() returns NULL.
    CxSList<CxSheetCellCoordinate>* depList =
        (CxSList<CxSheetCellCoordinate>*)dependents.find(referencedCell);

    if (depList == NULL) {
        // First time anything depends on this cell.
        // Create a new list with just this formula.
        CxSList<CxSheetCellCoordinate> newList;
        newList.append(formula);
        dependents.insert(referencedCell, newList);
    }
    else {
        // Already have some dependents. Add this formula if not already present.
        // (Avoid duplicates - a formula might reference the same cell twice)
        addToListIfNotPresent(*depList, formula);
    }
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::removeDependency
//
// Remove the record that 'formula' depends on 'referencedCell'.
//
// WHEN TO USE:
// ------------
// Call this when a formula changes and no longer references a cell.
//
// EXAMPLE:
// --------
// If C1 changes from "=A1+B1" to "=A1*2":
//   - C1 no longer depends on B1
//   - Call removeDependency(C1, B1)
//   - Now dependents[B1] no longer includes C1
//
// SAFETY:
// -------
// It's safe to call this even if the dependency doesn't exist.
// We simply do nothing in that case.
//-------------------------------------------------------------------------
void
CxSheetDependencyGraph::removeDependency(CxSheetCellCoordinate formula,
                                          CxSheetCellCoordinate referencedCell)
{
    // Look up the dependents list for the referenced cell.
    CxSList<CxSheetCellCoordinate>* depList =
        (CxSList<CxSheetCellCoordinate>*)dependents.find(referencedCell);

    if (depList == NULL) {
        // No dependents recorded for this cell - nothing to remove.
        return;
    }

    // Search for 'formula' in the list and remove it.
    for (int i = 0; i < (int)depList->entries(); i++) {
        if (depList->at(i) == formula) {
            depList->removeAt(i);
            return;  // Found and removed - we're done.
        }
    }

    // If we get here, 'formula' wasn't in the list. That's fine.
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::clearDependenciesFor
//
// Remove ALL dependencies for a cell (i.e., remove it from all
// dependents lists it appears in).
//
// WHEN TO USE:
// ------------
// Call this BEFORE setting up new dependencies when:
//   - A cell's formula is being changed
//   - A cell is being cleared/deleted
//
// EXAMPLE:
// --------
// If C1 had formula "=A1+B1" and we're changing it to "=D1*2":
//   1. clearDependenciesFor(C1)  -> Removes C1 from dependents[A1] and dependents[B1]
//   2. addDependency(C1, D1)     -> Adds C1 to dependents[D1]
//
// WHY THIS APPROACH:
// ------------------
// We iterate through ALL entries in the dependents map because we don't
// store "forward dependencies" (what each formula references). We only
// store "reverse dependencies" (who depends on each cell).
//
// For small to medium spreadsheets, this is fine. For very large sheets
// with many formula cells, we might want to also track forward deps.
// But that adds complexity and more data to keep in sync.
//-------------------------------------------------------------------------
void
CxSheetDependencyGraph::clearDependenciesFor(CxSheetCellCoordinate formula)
{
    // We need to check every cell's dependents list and remove 'formula' if present.
    // This is O(n) where n is the number of cells that have dependents.

    CxHashmapIterator<CxSheetCellCoordinate, CxSList<CxSheetCellCoordinate> > iter(&dependents);

    while (iter.next()) {
        CxSList<CxSheetCellCoordinate>* depList = iter.getEntry();

        if (depList == NULL) {
            continue;
        }

        // Search for 'formula' in this list and remove it.
        for (int i = 0; i < (int)depList->entries(); i++) {
            if (depList->at(i) == formula) {
                depList->removeAt(i);
                break;  // Each cell appears at most once per list.
            }
        }
    }
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::getCellsToRecalculate
//
// THE MAIN ENTRY POINT for recalculation.
//
// Given that 'changedCell' has changed, return an ordered list of all
// cells that need to be recalculated, in the correct evaluation order.
//
// HOW IT WORKS:
// -------------
// 1. Find all cells affected by the change (direct and indirect dependents)
// 2. Topologically sort them so dependencies come before dependents
// 3. Return the sorted list
//
// The caller should then evaluate each cell in the returned order.
// This guarantees that when evaluating cell X, all cells that X depends on
// have already been updated.
//
// EXAMPLE:
// --------
// Given: A1=10, B1=A1*2, C1=B1+5, D1=A1+C1
// If A1 changes:
//   - Affected cells: {B1, C1, D1}
//   - Topological order: [B1, C1, D1]
//   - Returns: [B1, C1, D1]
//
// NOTE:
// -----
// The changed cell (A1) is NOT included in the returned list.
// It's assumed the caller has already updated A1's value.
//-------------------------------------------------------------------------
CxSList<CxSheetCellCoordinate>
CxSheetDependencyGraph::getCellsToRecalculate(CxSheetCellCoordinate changedCell)
{
    // Step 1: Find all cells affected by this change.
    // This does a breadth-first traversal of the dependents graph.
    CxSList<CxSheetCellCoordinate> affected = findAllAffectedCells(changedCell);

    // Step 2: Sort them in topological order.
    // This ensures proper evaluation order.
    CxSList<CxSheetCellCoordinate> sorted = topologicalSort(affected);

    return sorted;
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::getCellsToRecalculateMultiple
//
// Same as getCellsToRecalculate, but handles multiple changed cells.
// Useful for paste operations or initial spreadsheet load.
//-------------------------------------------------------------------------
CxSList<CxSheetCellCoordinate>
CxSheetDependencyGraph::getCellsToRecalculateMultiple(
    CxSList<CxSheetCellCoordinate> changedCells)
{
    // Find all cells affected by any of the changed cells.
    CxSList<CxSheetCellCoordinate> affected = findAllAffectedCellsMultiple(changedCells);

    // Sort them in topological order.
    CxSList<CxSheetCellCoordinate> sorted = topologicalSort(affected);

    return sorted;
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::getDependentCount
//
// Returns how many formulas directly depend on this cell.
// Useful for debugging and testing.
//-------------------------------------------------------------------------
int
CxSheetDependencyGraph::getDependentCount(CxSheetCellCoordinate cell)
{
    CxSList<CxSheetCellCoordinate>* depList =
        (CxSList<CxSheetCellCoordinate>*)dependents.find(cell);

    if (depList == NULL) {
        return 0;
    }

    return (int)depList->entries();
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::clear
//
// Remove all dependency information.
// Used when resetting/clearing the spreadsheet.
//-------------------------------------------------------------------------
void
CxSheetDependencyGraph::clear(void)
{
    // The hashmap doesn't have a clear() method, so we need to
    // iterate and remove entries. For now, we can just let
    // the model create a new graph if needed.
    //
    // TODO: Add a clear method if this becomes a performance issue.
    // For now, this is called rarely (only on reset).

    // Walk through and clear each list
    CxHashmapIterator<CxSheetCellCoordinate, CxSList<CxSheetCellCoordinate> > iter(&dependents);

    while (iter.next()) {
        CxSList<CxSheetCellCoordinate>* depList = iter.getEntry();
        if (depList != NULL) {
            depList->clear();
        }
    }
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::findAllAffectedCells
//
// PRIVATE HELPER: Find all cells affected by a change to 'startCell'.
//
// ALGORITHM:
// ----------
// We use breadth-first search (BFS) to traverse the dependency graph:
//
// 1. Start with the direct dependents of startCell (cells that reference startCell)
// 2. For each of those cells, add THEIR dependents to the queue
// 3. Continue until no more cells to process
// 4. Return the complete set of affected cells
//
// WHY BFS:
// --------
// BFS naturally handles the graph structure and avoids infinite loops
// (by tracking which cells we've already visited). It finds all reachable
// cells regardless of how deep the dependency chain goes.
//
// EXAMPLE:
// --------
// dependents[A1] = {B1, D1}
// dependents[B1] = {C1}
// dependents[C1] = {D1}
//
// findAllAffectedCells(A1):
//   - Queue starts with: [B1, D1]  (direct dependents of A1)
//   - Process B1: add C1 to result, queue: [D1, C1]
//   - Process D1: already visited (skip adding dependents twice)
//   - Process C1: D1 is dependent, but already in result
//   - Result: [B1, D1, C1]
//
// NOTE: The order of the result doesn't matter here - topological sort
// will fix the order later.
//-------------------------------------------------------------------------
CxSList<CxSheetCellCoordinate>
CxSheetDependencyGraph::findAllAffectedCells(CxSheetCellCoordinate startCell)
{
    CxSList<CxSheetCellCoordinate> result;  // All affected cells
    CxSList<CxSheetCellCoordinate> queue;   // Cells to process (BFS queue)

    // Start with direct dependents of the changed cell.
    CxSList<CxSheetCellCoordinate>* directDeps =
        (CxSList<CxSheetCellCoordinate>*)dependents.find(startCell);

    if (directDeps == NULL || directDeps->entries() == 0) {
        // No cells depend on this one - nothing to recalculate.
        return result;
    }

    // Add all direct dependents to both result and queue.
    for (int i = 0; i < (int)directDeps->entries(); i++) {
        CxSheetCellCoordinate dep = directDeps->at(i);
        result.append(dep);
        queue.append(dep);
    }

    // BFS: process each cell in the queue.
    while (queue.entries() > 0) {
        // Dequeue the first cell.
        CxSheetCellCoordinate current = queue.first();  // first() removes and returns

        // Find cells that depend on 'current'.
        CxSList<CxSheetCellCoordinate>* currentDeps =
            (CxSList<CxSheetCellCoordinate>*)dependents.find(current);

        if (currentDeps == NULL) {
            continue;  // No dependents - nothing more to add from this branch.
        }

        // Add each dependent to result (if not already there) and queue.
        for (int i = 0; i < (int)currentDeps->entries(); i++) {
            CxSheetCellCoordinate dep = currentDeps->at(i);

            // Only add if not already in result (avoid duplicates and cycles).
            if (!isInList(result, dep)) {
                result.append(dep);
                queue.append(dep);
            }
        }
    }

    return result;
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::findAllAffectedCellsMultiple
//
// PRIVATE HELPER: Same as findAllAffectedCells but for multiple start cells.
//-------------------------------------------------------------------------
CxSList<CxSheetCellCoordinate>
CxSheetDependencyGraph::findAllAffectedCellsMultiple(
    CxSList<CxSheetCellCoordinate> startCells)
{
    CxSList<CxSheetCellCoordinate> result;
    CxSList<CxSheetCellCoordinate> queue;

    // Initialize with direct dependents of ALL start cells.
    for (int s = 0; s < (int)startCells.entries(); s++) {
        CxSheetCellCoordinate startCell = startCells.at(s);
        CxSList<CxSheetCellCoordinate>* directDeps =
        (CxSList<CxSheetCellCoordinate>*)dependents.find(startCell);

        if (directDeps == NULL) {
            continue;
        }

        for (int i = 0; i < (int)directDeps->entries(); i++) {
            CxSheetCellCoordinate dep = directDeps->at(i);
            addToListIfNotPresent(result, dep);
            addToListIfNotPresent(queue, dep);
        }
    }

    // BFS from here is the same as single-cell version.
    while (queue.entries() > 0) {
        CxSheetCellCoordinate current = queue.first();

        CxSList<CxSheetCellCoordinate>* currentDeps =
            (CxSList<CxSheetCellCoordinate>*)dependents.find(current);

        if (currentDeps == NULL) {
            continue;
        }

        for (int i = 0; i < (int)currentDeps->entries(); i++) {
            CxSheetCellCoordinate dep = currentDeps->at(i);

            if (!isInList(result, dep)) {
                result.append(dep);
                queue.append(dep);
            }
        }
    }

    return result;
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::topologicalSort
//
// PRIVATE HELPER: Sort cells so that dependencies come before dependents.
//
// ALGORITHM: Kahn's Algorithm
// ---------------------------
// Kahn's algorithm is a standard approach for topological sorting:
//
// 1. BUILD IN-DEGREE MAP: For each cell in our set, count how many of
//    its dependencies are ALSO in the set. This is the "in-degree".
//    (We only count dependencies within our set, not external cells.)
//
// 2. FIND STARTING CELLS: Cells with in-degree 0 have no dependencies
//    in our set - they can be evaluated immediately.
//
// 3. PROCESS ITERATIVELY:
//    - Take a cell with in-degree 0, add it to the output.
//    - For each cell that depends on it, decrement their in-degree.
//    - If any cell reaches in-degree 0, it's ready to process.
//    - Repeat until done.
//
// EXAMPLE:
// --------
// Cells to sort: {B1, C1, D1}
// Dependencies: B1 depends on A1, C1 depends on B1, D1 depends on A1 and C1
//
// Step 1 - Calculate in-degrees (only counting deps within {B1, C1, D1}):
//   B1: depends on A1 (not in set) -> in-degree = 0
//   C1: depends on B1 (in set) -> in-degree = 1
//   D1: depends on C1 (in set), A1 (not in set) -> in-degree = 1
//
// Step 2 - Initial ready list: [B1]
//
// Step 3 - Process:
//   - Take B1, output = [B1]
//   - C1 depends on B1, so decrement C1's in-degree: 1->0, now ready
//   - Ready list: [C1]
//
//   - Take C1, output = [B1, C1]
//   - D1 depends on C1, so decrement D1's in-degree: 1->0, now ready
//   - Ready list: [D1]
//
//   - Take D1, output = [B1, C1, D1]
//   - Ready list empty, done!
//
// Result: [B1, C1, D1]
//
// CIRCULAR REFERENCES:
// --------------------
// If we can't process all cells (some always have in-degree > 0), there's
// a circular reference. We detect this and return what we have. The actual
// circular reference error is reported during formula evaluation.
//-------------------------------------------------------------------------
CxSList<CxSheetCellCoordinate>
CxSheetDependencyGraph::topologicalSort(CxSList<CxSheetCellCoordinate> cells)
{
    CxSList<CxSheetCellCoordinate> result;

    if (cells.entries() == 0) {
        return result;  // Nothing to sort.
    }

    //---------------------------------------------------------------------
    // STEP 1: Calculate in-degrees for each cell.
    //
    // For each cell, count how many of its dependencies are in our set.
    // We use a parallel array approach since we don't want to modify the cells.
    //---------------------------------------------------------------------

    int cellCount = (int)cells.entries();

    // Allocate array for in-degrees (one entry per cell in our set).
    int* inDegree = (int*)malloc(cellCount * sizeof(int));
    if (inDegree == NULL) {
        // Memory allocation failed - return unsorted as fallback.
        return cells;
    }

    // Initialize all in-degrees to 0.
    for (int i = 0; i < cellCount; i++) {
        inDegree[i] = 0;
    }

    // For each cell, look at what depends on it (its dependents).
    // If the dependent is in our set, increment that dependent's in-degree.
    //
    // This is a bit backwards from the typical explanation, but it works:
    // If B1 is in dependents[A1], and both are in our set,
    // then B1 depends on A1, so B1's in-degree should include this.

    for (int i = 0; i < cellCount; i++) {
        CxSheetCellCoordinate cell = cells.at(i);

        // Look up who depends on 'cell'.
        CxSList<CxSheetCellCoordinate>* cellDeps =
            (CxSList<CxSheetCellCoordinate>*)dependents.find(cell);

        if (cellDeps == NULL) {
            continue;
        }

        // For each dependent of 'cell'...
        for (int j = 0; j < (int)cellDeps->entries(); j++) {
            CxSheetCellCoordinate dep = cellDeps->at(j);

            // If this dependent is in our set, increment its in-degree.
            // (It has one more dependency within our set.)
            for (int k = 0; k < cellCount; k++) {
                if (cells.at(k) == dep) {
                    inDegree[k]++;
                    break;
                }
            }
        }
    }

    //---------------------------------------------------------------------
    // STEP 2: Initialize queue with cells that have in-degree 0.
    // These cells have no dependencies within our set.
    //---------------------------------------------------------------------

    CxSList<int> readyQueue;  // Indices of cells ready to process

    for (int i = 0; i < cellCount; i++) {
        if (inDegree[i] == 0) {
            readyQueue.append(i);
        }
    }

    //---------------------------------------------------------------------
    // STEP 3: Process cells in topological order.
    //---------------------------------------------------------------------

    while (readyQueue.entries() > 0) {
        // Get next ready cell index.
        int currentIdx = readyQueue.first();  // first() removes and returns
        CxSheetCellCoordinate current = cells.at(currentIdx);

        // Add to result - this cell can be evaluated now.
        result.append(current);

        // For each cell that depends on 'current', decrement its in-degree.
        CxSList<CxSheetCellCoordinate>* currentDeps =
            (CxSList<CxSheetCellCoordinate>*)dependents.find(current);

        if (currentDeps != NULL) {
            for (int j = 0; j < (int)currentDeps->entries(); j++) {
                CxSheetCellCoordinate dep = currentDeps->at(j);

                // Find this dependent in our set.
                for (int k = 0; k < cellCount; k++) {
                    if (cells.at(k) == dep) {
                        inDegree[k]--;

                        // If in-degree reaches 0, this cell is now ready.
                        if (inDegree[k] == 0) {
                            readyQueue.append(k);
                        }
                        break;
                    }
                }
            }
        }
    }

    //---------------------------------------------------------------------
    // STEP 4: Check for unprocessed cells (circular references).
    //---------------------------------------------------------------------

    if ((int)result.entries() < cellCount) {
        // Some cells couldn't be processed - they're in a cycle.
        // Add them to the result anyway (they'll error during evaluation).
        // This ensures we don't silently skip cells.

        for (int i = 0; i < cellCount; i++) {
            CxSheetCellCoordinate cell = cells.at(i);
            if (!isInList(result, cell)) {
                result.append(cell);
            }
        }
    }

    // Clean up.
    free(inDegree);

    return result;
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::isInList
//
// PRIVATE HELPER: Check if a coordinate is in a list.
// Returns 1 if found, 0 if not.
//-------------------------------------------------------------------------
int
CxSheetDependencyGraph::isInList(CxSList<CxSheetCellCoordinate>& list,
                                  CxSheetCellCoordinate coord)
{
    for (int i = 0; i < (int)list.entries(); i++) {
        if (list.at(i) == coord) {
            return 1;
        }
    }
    return 0;
}


//-------------------------------------------------------------------------
// CxSheetDependencyGraph::addToListIfNotPresent
//
// PRIVATE HELPER: Add coordinate to list only if not already present.
//-------------------------------------------------------------------------
void
CxSheetDependencyGraph::addToListIfNotPresent(CxSList<CxSheetCellCoordinate>& list,
                                               CxSheetCellCoordinate coord)
{
    if (!isInList(list, coord)) {
        list.append(coord);
    }
}
