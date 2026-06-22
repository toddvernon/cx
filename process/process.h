//-------------------------------------------------------------------------------------------------
//
//  process.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  process.h
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxProcess_h_
#define _CxProcess_h_

#include <cx/base/string.h>

//-------------------------------------------------------------------------------------------------
// CxBuildError
//
// Holds parsed error information from a compiler output line.
//-------------------------------------------------------------------------------------------------
struct CxBuildError
{
    CxString filename;      // source file path
    int line;               // line number (1-based), 0 if not found
    int column;             // column number (1-based), 0 if not found
    CxString message;       // error/warning message
    int valid;              // 1 if successfully parsed, 0 otherwise

    CxBuildError() : line(0), column(0), valid(0) {}
};


//-------------------------------------------------------------------------------------------------
// CxProcess
//
// Run external commands and capture output.
//-------------------------------------------------------------------------------------------------
class CxProcess
{
public:
    CxProcess();
    ~CxProcess();

    //---------------------------------------------------------------------------------------------
    // Run a command and capture its output (stdout and stderr combined)
    // Returns 0 on success, -1 on failure to execute
    // Output is stored and accessible via getOutput()
    //---------------------------------------------------------------------------------------------
    int run(const char *command);
    int run(CxString command);

    //---------------------------------------------------------------------------------------------
    // Run a command in an optional working directory, with an optional timeout.
    //   cwd        : NULL or "" inherits the caller's cwd; otherwise chdir before exec
    //   timeout_ms : 0 waits indefinitely; > 0 kills the command after the deadline
    // Returns 0 on a clean launch + wait, -1 on launch failure or a timeout kill.
    // Combined stdout+stderr is in getOutput(); exit code in getExitCode()
    // (128 + signal if the command was killed by a signal); see wasTimedOut().
    //---------------------------------------------------------------------------------------------
    int run(const char *command, const char *cwd, int timeout_ms);

    //---------------------------------------------------------------------------------------------
    // As above, but drop privileges to `user` before exec (NULL/"" = run as the
    // caller, the historical behavior). The child validates the user via
    // getpwnam and does initgroups + setgid + setuid (+ a login-ish HOME/USER/
    // LOGNAME/SHELL env); a failed drop _exit(127)s rather than running as the
    // caller. Requires the caller to be root to drop to a different user.
    // Portable across Solaris 2.6 / BSD / Linux / macOS / Irix (no #ifdef).
    //---------------------------------------------------------------------------------------------
    int run(const char *command, const char *cwd, int timeout_ms, const char *user);

    //---------------------------------------------------------------------------------------------
    // Get the captured output from the last run
    //---------------------------------------------------------------------------------------------
    CxString getOutput(void);

    //---------------------------------------------------------------------------------------------
    // Get the exit code from the last run
    //---------------------------------------------------------------------------------------------
    int getExitCode(void);

    //---------------------------------------------------------------------------------------------
    // True if the last run was killed because it exceeded its timeout (vs. a
    // normal exit). Only meaningful after a run() with timeout_ms > 0.
    //---------------------------------------------------------------------------------------------
    int wasTimedOut(void);

    //---------------------------------------------------------------------------------------------
    // Parse a single line for build error pattern (file:line:col: or file:line:)
    // Recognizes common compiler output formats:
    //   - GCC/Clang: file.cpp:123:45: error: message
    //   - GCC/Clang: file.cpp:123: message
    //   - Generic:   file:123: message
    //---------------------------------------------------------------------------------------------
    static CxBuildError parseBuildError(CxString line);

private:
    CxString _output;
    int _exitCode;
    int _killedByTimeout;
};

#endif
