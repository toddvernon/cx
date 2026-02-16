//-------------------------------------------------------------------------------------------------
//
//  buildoutput.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  buildoutput.cpp
//
//-------------------------------------------------------------------------------------------------

#include "buildoutput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#if defined(_LINUX_) || defined(_OSX_) || defined(_SUNOS_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_IRIX6_) || defined(_NETBSD_) || defined(_NEXT_)
#include <sys/wait.h>
#include <unistd.h>
#endif

//-------------------------------------------------------------------------
// SunOS 4.x doesn't have ssize_t - define it
// Also needs extern "C" declaration for pclose
//-------------------------------------------------------------------------
#if defined(_SUNOS_)
typedef long ssize_t;
extern "C" {
int pclose(FILE *stream);
}
#endif


//-------------------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------------------
BuildOutput::BuildOutput()
    : _state(BUILD_IDLE)
    , _exitCode(-1)
    , _errorCount(0)
    , _warningCount(0)
    , _pipe(NULL)
    , _fd(-1)
{
}


//-------------------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------------------
BuildOutput::~BuildOutput()
{
    if (_pipe != NULL) {
        pclose(_pipe);
        _pipe = NULL;
    }
}


//-------------------------------------------------------------------------------------------------
// start
//
// Start a build command in non-blocking mode.
//-------------------------------------------------------------------------------------------------
int
BuildOutput::start(CxString command)
{
    // Clear any previous state
    clear();

    if (command.length() == 0) {
        return -1;
    }

    _command = command;

    // Redirect stderr to stdout so we capture both
    CxString fullCommand = command;
    fullCommand += " 2>&1";

    // Open pipe to command
    _pipe = popen(fullCommand.data(), "r");
    if (_pipe == NULL) {
        _state = BUILD_ERROR;
        _exitCode = -1;
        return -1;
    }

    // Get file descriptor
    _fd = fileno(_pipe);
    if (_fd < 0) {
        pclose(_pipe);
        _pipe = NULL;
        _state = BUILD_ERROR;
        _exitCode = -1;
        return -1;
    }

    // Set non-blocking mode
    int flags = fcntl(_fd, F_GETFL, 0);
    if (flags != -1) {
        fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
    }

    _state = BUILD_RUNNING;
    return 0;
}


//-------------------------------------------------------------------------------------------------
// startNext
//
// Start a new build command without clearing previous output.
// Used for build-all to accumulate output from multiple subprojects.
// Keeps existing lines, error/warning counts. Resets pipe state only.
//-------------------------------------------------------------------------------------------------
int
BuildOutput::startNext(CxString command)
{
    if (command.length() == 0) {
        return -1;
    }

    _command = command;
    _exitCode = -1;
    _partialLine = "";

    // Redirect stderr to stdout so we capture both
    CxString fullCommand = command;
    fullCommand += " 2>&1";

    // Open pipe to command
    _pipe = popen(fullCommand.data(), "r");
    if (_pipe == NULL) {
        _state = BUILD_ERROR;
        _exitCode = -1;
        return -1;
    }

    // Get file descriptor
    _fd = fileno(_pipe);
    if (_fd < 0) {
        pclose(_pipe);
        _pipe = NULL;
        _state = BUILD_ERROR;
        _exitCode = -1;
        return -1;
    }

    // Set non-blocking mode
    int flags = fcntl(_fd, F_GETFL, 0);
    if (flags != -1) {
        fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
    }

    _state = BUILD_RUNNING;
    return 0;
}


//-------------------------------------------------------------------------------------------------
// poll
//
// Non-blocking read from the build process.
// Returns 1 if new lines were added, 0 otherwise.
//-------------------------------------------------------------------------------------------------
int
BuildOutput::poll(void)
{
    if (_state != BUILD_RUNNING || _pipe == NULL) {
        return 0;
    }

    int newLines = 0;
    char buffer[4096];

    // Non-blocking read loop
    while (1) {
        ssize_t bytesRead = read(_fd, buffer, sizeof(buffer) - 1);

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';

            // Process the buffer, splitting on newlines
            int i = 0;
            while (i < bytesRead) {
                char c = buffer[i];

                if (c == '\n') {
                    // End of line - parse and add
                    BuildOutputLine *line = parseLine(_partialLine);
                    _lines.append(line);
                    _partialLine = "";
                    newLines = 1;
                } else {
                    _partialLine += c;
                }
                i++;
            }
        }
        else if (bytesRead == 0) {
            // EOF - process is done
            // Handle any remaining partial line
            if (_partialLine.length() > 0) {
                BuildOutputLine *line = parseLine(_partialLine);
                _lines.append(line);
                _partialLine = "";
                newLines = 1;
            }

            finalizeBuild();
            break;
        }
        else {
            // bytesRead < 0
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data available right now - that's fine
                break;
            }
            else {
                // Real error - finalize
                if (_partialLine.length() > 0) {
                    BuildOutputLine *line = parseLine(_partialLine);
                    _lines.append(line);
                    _partialLine = "";
                    newLines = 1;
                }

                finalizeBuild();
                break;
            }
        }
    }

    return newLines;
}


