//-------------------------------------------------------------------------------------------------
//
//  completer.h
//
//  Hierarchical command completion with fuzzy prefix matching.
//
//  This module provides a tree-based completion system where each level can have
//  candidates that either complete to a value or transition to a child completer.
//
//  Features:
//  - Fuzzy prefix matching (hyphens ignored): "gl" matches "goto-line"
//  - Acronym matching: "bl" matches "buffer-list"
//  - Auto-completion to common prefix when all matches share one
//  - Support for filtered display names (show "horizontal" instead of "box-horizontal")
//  - Child completers for multi-level command sequences
//
//  Created by Todd Vernon on 2/4/26.
//  Copyright (c) 2026 Todd Vernon. All rights reserved.
//
//-------------------------------------------------------------------------------------------------

#ifndef _Completer_h_
#define _Completer_h_

#include <cx/base/string.h>

//-------------------------------------------------------------------------------------------------
// Forward declarations
//-------------------------------------------------------------------------------------------------
class Completer;

//-------------------------------------------------------------------------------------------------
// CompleterStatus - tells you where you are in the completion process
//-------------------------------------------------------------------------------------------------
enum CompleterStatus
{
    COMPLETER_NO_MATCH,         // No matches for current input
    COMPLETER_MULTIPLE,         // Multiple matches - show hints to user
    COMPLETER_PARTIAL,          // Auto-completed to common prefix, still multiple matches
    COMPLETER_UNIQUE,           // Single match, auto-completed, awaiting ENTER
    COMPLETER_NEXT_LEVEL,       // Single match with child completer - transition now
    COMPLETER_SELECTED          // Selection confirmed (from processEnter)
};

//-------------------------------------------------------------------------------------------------
// CompleterCandidate - a single completion candidate
//
// Each candidate has:
//   - name: the name used for matching and display
//   - childCompleter: if non-NULL, selecting this transitions to a child level
//   - userData: application-specific data pointer
//-------------------------------------------------------------------------------------------------
struct CompleterCandidate
{
    CxString name;              // name for matching and display
    Completer *childCompleter;  // next level, or NULL if this is a leaf
    void *userData;             // application-specific payload
};

//-------------------------------------------------------------------------------------------------
// CompleterResult - result of processing input
//
// Use accessor methods to query results:
//   getStatus()       - what happened (the main thing to check)
//   getInput()        - updated input after auto-completion
//   getMatchCount()   - number of current matches
//   getNextLevel()    - child completer when status == COMPLETER_NEXT_LEVEL
//   getSelectedData() - userData of selected candidate
//   getSelectedName() - full name of selected candidate
//-------------------------------------------------------------------------------------------------
class CompleterResult
{
public:
    CompleterResult();

    // Accessors
    CompleterStatus getStatus() const       { return _status; }
    CxString getInput() const               { return _input; }
    int getMatchCount() const               { return _matchCount; }
    Completer *getNextLevel() const         { return _nextLevel; }
    void *getSelectedData() const           { return _selectedData; }
    CxString getSelectedName() const        { return _selectedName; }

private:
    friend class Completer;

    CompleterStatus _status;
    CxString _input;
    int _matchCount;
    Completer *_nextLevel;
    void *_selectedData;
    CxString _selectedName;
};

//-------------------------------------------------------------------------------------------------
// Completer - manages completion for one level of a command tree
//
// Usage:
//   Completer commands;
//   commands.addCandidate("goto-line", "", NULL, &gotoHandler);
//   commands.addCandidate("utf-box", "", &boxSymbols, NULL);
//
//   CompleterResult r = commands.processChar("", 'g');
//   // r.input = "goto-line" (auto-completed, single match)
//   // r.matchCount = 1
//   // r.nextLevel = NULL (goto-line has no child, but needs argument handling elsewhere)
//
//-------------------------------------------------------------------------------------------------
class Completer
{
public:
    Completer();
    ~Completer();

    //---------------------------------------------------------------------------------------------
    // Setup
    //---------------------------------------------------------------------------------------------

    void addCandidate( CxString name, Completer *childCompleter = NULL, void *userData = NULL );
    // Add a completion candidate.
    // - name: the name used for matching and display
    // - childCompleter: if selecting this should transition to another level
    // - userData: application-specific data

    void clear();
    // Remove all candidates

    //---------------------------------------------------------------------------------------------
    // Core completion operations
    //---------------------------------------------------------------------------------------------

    CompleterResult processChar( CxString currentInput, char c );
    // Process a single character input.
    // Returns updated input (possibly auto-completed) and match info.
    // If a single match with a childCompleter is found, nextLevel is set.

    CompleterResult processEnter( CxString currentInput );
    // Process ENTER key - attempt to select current match.
    // Returns selectedData/selectedName if a unique match exists.
    // Returns nextLevel if selected candidate has a child completer.

    CompleterResult processTab( CxString currentInput );
    // Process TAB key - complete to common prefix.
    // Same behavior as auto-completion in processChar, but explicit.

    //---------------------------------------------------------------------------------------------
    // Query operations
    //---------------------------------------------------------------------------------------------

    int findMatches( CxString input, CxString *names, int maxMatches );
    // Get names of all candidates matching input.
    // Returns count of matches found.

    int findMatchesFull( CxString input, CompleterCandidate **matches, int maxMatches );
    // Get full candidate info for all matches.
    // Returns count of matches found.

    CxString complete( CxString input );
    // Return input extended to common prefix of all matches.
    // If single match, returns full candidate name.
    // If no matches, returns input unchanged.

    int getCandidateCount();
    // Total number of candidates

    CompleterCandidate *getCandidateAt( int index );
    // Get candidate by index (for iteration)

    //---------------------------------------------------------------------------------------------
    // Matching utilities (public for testing)
    //---------------------------------------------------------------------------------------------

    static CxString dehyphenate( CxString s );
    // Remove hyphens from string

    static int matchesPrefix( CxString candidateName, CxString userInput );
    // Fuzzy prefix match (dehyphenated comparison)

    static CxString findCommonPrefix( CxString *names, int count );
    // Find longest common prefix (literal, preserves hyphens)

private:
    CompleterCandidate *_candidates;
    int _candidateCount;
    int _candidateCapacity;

    void grow();
};

#endif // _Completer_h_
