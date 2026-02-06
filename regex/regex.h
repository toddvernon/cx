/***************************************************************************
 *
 *  regex.h
 *
 *  CxRegex Class - POSIX regex wrapper
 *
 ***************************************************************************
 *
 * (c) Copyright 2024 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, T.Vernon , which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/

#ifndef _CxREGEX_H_
#define _CxREGEX_H_

#if defined(_LINUX_) || defined(_OSX_)

#include <regex.h>
#include <cx/base/string.h>

//-------------------------------------------------------------------------
// class CxRegex
//
// Wraps POSIX regex for pattern matching and replacement.
// Linux and macOS only.
//-------------------------------------------------------------------------
class CxRegex
{
public:

    CxRegex();
    // Constructor

    ~CxRegex();
    // Destructor

    int compile(CxString pattern, int caseInsensitive = 0);
    // Compile pattern. Returns 0 on success, non-zero on failure.

    int isCompiled();
    // Returns true (1) if pattern compiled successfully.

    int match(CxString text);
    // Returns true (1) if text matches the compiled pattern anywhere.

    int match(CxString text, int *matchStart, int *matchLen);
    // Returns true (1) if match found, sets start position and length.
    // matchStart and matchLen may be NULL if not needed.

    CxString getError();
    // Returns error message if compile() failed.

    void reset();
    // Free compiled pattern and reset state.

    static CxString escapeForLiteral(CxString text);
    // Escape special regex characters for literal matching.
    // Special chars: . * + ? ^ $ [ ] { } ( ) | \

private:

    regex_t _compiled;
    int _isCompiled;
    CxString _errorMsg;
};


//-------------------------------------------------------------------------
// Utility functions
//-------------------------------------------------------------------------

CxString regexReplace(CxString text, CxString pattern, CxString replacement, int caseInsensitive = 0);
// Replace first match of pattern in text with replacement.
// Returns modified text, or original if no match.

CxString regexReplaceAll(CxString text, CxString pattern, CxString replacement, int caseInsensitive = 0);
// Replace all matches of pattern in text with replacement.
// Returns modified text, or original if no matches.

#endif // _LINUX_ || _OSX_

#endif // _CxREGEX_H_