//-------------------------------------------------------------------------------------------------
// finalizeBuild
//
// Close the pipe and extract exit code.
//-------------------------------------------------------------------------------------------------
void
BuildOutput::finalizeBuild(void)
{
    if (_pipe != NULL) {
        int status = pclose(_pipe);
        _pipe = NULL;
        _fd = -1;

#if defined(_LINUX_) || defined(_OSX_) || defined(_SUNOS_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_IRIX6_) || defined(_NETBSD_) || defined(_NEXT_)
        if (WIFEXITED(status)) {
            _exitCode = WEXITSTATUS(status);
        } else {
            _exitCode = -1;
        }
#else
        _exitCode = status;
#endif
    }

    if (_exitCode == 0) {
        _state = BUILD_COMPLETE;
    } else {
        _state = BUILD_ERROR;
    }

    // Add "Build Done" line to the output
    BuildOutputLine *doneLine = new BuildOutputLine();
    doneLine->text = "Build Done";
    doneLine->type = BUILD_LINE_PLAIN;
    doneLine->line = 0;
    doneLine->column = 0;
    _lines.append(doneLine);
}


//-------------------------------------------------------------------------------------------------
// parseLine
//
// Parse a single line and classify it.
// Returns a newly allocated BuildOutputLine that must be freed by the caller.
//-------------------------------------------------------------------------------------------------
BuildOutputLine*
BuildOutput::parseLine(CxString text)
{
    BuildOutputLine *result = new BuildOutputLine();
    result->text = text;
    result->type = BUILD_LINE_PLAIN;
    result->line = 0;
    result->column = 0;

    // Try to parse as a build error
    CxBuildError err = CxProcess::parseBuildError(text);

    if (err.valid) {
        result->filename = err.filename;
        result->line = err.line;
        result->column = err.column;

        // Classify based on message content
        CxString msg = err.message;

        // Check for error
        if (msg.index("error:") >= 0 || msg.index("Error:") >= 0) {
            result->type = BUILD_LINE_ERROR;
            _errorCount++;
        }
        // Check for warning
        else if (msg.index("warning:") >= 0 || msg.index("Warning:") >= 0) {
            result->type = BUILD_LINE_WARNING;
            _warningCount++;
        }
        // Check for note
        else if (msg.index("note:") >= 0 || msg.index("Note:") >= 0) {
            result->type = BUILD_LINE_NOTE;
        }
        // Has file:line but no known keyword - treat as error
        else {
            result->type = BUILD_LINE_ERROR;
            _errorCount++;
        }
    }
    else {
        // Not a file:line pattern - check for compiler command
        // Common compiler invocations: gcc, g++, clang, clang++, cc, c++
        if (text.length() > 0) {
            // Check if line starts with a compiler name
            if (text.index("gcc ") == 0 || text.index("gcc\t") == 0 ||
                text.index("g++ ") == 0 || text.index("g++\t") == 0 ||
                text.index("clang ") == 0 || text.index("clang\t") == 0 ||
                text.index("clang++ ") == 0 || text.index("clang++\t") == 0 ||
                text.index("cc ") == 0 || text.index("cc\t") == 0 ||
                text.index("c++ ") == 0 || text.index("c++\t") == 0) {
                result->type = BUILD_LINE_COMMAND;
            }
            // Also check for indented compiler commands (from make)
            else if (text.length() > 1 && (text.charAt(0) == ' ' || text.charAt(0) == '\t')) {
                CxString trimmed = text;
                while (trimmed.length() > 0 && (trimmed.charAt(0) == ' ' || trimmed.charAt(0) == '\t')) {
                    trimmed = trimmed.subString(1, trimmed.length() - 1);
                }
                if (trimmed.index("gcc ") == 0 || trimmed.index("gcc\t") == 0 ||
                    trimmed.index("g++ ") == 0 || trimmed.index("g++\t") == 0 ||
                    trimmed.index("clang ") == 0 || trimmed.index("clang\t") == 0 ||
                    trimmed.index("clang++ ") == 0 || trimmed.index("clang++\t") == 0 ||
                    trimmed.index("cc ") == 0 || trimmed.index("cc\t") == 0 ||
                    trimmed.index("c++ ") == 0 || trimmed.index("c++\t") == 0) {
                    result->type = BUILD_LINE_COMMAND;
                }
            }
        }
    }

    return result;
}


