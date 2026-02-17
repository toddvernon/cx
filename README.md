# cx

A portable C++ library with zero external dependencies. Builds on everything from your M3 Mac to a 1993 SPARCstation.

## The Problem

Most C++ libraries assume the C++ Standard Library exists and works correctly. That's fine for modern systems, but it breaks down fast when you need to support vintage Unix machines, embedded systems, or anything where the C++ runtime is incomplete or missing.

cx provides a complete foundation—strings, containers, file I/O, JSON, networking, terminal handling, and more—all implemented from scratch with no external dependencies. The same source compiles unchanged on:

- macOS (ARM64 and x86_64) with modern Clang
- Linux with GCC 4.x through 14.x
- SGI IRIX 6.5 with GCC 2.8.1
- Solaris 5.6, 5.7, 5.10
- SunOS 4.1.x (the BSD-based one from 1993)
- NetBSD on whatever hardware you've got

No Boost. No STL. No autoconf. No cmake. No package managers. Just `make`.

## Design Philosophy

cx is built for portability without compromise. The codebase:

- **Avoids the C++ Standard Library entirely** — No `std::string`, `std::vector`, `std::map`. Everything is implemented from scratch.
- **Uses minimal templates** — Only for containers (`CxSList<T>`, `CxHashmap<K,V>`, `CxHandle<T>`). No template metaprogramming.
- **Avoids modern C++ features** — No `auto`, `nullptr`, lambdas, range-for, `constexpr`. Code compiles with GCC 2.8.1.
- **Isolates platform differences** — OS-specific code is behind `#ifdef` guards. The API is identical everywhere.

But portability doesn't mean primitive. On modern systems, cx provides full UTF-8 string handling with grapheme cluster support, 24-bit RGB terminal colors, threading primitives, JSON parsing, regular expressions, and timezone handling. The same code adapts to what the platform offers.

## What's Included

| Module | Description |
|--------|-------------|
| **base** | Strings (`CxString`, `CxUTFString`), containers (`CxSList`, `CxHashmap`), files, buffers, exceptions, reference counting |
| **editbuffer** | Text editing buffer with cursor movement, insert/delete, UTF-8 support |
| **screen** | Terminal control: cursor positioning, colors (ANSI and 24-bit RGB), alternate screen, resize callbacks |
| **keyboard** | Raw keyboard input with escape sequence parsing |
| **json** | JSON parsing and generation without external libraries |
| **net** | TCP/IP sockets with platform abstraction |
| **thread** | Pthreads wrapper: threads, mutexes, condition variables, thread pools |
| **process** | Process spawning and output capture |
| **buildoutput** | Compiler output parsing (GCC/Clang error formats) |
| **commandcompleter** | Tab-completion engine with hierarchical completion |
| **regex** | POSIX regex wrapper (Linux/macOS) |
| **log** | Structured logging with configurable output |
| **expression** | Mathematical expression parser and evaluator |
| **tz** | Timezone handling with C++ abstraction layer over embedded CCTZ (Google, Apache 2.0) |
| **b64** | Base64 encoding and decoding |

## Quick Example

```cpp
#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/json/json.h>

// Strings work like you'd expect
CxString name = "cx";
CxString version = CxString(1) + "." + CxString(0);
CxString message = name + " version " + version;

// Type-safe linked list (no STL)
CxSList<CxString> files;
files.append("main.cpp");
files.append("utils.cpp");

// JSON without external dependencies
CxJSONObject *config = new CxJSONObject();
config->addMember("name", new CxJSONString("cx"));
config->addMember("portable", new CxJSONBoolean(true));
CxString json = config->toJsonString();
```

## Getting Started

cx expects this directory layout:

```
cx/
├── cx/              <- this repo (the library)
├── cx_apps/
│   └── cm/          <- cmacs terminal editor
├── cx_tests/        <- test suite
└── lib/             <- built libraries (created by make)
```

### Clone and Build

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

### Link Against cx

```makefile
# In your makefile
LIBS = -L../lib/darwin_arm64 -lcx_base -lcx_screen -lcx_json
INC = -I../../..
```

### Building on Vintage Unix (No Git)

