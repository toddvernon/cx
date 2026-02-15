//-------------------------------------------------------------------------------------------------
//
//  completer.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  Hierarchical command completion with literal prefix matching.
//
//-------------------------------------------------------------------------------------------------

#include "completer.h"
#include <stdlib.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//-------------------------------------------------------------------------------------------------
// CompleterResult::CompleterResult
//-------------------------------------------------------------------------------------------------
CompleterResult::CompleterResult()
{
    _status = COMPLETER_NO_MATCH;
    _matchCount = 0;
    _nextLevel = NULL;
    _selectedData = NULL;
}


//-------------------------------------------------------------------------------------------------
// Completer::Completer
//-------------------------------------------------------------------------------------------------
Completer::Completer()
{
    _candidates = NULL;
    _candidateCount = 0;
    _candidateCapacity = 0;
}


//-------------------------------------------------------------------------------------------------
// Completer::~Completer
//-------------------------------------------------------------------------------------------------
Completer::~Completer()
{
    clear();
}


//-------------------------------------------------------------------------------------------------
// Completer::clear
//-------------------------------------------------------------------------------------------------
void
Completer::clear()
{
    if (_candidates != NULL) {
        delete[] _candidates;
        _candidates = NULL;
    }
    _candidateCount = 0;
    _candidateCapacity = 0;
}


//-------------------------------------------------------------------------------------------------
// Completer::grow
//-------------------------------------------------------------------------------------------------
void
Completer::grow()
{
    int newCapacity = (_candidateCapacity == 0) ? 16 : _candidateCapacity * 2;
    CompleterCandidate *newCandidates = new CompleterCandidate[newCapacity];

    for (int i = 0; i < _candidateCount; i++) {
        newCandidates[i] = _candidates[i];
    }

    if (_candidates != NULL) {
        delete[] _candidates;
    }

    _candidates = newCandidates;
    _candidateCapacity = newCapacity;
}


//-------------------------------------------------------------------------------------------------
// Completer::addCandidate
//-------------------------------------------------------------------------------------------------
void
Completer::addCandidate( CxString name, Completer *childCompleter, void *userData )
{
    if (_candidateCount >= _candidateCapacity) {
        grow();
    }

    CompleterCandidate &c = _candidates[_candidateCount];
    c.name = name;
    c.childCompleter = childCompleter;
    c.userData = userData;

    _candidateCount++;
}


//-------------------------------------------------------------------------------------------------
// Completer::getCandidateCount
//-------------------------------------------------------------------------------------------------
int
Completer::getCandidateCount()
{
    return _candidateCount;
}


//-------------------------------------------------------------------------------------------------
// Completer::getCandidateAt
//-------------------------------------------------------------------------------------------------
CompleterCandidate *
Completer::getCandidateAt( int index )
{
    if (index < 0 || index >= _candidateCount) {
        return NULL;
    }
    return &_candidates[index];
}


//-------------------------------------------------------------------------------------------------
// Completer::matchesPrefix
//
// Literal prefix match - user input matches if it's a prefix of the candidate name.
//-------------------------------------------------------------------------------------------------
int
Completer::matchesPrefix( CxString candidateName, CxString userInput )
{
    if (userInput.length() > candidateName.length()) {
        return FALSE;
    }

    for (unsigned long i = 0; i < userInput.length(); i++) {
        if (userInput.charAt( (int) i ) != candidateName.charAt( (int) i )) {
            return FALSE;
        }
    }

    return TRUE;
}



//-------------------------------------------------------------------------------------------------
// Completer::findCommonPrefix
//
// Find longest common prefix among names (literal comparison, preserves hyphens).
//-------------------------------------------------------------------------------------------------
CxString
Completer::findCommonPrefix( CxString *names, int count )
{
    if (count == 0) {
        return "";
    }

    if (count == 1) {
        return names[0];
    }

    CxString first = names[0];
    unsigned long commonLen = first.length();

    for (int i = 1; i < count; i++) {
        CxString other = names[i];
        unsigned long len = 0;

        while (len < commonLen && len < other.length() &&
               first.charAt( (int) len ) == other.charAt( (int) len )) {
            len++;
        }

        commonLen = len;
    }

    return first.subString( 0, commonLen );
}


//-------------------------------------------------------------------------------------------------
// Completer::findMatches
//
// Find all candidates matching input, return names.
//-------------------------------------------------------------------------------------------------
int
Completer::findMatches( CxString input, CxString *names, int maxMatches )
{
    int count = 0;

    for (int i = 0; i < _candidateCount && count < maxMatches; i++) {
        if (matchesPrefix( _candidates[i].name, input )) {
            names[count++] = _candidates[i].name;
        }
    }

    return count;
}


//-------------------------------------------------------------------------------------------------
// Completer::findMatchesFull
//
// Find all candidates matching input, return full candidate info.
//-------------------------------------------------------------------------------------------------
int
Completer::findMatchesFull( CxString input, CompleterCandidate **matches, int maxMatches )
{
    int count = 0;

    for (int i = 0; i < _candidateCount && count < maxMatches; i++) {
        if (matchesPrefix( _candidates[i].name, input )) {
            matches[count++] = &_candidates[i];
        }
    }

    return count;
}


