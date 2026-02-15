//-------------------------------------------------------------------------------------------------
//
//  process.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  process.cpp
//
//-------------------------------------------------------------------------------------------------

#include "process.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(_LINUX_) || defined(_OSX_) || defined(_SUNOS_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_IRIX6_) || defined(_NETBSD_) || defined(_NEXT_)
#include <sys/wait.h>
#endif


//-------------------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------------------
CxProcess::CxProcess()
    : _exitCode(-1)
{
}


//-------------------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------------------
CxProcess::~CxProcess()
{
}


//-------------------------------------------------------------------------------------------------
// Run a command and capture output
//-------------------------------------------------------------------------------------------------
int
CxProcess::run(const char *command)
{
    _output = "";
    _exitCode = -1;

    if (command == NULL || command[0] == '\0') {
        return -1;
    }

    // Redirect stderr to stdout so we capture both
    CxString fullCommand = command;
    fullCommand += " 2>&1";

    FILE *pipe = popen(fullCommand.data(), "r");
    if (pipe == NULL) {
        return -1;
    }

    // Read output
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        _output += buffer;
    }

    // Get exit status
    int status = pclose(pipe);
#if defined(_LINUX_) || defined(_OSX_) || defined(_SUNOS_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_IRIX6_) || defined(_NETBSD_) || defined(_NEXT_)
    if (WIFEXITED(status)) {
        _exitCode = WEXITSTATUS(status);
    } else {
        _exitCode = -1;
    }
#else
    _exitCode = status;
#endif

    return 0;
}


//-------------------------------------------------------------------------------------------------
// Run (CxString version)
//-------------------------------------------------------------------------------------------------
int
CxProcess::run(CxString command)
{
    return run(command.data());
}


//-------------------------------------------------------------------------------------------------
// Get captured output
//-------------------------------------------------------------------------------------------------
CxString
CxProcess::getOutput(void)
{
    return _output;
}


//-------------------------------------------------------------------------------------------------
// Get exit code
//-------------------------------------------------------------------------------------------------
int
CxProcess::getExitCode(void)
{
    return _exitCode;
}


//-------------------------------------------------------------------------------------------------
// Parse a line for build error pattern
//
// Recognizes:
//   file.cpp:123:45: error: message    (GCC/Clang with column)
//   file.cpp:123: error: message       (GCC/Clang without column)
//   file:123: message                  (generic)
//   /path/to/file.cpp:123:45: message  (absolute path)
//
// Returns CxBuildError with valid=1 if parsed successfully
//-------------------------------------------------------------------------------------------------
CxBuildError
CxProcess::parseBuildError(CxString line)
{
    CxBuildError result;

    // Look for the pattern: filename:line: or filename:line:col:
    // The filename can contain path separators but not colons (on Unix)

    int len = line.length();
    if (len == 0) {
        return result;
    }

    // Find first colon that's followed by a digit (the line number)
    int firstColon = -1;
    for (int i = 0; i < len - 1; i++) {
        if (line.charAt(i) == ':') {
            char next = line.charAt(i + 1);
            if (next >= '0' && next <= '9') {
                firstColon = i;
                break;
            }
        }
    }

    if (firstColon <= 0) {
        // No valid pattern found (colon at start or not found)
        return result;
    }

    // Extract filename
    result.filename = line.subString(0, firstColon);

    // Parse line number starting after the first colon
    int lineStart = firstColon + 1;
    int lineNum = 0;
    int i = lineStart;

    while (i < len) {
        char c = line.charAt(i);
        if (c >= '0' && c <= '9') {
            lineNum = lineNum * 10 + (c - '0');
            i++;
        } else {
            break;
        }
    }

    if (lineNum == 0) {
        // No valid line number
        result.filename = "";
        return result;
    }

    result.line = lineNum;

    // Check for column number (another colon followed by digits)
    if (i < len && line.charAt(i) == ':') {
        i++;  // skip colon
        int colNum = 0;
        while (i < len) {
            char c = line.charAt(i);
            if (c >= '0' && c <= '9') {
                colNum = colNum * 10 + (c - '0');
                i++;
            } else {
                break;
            }
        }
        if (colNum > 0) {
            result.column = colNum;
        }
    }

    // Skip colon and space after line/column
    while (i < len && (line.charAt(i) == ':' || line.charAt(i) == ' ')) {
        i++;
    }

    // Rest is the message
    if (i < len) {
        result.message = line.subString(i, len - i);
    }

    result.valid = 1;
    return result;
}