//-------------------------------------------------------------------------------------------------
// getState
//-------------------------------------------------------------------------------------------------
BuildState
BuildOutput::getState(void)
{
    return _state;
}


//-------------------------------------------------------------------------------------------------
// isRunning
//-------------------------------------------------------------------------------------------------
int
BuildOutput::isRunning(void)
{
    return (_state == BUILD_RUNNING) ? 1 : 0;
}


//-------------------------------------------------------------------------------------------------
// isComplete
//-------------------------------------------------------------------------------------------------
int
BuildOutput::isComplete(void)
{
    return (_state == BUILD_COMPLETE || _state == BUILD_ERROR) ? 1 : 0;
}


//-------------------------------------------------------------------------------------------------
// lineCount
//-------------------------------------------------------------------------------------------------
int
BuildOutput::lineCount(void)
{
    return _lines.entries();
}


//-------------------------------------------------------------------------------------------------
// lineAt
//-------------------------------------------------------------------------------------------------
BuildOutputLine*
BuildOutput::lineAt(int index)
{
    if (index < 0 || index >= (int)_lines.entries()) {
        return NULL;
    }
    // List stores pointers, at() returns a copy of the pointer value
    return _lines.at(index);
}


//-------------------------------------------------------------------------------------------------
// errorCount
//-------------------------------------------------------------------------------------------------
int
BuildOutput::errorCount(void)
{
    return _errorCount;
}


//-------------------------------------------------------------------------------------------------
// warningCount
//-------------------------------------------------------------------------------------------------
int
BuildOutput::warningCount(void)
{
    return _warningCount;
}


//-------------------------------------------------------------------------------------------------
// exitCode
//-------------------------------------------------------------------------------------------------
int
BuildOutput::exitCode(void)
{
    return _exitCode;
}


//-------------------------------------------------------------------------------------------------
// getCommand
//-------------------------------------------------------------------------------------------------
CxString
BuildOutput::getCommand(void)
{
    return _command;
}


//-------------------------------------------------------------------------------------------------
// clear
//-------------------------------------------------------------------------------------------------
void
BuildOutput::clear(void)
{
    // Close pipe if still open
    if (_pipe != NULL) {
        pclose(_pipe);
        _pipe = NULL;
    }

    _fd = -1;
    _state = BUILD_IDLE;
    _command = "";
    _exitCode = -1;
    _errorCount = 0;
    _warningCount = 0;
    _partialLine = "";
    _buildDirectory = "";
    _subprojectName = "";

    // Delete all allocated lines and clear the list
    _lines.clearAndDelete();
}


//-------------------------------------------------------------------------------------------------
// setBuildContext
//
// Set the build directory and subproject name for path resolution and UI display.
//-------------------------------------------------------------------------------------------------
void
BuildOutput::setBuildContext(CxString directory, CxString subprojectName)
{
    _buildDirectory = directory;
    _subprojectName = subprojectName;
}


//-------------------------------------------------------------------------------------------------
// getBuildDirectory
//-------------------------------------------------------------------------------------------------
CxString
BuildOutput::getBuildDirectory(void)
{
    return _buildDirectory;
}


//-------------------------------------------------------------------------------------------------
// getSubprojectName
//-------------------------------------------------------------------------------------------------
CxString
BuildOutput::getSubprojectName(void)
{
    return _subprojectName;
}


//-------------------------------------------------------------------------------------------------
// appendSeparator
//
// Append a separator line to the output (e.g., "=== Building cx ===").
// Used by build-all to visually separate output from different subprojects.
//-------------------------------------------------------------------------------------------------
void
BuildOutput::appendSeparator(CxString text)
{
    BuildOutputLine *line = new BuildOutputLine();
    line->type = BUILD_LINE_SEPARATOR;
    line->text = text;
    line->line = 0;
    line->column = 0;
    _lines.append(line);
}