//-------------------------------------------------------------------------------------------------
// Completer::complete
//
// Return input extended to common prefix of all matches.
//-------------------------------------------------------------------------------------------------
CxString
Completer::complete( CxString input )
{
    CompleterCandidate *matches[64];
    int count = findMatchesFull( input, matches, 64 );

    if (count == 0) {
        return input;
    }

    if (count == 1) {
        return matches[0]->name;
    }

    // Collect names for common prefix
    CxString names[64];
    for (int i = 0; i < count; i++) {
        names[i] = matches[i]->name;
    }

    CxString common = findCommonPrefix( names, count );

    // Only return common if it extends input
    if (common.length() > input.length()) {
        return common;
    }

    return input;
}


//-------------------------------------------------------------------------------------------------
// Completer::processChar
//
// Process a single character, auto-complete if possible.
//-------------------------------------------------------------------------------------------------
CompleterResult
Completer::processChar( CxString currentInput, char c )
{
    CompleterResult result;

    // Add character to input
    CxString newInput = currentInput;
    newInput += c;

    // Find matches
    CompleterCandidate *matches[64];
    int count = findMatchesFull( newInput, matches, 64 );

    if (count == 0) {
        // No matches - return input unchanged (or could return newInput to show typo)
        result._status = COMPLETER_NO_MATCH;
        result._input = newInput;
        result._matchCount = 0;
        return result;
    }

    if (count == 1) {
        // Single match - auto-complete to full name
        result._input = matches[0]->name;
        result._matchCount = 1;
        result._selectedData = matches[0]->userData;
        result._selectedName = matches[0]->name;

        // If this candidate has a child, signal transition
        if (matches[0]->childCompleter != NULL) {
            result._status = COMPLETER_NEXT_LEVEL;
            result._nextLevel = matches[0]->childCompleter;
        } else {
            result._status = COMPLETER_UNIQUE;
        }

        return result;
    }

    // Multiple matches - auto-complete to common prefix
    CxString names[64];
    for (int i = 0; i < count; i++) {
        names[i] = matches[i]->name;
    }

    CxString common = findCommonPrefix( names, count );
    if (common.length() > newInput.length()) {
        result._status = COMPLETER_PARTIAL;
        result._input = common;
    } else {
        result._status = COMPLETER_MULTIPLE;
        result._input = newInput;
    }

    result._matchCount = count;
    return result;
}


//-------------------------------------------------------------------------------------------------
// Completer::processTab
//
// Explicit completion - same as auto-complete in processChar.
//-------------------------------------------------------------------------------------------------
CompleterResult
Completer::processTab( CxString currentInput )
{
    CompleterResult result;

    CompleterCandidate *matches[64];
    int count = findMatchesFull( currentInput, matches, 64 );

    if (count == 0) {
        result._status = COMPLETER_NO_MATCH;
        result._input = currentInput;
        result._matchCount = 0;
        return result;
    }

    if (count == 1) {
        result._input = matches[0]->name;
        result._matchCount = 1;
        result._selectedData = matches[0]->userData;
        result._selectedName = matches[0]->name;

        if (matches[0]->childCompleter != NULL) {
            result._status = COMPLETER_NEXT_LEVEL;
            result._nextLevel = matches[0]->childCompleter;
        } else {
            result._status = COMPLETER_UNIQUE;
        }

        return result;
    }

    // Multiple matches - complete to common prefix
    CxString names[64];
    for (int i = 0; i < count; i++) {
        names[i] = matches[i]->name;
    }

    CxString common = findCommonPrefix( names, count );
    if (common.length() > currentInput.length()) {
        result._status = COMPLETER_PARTIAL;
        result._input = common;
    } else {
        result._status = COMPLETER_MULTIPLE;
        result._input = currentInput;
    }

    result._matchCount = count;
    return result;
}


//-------------------------------------------------------------------------------------------------
// Completer::processEnter
//
// Attempt to select current match.
//-------------------------------------------------------------------------------------------------
CompleterResult
Completer::processEnter( CxString currentInput )
{
    CompleterResult result;
    result._input = currentInput;

    // First check for exact match
    for (int i = 0; i < _candidateCount; i++) {
        if (_candidates[i].name == currentInput) {
            result._status = COMPLETER_SELECTED;
            result._matchCount = 1;
            result._selectedData = _candidates[i].userData;
            result._selectedName = _candidates[i].name;

            if (_candidates[i].childCompleter != NULL) {
                result._nextLevel = _candidates[i].childCompleter;
            }

            return result;
        }
    }

    // No exact match - try unique prefix match
    CompleterCandidate *matches[64];
    int count = findMatchesFull( currentInput, matches, 64 );

    result._matchCount = count;

    if (count == 0) {
        result._status = COMPLETER_NO_MATCH;
    } else if (count == 1) {
        result._status = COMPLETER_SELECTED;
        result._selectedData = matches[0]->userData;
        result._selectedName = matches[0]->name;
        result._input = matches[0]->name;

        if (matches[0]->childCompleter != NULL) {
            result._nextLevel = matches[0]->childCompleter;
        }
    } else {
        result._status = COMPLETER_MULTIPLE;
    }

    return result;
}
