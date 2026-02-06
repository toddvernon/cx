/***************************************************************************
 *
 *  regex.cpp
 *
 *  CxRegex Class - POSIX regex wrapper implementation
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

#if defined(_LINUX_) || defined(_OSX_)

#include <stdio.h>
#include <string.h>
#include <cx/regex/regex.h>

//-------------------------------------------------------------------------
// CxRegex::CxRegex
//-------------------------------------------------------------------------
CxRegex::CxRegex()
: _isCompiled(0), _errorMsg("")
{
    memset(&_compiled, 0, sizeof(_compiled));
}


//-------------------------------------------------------------------------
// CxRegex::~CxRegex
//-------------------------------------------------------------------------
CxRegex::~CxRegex()
{
    reset();
}


//-------------------------------------------------------------------------
// CxRegex::compile
//
// Compile the regex pattern. Returns 0 on success, error code on failure.
//-------------------------------------------------------------------------
int
CxRegex::compile(CxString pattern, int caseInsensitive)
{
    // Free any previously compiled pattern
    reset();

    // Set up flags
    int flags = REG_EXTENDED;
    if (caseInsensitive) {
        flags |= REG_ICASE;
    }

    // Compile the pattern
    int result = regcomp(&_compiled, pattern.data(), flags);

    if (result == 0) {
        _isCompiled = 1;
        _errorMsg = "";
    } else {
        _isCompiled = 0;
        // Get error message
        char errBuf[256];
        regerror(result, &_compiled, errBuf, sizeof(errBuf));
        _errorMsg = errBuf;
    }

    return result;
}


//-------------------------------------------------------------------------
// CxRegex::isCompiled
//-------------------------------------------------------------------------
int
CxRegex::isCompiled()
{
    return _isCompiled;
}


//-------------------------------------------------------------------------
// CxRegex::match (simple version)
//
// Returns 1 if pattern matches anywhere in text, 0 otherwise.
//-------------------------------------------------------------------------
int
CxRegex::match(CxString text)
{
    return match(text, NULL, NULL);
}


//-------------------------------------------------------------------------
// CxRegex::match (with position)
//
// Returns 1 if pattern matches, sets matchStart and matchLen if provided.
//-------------------------------------------------------------------------
int
CxRegex::match(CxString text, int *matchStart, int *matchLen)
{
    if (!_isCompiled) {
        return 0;
    }

    regmatch_t pmatch[1];
    int result = regexec(&_compiled, text.data(), 1, pmatch, 0);

    if (result == 0) {
        // Match found
        if (matchStart != NULL) {
            *matchStart = (int)pmatch[0].rm_so;
        }
        if (matchLen != NULL) {
            *matchLen = (int)(pmatch[0].rm_eo - pmatch[0].rm_so);
        }
        return 1;
    }

    return 0;
}


//-------------------------------------------------------------------------
// CxRegex::getError
//-------------------------------------------------------------------------
CxString
CxRegex::getError()
{
    return _errorMsg;
}


//-------------------------------------------------------------------------
// CxRegex::reset
//-------------------------------------------------------------------------
void
CxRegex::reset()
{
    if (_isCompiled) {
        regfree(&_compiled);
        _isCompiled = 0;
    }
    _errorMsg = "";
    memset(&_compiled, 0, sizeof(_compiled));
}


//-------------------------------------------------------------------------
// CxRegex::escapeForLiteral
//
// Escape special regex characters so they match literally.
// Special chars: . * + ? ^ $ [ ] { } ( ) | \
//-------------------------------------------------------------------------
CxString
CxRegex::escapeForLiteral(CxString text)
{
    CxString result = "";
    const char *specialChars = ".*+?^$[]{}()|\\";

    for (unsigned long i = 0; i < text.length(); i++) {
        char c = text.charAt(i);
        // Check if this is a special character
        if (strchr(specialChars, c) != NULL) {
            result.append('\\');
        }
        result.append(c);
    }

    return result;
}


//-------------------------------------------------------------------------
// regexReplace
//
// Replace first match of pattern in text.
//-------------------------------------------------------------------------
CxString
regexReplace(CxString text, CxString pattern, CxString replacement, int caseInsensitive)
{
    CxRegex regex;
    if (regex.compile(pattern, caseInsensitive) != 0) {
        return text;  // Invalid pattern, return original
    }

    int matchStart, matchLen;
    if (!regex.match(text, &matchStart, &matchLen)) {
        return text;  // No match, return original
    }

    // Build result: before + replacement + after
    CxString result = "";

    // Part before match
    if (matchStart > 0) {
        result = text.subString(0, matchStart);
    }

    // Replacement
    result.append(replacement);

    // Part after match
    int afterStart = matchStart + matchLen;
    if ((unsigned long)afterStart < text.length()) {
        result.append(text.subString(afterStart, text.length() - afterStart));
    }

    return result;
}


//-------------------------------------------------------------------------
// regexReplaceAll
//
// Replace all matches of pattern in text.
//-------------------------------------------------------------------------
CxString
regexReplaceAll(CxString text, CxString pattern, CxString replacement, int caseInsensitive)
{
    CxRegex regex;
    if (regex.compile(pattern, caseInsensitive) != 0) {
        return text;  // Invalid pattern, return original
    }

    CxString result = "";
    CxString remaining = text;
    int matchStart, matchLen;

    while (regex.match(remaining, &matchStart, &matchLen)) {
        // Add part before match
        if (matchStart > 0) {
            result.append(remaining.subString(0, matchStart));
        }

        // Add replacement
        result.append(replacement);

        // Move past the match
        int afterStart = matchStart + matchLen;
        if ((unsigned long)afterStart >= remaining.length()) {
            remaining = "";
            break;
        }
        remaining = remaining.subString(afterStart, remaining.length() - afterStart);

        // Safety check to avoid infinite loop on zero-length matches
        if (matchLen == 0) {
            if (remaining.length() > 0) {
                result.append(remaining.charAt(0));
                remaining = remaining.subString(1, remaining.length() - 1);
            } else {
                break;
            }
        }
    }

    // Add any remaining text
    result.append(remaining);

    return result;
}

#endif // _LINUX_ || _OSX_
