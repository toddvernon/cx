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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/time.h>

#if defined(_LINUX_) || defined(_OSX_) || defined(_SUNOS_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_IRIX6_) || defined(_NETBSD_) || defined(_NEXT_)
#include <sys/wait.h>
#endif


//-------------------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------------------
CxProcess::CxProcess()
    : _exitCode(-1), _killedByTimeout(0)
{
}


//-------------------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------------------
CxProcess::~CxProcess()
{
}


//-------------------------------------------------------------------------------------------------
// cxSetEnvDup -- putenv a NAME=value pair, strdup'd so it outlives the call.
// putenv (not setenv: setenv is absent on Solaris 2.6) keeps the pointer, so it
// must persist; we leak it deliberately (called in a child about to exec).
//-------------------------------------------------------------------------------------------------
static void
cxSetEnvDup(const char *name, const char *value)
{
    if (name == (const char*)0 || value == (const char*)0) {
        return;
    }
    char *kv = (char*) malloc(strlen(name) + 1 + strlen(value) + 1);
    if (kv == (char*)0) {
        return;
    }
    sprintf(kv, "%s=%s", name, value);
    putenv(kv);
}


//-------------------------------------------------------------------------------------------------
// cxDropToUser -- drop privileges to `user` in the child, before exec, and set a
// login-ish environment so the command behaves as that user. Returns 0 on
// success, -1 on any failure (the caller _exit()s, so a failed drop can never
// fall through to running as root).
//
// Max portability: getpwnam + initgroups + setgid + setuid + putenv -- all
// present on Solaris 2.6 / SunOS 4 / BSD / Linux / macOS / Irix, no per-platform
// #ifdef. Deliberately NOT getpwnam_r (its signature differs Solaris-vs-POSIX)
// -- safe here because we're single-threaded after fork. Order matters: groups
// and gid are set while still root; setuid is LAST and irreversible (a permanent
// drop, vs. seteuid which could be reversed).
//-------------------------------------------------------------------------------------------------
static int
cxDropToUser(const char *user)
{
    struct passwd *pw = getpwnam(user);
    if (pw == (struct passwd*)0) {
        fprintf(stderr, "cx: unknown user: %s\n", user);
        return -1;
    }
    if (initgroups(pw->pw_name, pw->pw_gid) != 0) {
        fprintf(stderr, "cx: initgroups failed for %s\n", user);
        return -1;
    }
    if (setgid(pw->pw_gid) != 0) {
        fprintf(stderr, "cx: setgid failed for %s\n", user);
        return -1;
    }
    if (setuid(pw->pw_uid) != 0) {
        fprintf(stderr, "cx: setuid failed for %s\n", user);
        return -1;
    }
    cxSetEnvDup("HOME", pw->pw_dir);
    cxSetEnvDup("USER", pw->pw_name);
    cxSetEnvDup("LOGNAME", pw->pw_name);
    if (pw->pw_shell != (char*)0 && pw->pw_shell[0] != '\0') {
        cxSetEnvDup("SHELL", pw->pw_shell);
    }
    return 0;
}


//-------------------------------------------------------------------------------------------------
// cxMilliSleep -- portable sub-second sleep via an empty select() timeout.
//-------------------------------------------------------------------------------------------------
static void
cxMilliSleep(int ms)
{
    struct timeval tv;
    tv.tv_sec  = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    select(0, (fd_set*)0, (fd_set*)0, (fd_set*)0, &tv);
}


//-------------------------------------------------------------------------------------------------
// Run a command and capture output (no cwd, no timeout).
// Delegates to the full form so there is a single implementation.
//-------------------------------------------------------------------------------------------------
int
CxProcess::run(const char *command)
{
    return run(command, (const char*)0, 0, (const char*)0);
}


//-------------------------------------------------------------------------------------------------
// Three-arg form: run with cwd/timeout but no privilege drop (inherits the
// caller's uid). Delegates to the full form so there is one implementation.
//-------------------------------------------------------------------------------------------------
int
CxProcess::run(const char *command, const char *cwd, int timeout_ms)
{
    return run(command, cwd, timeout_ms, (const char*)0);
}


