# cx

A portable C++ library that builds on everything from your M3 Mac to a 1993 SPARCstation.

## Why cx?

Most C++ libraries assume the C++ Standard Library exists. cx doesn't. It provides strings, containers, file I/O, JSON parsing, terminal handling, and more—all implemented from scratch with zero external dependencies.

This means cx compiles on:
- Modern macOS and Linux with current compilers
- SGI IRIX 6.5 with GCC 2.8.1
- Solaris and SunOS from the 1990s
- NetBSD on whatever hardware you've got

No Boost. No external dependencies. No autoconf. No cmake. Just `make`.

## Design Philosophy

cx is built for portability without compromise. The codebase uses only minimal internal templates for containers, avoids exceptions and modern C++ features that break on older compilers. Every module compiles cleanly on systems from 1993 to today.

But portability doesn't mean primitive. On modern systems, cx provides:
- Full UTF-8 string handling
- 24-bit RGB terminal color support
- Threading primitives (pthreads)
- JSON parsing and generation
- Network sockets

The same code adapts to what the platform offers.

## Directory Structure

cx expects this layout:

```
cx/
├── cx/              <- this repo (the library)
├── cx_apps/
│   └── cm/          <- CMacs terminal editor
├── cx_tests/        <- test suite
└── lib/             <- built libraries (created by make)
```

## Getting Started

```bash
# Create the directory structure
mkdir -p ~/dev/cx/cx_apps
cd ~/dev/cx

# Clone cx
git clone https://github.com/toddvernon/cx.git

# Build
cd cx
make
```

Libraries are built to `../lib/<platform>_<arch>/` (e.g., `lib/darwin_arm64/`).

### Building on Vintage Unix (No Git)

For platforms without git (SunOS, IRIX, older Solaris, etc.), build archives on a modern machine first:

**Step 1: Create archives on a machine with git**

```bash
# Clone all repos on Mac or Linux
mkdir -p ~/dev/cx/cx_apps
cd ~/dev/cx

git clone https://github.com/toddvernon/cx.git
git clone https://github.com/toddvernon/cx_tests.git
git clone https://github.com/toddvernon/cm.git cx_apps/cm

# Create tar archives
cd cx && make archive
cd ../cx_tests && make archive
cd ../cx_apps/cm && make archive
```

Archives are created in `~/dev/cx/ARCHIVE/`:
- `cxlibs_unix.tar` — cx library source
- `cxtest_unix.tar` — test suite
- `cm_unix.tar` — CMacs editor

**Step 2: Transfer to target machine**

```bash
# FTP or copy the archives to the target
ftp target-machine
> cd /home/user
> mkdir cx
> cd cx
> put ARCHIVE/cxlibs_unix.tar
> put ARCHIVE/cxtest_unix.tar
> put ARCHIVE/cm_unix.tar
> quit
```

**Step 3: Extract and build on target**

```bash
# On the target machine
cd ~/cx

# Extract each archive (they include path prefixes)
tar xvf cxlibs_unix.tar      # extracts to cx/
tar xvf cxtest_unix.tar      # extracts to cx_tests/
tar xvf cm_unix.tar          # extracts to cx_apps/cm/

# Create lib directory
mkdir lib

# Build in order
cd cx && make
cd ../cx_tests && make
cd ../cx_apps/cm && make

# Run tests
cd ../cx_tests && make test
```

See `PLATFORM_SUPPORT.md` for details on which libraries and tests are available per platform.

### If Something Goes Wrong

- **Vintage Unix?** You'll need GNU Make and GCC 2.8.1 or later. The makefile uses GNU Make syntax (`ifeq`, `$(shell)`, `:=`). Both are available at [gnu.org](https://www.gnu.org/software/).

## Modules

| Module | Description |
|--------|-------------|
| **base** | Core types: strings, buffers, files, lists, hashmaps |
| **editbuffer** | Text editing buffer with UTF-8 support |
| **screen** | Terminal/screen handling with RGB color |
| **keyboard** | Keyboard input handling |
| **json** | JSON parsing and generation |
| **net** | TCP/IP sockets |
| **thread** | Threading primitives (Linux/macOS) |
| **process** | Process spawning and management |
| **buildoutput** | Build output parsing |
| **commandcompleter** | Tab-completion engine |
| **regex** | Regular expressions (Linux/macOS) |
| **log** | Logging |
| **expression** | Expression evaluation |
| **tz** | Timezone handling |
| **b64** | Base64 encoding |

## Supported Platforms

- macOS (ARM64 and x86_64)
- Linux (x86_64)
- Solaris / SunOS
- IRIX 6.5
- NetBSD

## Related Projects

- [CMacs](https://github.com/toddvernon/cm) — Terminal text editor built on cx
- [cx_tests](https://github.com/toddvernon/cx_tests) — Test suite

## License

Apache License 2.0. See LICENSE file.