For systems without git (SunOS 4.x, IRIX, older Solaris), create archives on a modern machine first:

**Step 1: Create archives on a machine with git**

```bash
cd ~/dev/cx/cx
make archive
```

This creates `~/dev/cx/ARCHIVE/cxlibs_unix.tar`.

**Step 2: Transfer and build**

```bash
# On the target machine
cd ~/cx
tar xvf cxlibs_unix.tar    # extracts to cx/
mkdir lib
cd cx && make
```

See `PLATFORM_SUPPORT.md` for the complete platform matrix.

## Platform Support

### Full Support (all modules)

| Platform | Tested Compilers |
|----------|------------------|
| macOS ARM64 | Apple Clang 15+ |
| macOS x86_64 | Apple Clang 12+ |
| Linux x86_64 | GCC 4.8 - 14.x |

### Most Modules

| Platform | Notes |
|----------|-------|
| Solaris 5.6/5.7/5.10 | No regex module |
| IRIX 6.5 | No tz module |
| NetBSD | Full support |

### Core Modules Only

| Platform | Notes |
|----------|-------|
| SunOS 4.1.x | No thread, regex, or tz modules |

### Requirements for Vintage Systems

- GNU Make (the makefile uses `ifeq`, `$(shell)`, `:=`)
- GCC 2.8.1 or later

Both are available at [gnu.org](https://www.gnu.org/software/).

## API Highlights

### Strings

```cpp
CxString s = "hello";
s += " world";
s.append("!");
int pos = s.firstChar('w');        // 6
CxString sub = s.subString(0, 5);  // "hello"
CxString upper = CxString::toUpper(s);

// UTF-8 with grapheme cluster support
CxUTFString utf;
utf.fromBytes(utf8_data, len, 4);  // 4-space tabs
int chars = utf.charCount();       // grapheme clusters
int width = utf.displayWidth();    // screen columns
```

### Containers

```cpp
// Linked list
CxSList<int> numbers;
numbers.append(1);
numbers.append(2);
for (CxSList<int>::Iterator it = numbers.begin(); it != numbers.end(); ++it) {
    printf("%d\n", *it);
}

// Hash map
CxHashmap<CxString, int> ages;
ages.insert("alice", 30);
int *age = ages.find("alice");
```

### JSON

```cpp
// Parse
CxJSONBase *root = CxJSONFactory::parse("{\"name\": \"cx\"}");
CxJSONObject *obj = (CxJSONObject *)root;
CxJSONString *name = (CxJSONString *)obj->getMember("name");

// Generate
CxJSONObject *config = new CxJSONObject();
config->addMember("version", new CxJSONNumber(1.0));
CxString json = config->toJsonString();
```

### Terminal

```cpp
CxScreen screen;
screen.openAlternateScreen();
CxScreen::placeCursor(10, 20);
CxScreen::setForegroundColor(new CxRGBColor(255, 100, 50));
screen.writeText("Hello in orange");
CxScreen::resetColors();
```

### Networking

```cpp
CxSocket sock;
CxInAddress addr("example.com", 80);
sock.connect(&addr);
sock.send("GET / HTTP/1.0\r\n\r\n");
CxString response = sock.receive();
```

## What cx Doesn't Do

cx is a foundation library. It provides:

- Core data structures and utilities
- Platform abstraction for I/O, networking, terminals
- Common functionality (JSON, regex, logging)

It doesn't provide:

- GUI frameworks
- Database connectors
- HTTP clients/servers
- Cryptography

For those, you'd build on top of cx or use platform-native APIs.

## Related Projects

- [cmacs](https://github.com/toddvernon/cm) — Terminal text editor built entirely on cx
- [cx_tests](https://github.com/toddvernon/cx_tests) — Test suite

## Why "cx"?

It started as "C++ cross-platform" and got shortened. The name stuck.

## Third-Party Code

The **tz** module provides a C++ abstraction layer over [CCTZ](https://github.com/google/cctz), Google's timezone library. CCTZ is embedded in cx and distributed under the Apache 2.0 License. The cx wrapper provides a simpler C++ interface for common timezone operations without requiring direct use of the CCTZ API.

## License

Apache License 2.0. See LICENSE file.