//-------------------------------------------------------------------------------------------------
// Run a command in an optional working directory, with an optional timeout.
//
// fork + pipe + execl("/bin/sh","-c"), draining combined stdout/stderr through a
// select() loop. The child gets its own process group so a timeout can signal
// the whole tree (SIGTERM, ~1s grace, then SIGKILL). Exit code is WEXITSTATUS,
// or 128+signal if the command was killed. POSIX-only (fork/pipe/dup2/execl/
// chdir/setpgid/select/waitpid/kill), so it builds the same on macOS, Linux,
// and Solaris.
//
// `user`: NULL or "" runs as the caller's uid (the historical behavior). When
// set, the child drops privileges to that user (getpwnam validated) before
// exec; a failed drop _exit(127)s rather than running as the caller. Requires
// the caller to be root (only root can setuid to another user); a non-root
// caller asking for a different user fails the drop and the child exits 127.
//-------------------------------------------------------------------------------------------------
int
CxProcess::run(const char *command, const char *cwd, int timeout_ms, const char *user)
{
    _output = "";
    _exitCode = -1;
    _killedByTimeout = 0;

    if (command == (const char*)0 || command[0] == '\0') {
        return -1;
    }

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid == 0) {
        // ---- child ----
        setpgid(0, 0);                       // own group, for a group-wide kill

        close(pipefd[0]);
        dup2(pipefd[1], 1);                  // stdout -> pipe
        dup2(pipefd[1], 2);                  // stderr -> pipe
        close(pipefd[1]);

        // Drop to the target user (if any) while still root: groups + gid +
        // uid, then a login-ish env. A failed drop never falls through to root.
        if (user != (const char*)0 && user[0] != '\0') {
            if (cxDropToUser(user) != 0) {
                _exit(127);
            }
        }

        if (cwd != (const char*)0 && cwd[0] != '\0') {
            if (chdir(cwd) != 0) {
                fprintf(stderr, "cx: chdir failed: %s\n", cwd);
                _exit(127);
            }
        }

        execl("/bin/sh", "sh", "-c", command, (char*)0);
        _exit(127);                          // exec failed
    }

    // ---- parent ----
    close(pipefd[1]);
    setpgid(pid, pid);                       // best-effort (races with exec)

    struct timeval deadline;
    int haveDeadline = 0;
    if (timeout_ms > 0) {
        struct timeval now;
        gettimeofday(&now, (struct timezone*)0);
        long carry = now.tv_usec + (long)(timeout_ms % 1000) * 1000L;
        deadline.tv_sec  = now.tv_sec + (timeout_ms / 1000) + (carry / 1000000L);
        deadline.tv_usec = carry % 1000000L;
        haveDeadline = 1;
    }

    char buffer[4096];
    int done = 0;

    while (!done) {

        if (haveDeadline) {
            struct timeval now;
            gettimeofday(&now, (struct timezone*)0);
            if (now.tv_sec > deadline.tv_sec ||
                (now.tv_sec == deadline.tv_sec && now.tv_usec >= deadline.tv_usec)) {
                // Deadline passed: terminate the group, grace, then hard kill.
                _killedByTimeout = 1;
                kill(-pid, SIGTERM);
                int reaped = 0;
                int waited = 0;
                while (waited < 1000) {
                    int st;
                    if (waitpid(pid, &st, WNOHANG) == pid) { reaped = 1; break; }
                    cxMilliSleep(100);
                    waited += 100;
                }
                if (!reaped) {
                    int st;
                    kill(-pid, SIGKILL);
                    waitpid(pid, &st, 0);
                }
                close(pipefd[0]);
                _exitCode = -1;
                return -1;
            }
        }

        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(pipefd[0], &rset);

        struct timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 100000;                 // 100ms slice

        int rc = select(pipefd[0] + 1, &rset, (fd_set*)0, (fd_set*)0, &tv);

        if (rc < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;                           // select error
        }

        if (rc > 0 && FD_ISSET(pipefd[0], &rset)) {
            int n = read(pipefd[0], buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0';
                _output += buffer;
            } else if (n == 0) {
                done = 1;                     // EOF: child closed its write end
            } else {
                if (errno == EINTR) {
                    continue;
                }
                done = 1;                     // read error
            }
        }
        // rc == 0: nothing this slice; loop re-checks the deadline
    }

    close(pipefd[0]);

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        _exitCode = -1;                       // launched, but couldn't reap
        return 0;
    }

    if (WIFEXITED(status)) {
        _exitCode = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        _exitCode = 128 + WTERMSIG(status);
    } else {
        _exitCode = -1;
    }

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
// Was the last run killed by its timeout?
//-------------------------------------------------------------------------------------------------
int
CxProcess::wasTimedOut(void)
{
    return _killedByTimeout;
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
