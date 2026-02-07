//-------------------------------------------------------------------------------------------------
//
//  process.h
//  cx/process
//
//  Run external commands and parse build output.
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
    // Get the captured output from the last run
    //---------------------------------------------------------------------------------------------
    CxString getOutput(void);

    //---------------------------------------------------------------------------------------------
    // Get the exit code from the last run
    //---------------------------------------------------------------------------------------------
    int getExitCode(void);

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
};

#endif
