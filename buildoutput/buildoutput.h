//-------------------------------------------------------------------------------------------------
//
//  buildoutput.h
//  cx/buildoutput
//
//  Non-blocking build process execution with streaming output and error parsing.
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxBuildOutput_h_
#define _CxBuildOutput_h_

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/process/process.h>

//-------------------------------------------------------------------------------------------------
// BuildLineType
//
// Classification of a build output line for display purposes.
//-------------------------------------------------------------------------------------------------
enum BuildLineType {
    BUILD_LINE_PLAIN,      // normal output
    BUILD_LINE_ERROR,      // file:line: error:
    BUILD_LINE_WARNING,    // file:line: warning:
    BUILD_LINE_NOTE,       // file:line: note:
    BUILD_LINE_COMMAND     // gcc, g++, clang invocations
};


//-------------------------------------------------------------------------------------------------
// BuildOutputLine
//
// A single line of build output with parsed metadata.
//-------------------------------------------------------------------------------------------------
struct BuildOutputLine {
    BuildLineType type;
    CxString text;         // full line text
    CxString filename;     // parsed from error/warning (empty if none)
    int line;              // 1-based line number, 0 if none
    int column;            // 1-based column number, 0 if none

    BuildOutputLine() : type(BUILD_LINE_PLAIN), line(0), column(0) {}
};


//-------------------------------------------------------------------------------------------------
// BuildState
//
// Current state of the build process.
//-------------------------------------------------------------------------------------------------
enum BuildState {
    BUILD_IDLE,            // no build running or started
    BUILD_RUNNING,         // build in progress
    BUILD_COMPLETE,        // build finished successfully (exit code 0)
    BUILD_ERROR            // build finished with non-zero exit code
};


//-------------------------------------------------------------------------------------------------
// BuildOutput
//
// Manages a non-blocking build process with streaming output capture.
// Uses popen() + fcntl(O_NONBLOCK) for cross-platform compatibility.
//
// Usage:
//   BuildOutput build;
//   build.start("make");
//   while (build.isRunning()) {
//       if (build.poll()) {
//           // new lines available
//       }
//   }
//   // build complete, access results via lineAt(), errorCount(), etc.
//
//-------------------------------------------------------------------------------------------------
class BuildOutput
{
public:
    BuildOutput();
    ~BuildOutput();

    //---------------------------------------------------------------------------------------------
    // Start a build command (non-blocking)
    // Returns 0 on success, -1 on failure to start
    //---------------------------------------------------------------------------------------------
    int start(CxString command);

    //---------------------------------------------------------------------------------------------
    // Poll for new output (non-blocking)
    // Returns 1 if new lines were read, 0 otherwise
    // Should be called periodically (e.g., from keyboard idle callback)
    //---------------------------------------------------------------------------------------------
    int poll(void);

    //---------------------------------------------------------------------------------------------
    // Get current build state
    //---------------------------------------------------------------------------------------------
    BuildState getState(void);

    //---------------------------------------------------------------------------------------------
    // Convenience state checks
    //---------------------------------------------------------------------------------------------
    int isRunning(void);
    int isComplete(void);

    //---------------------------------------------------------------------------------------------
    // Get number of output lines
    //---------------------------------------------------------------------------------------------
    int lineCount(void);

    //---------------------------------------------------------------------------------------------
    // Get a specific line (0-based index)
    // Returns NULL if index out of range
    //---------------------------------------------------------------------------------------------
    BuildOutputLine* lineAt(int index);

    //---------------------------------------------------------------------------------------------
    // Get error/warning counts
    //---------------------------------------------------------------------------------------------
    int errorCount(void);
    int warningCount(void);

    //---------------------------------------------------------------------------------------------
    // Get exit code (-1 if not yet finished or failed to start)
    //---------------------------------------------------------------------------------------------
    int exitCode(void);

    //---------------------------------------------------------------------------------------------
    // Get the command that was run
    //---------------------------------------------------------------------------------------------
    CxString getCommand(void);

    //---------------------------------------------------------------------------------------------
    // Clear all output and reset to IDLE state
    // Use before starting a new build
    //---------------------------------------------------------------------------------------------
    void clear(void);

private:
    //---------------------------------------------------------------------------------------------
    // Parse a line and classify it (allocates new BuildOutputLine)
    //---------------------------------------------------------------------------------------------
    BuildOutputLine* parseLine(CxString text);

    //---------------------------------------------------------------------------------------------
    // Finalize the build process after it completes
    //---------------------------------------------------------------------------------------------
    void finalizeBuild(void);

    BuildState _state;
    CxString _command;
    int _exitCode;
    int _errorCount;
    int _warningCount;

    // List of parsed output lines (stored as pointers for objectAt compatibility)
    CxSList<BuildOutputLine*> _lines;

    // Partial line buffer (for lines split across reads)
    CxString _partialLine;

    // File handle for popen
    FILE *_pipe;

    // File descriptor for non-blocking reads
    int _fd;
};

#endif
