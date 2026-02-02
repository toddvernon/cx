# cx

A portable C++ library with modules for base types, networking, JSON, terminal UI, and more.

## Supported Platforms

- macOS (Darwin)
- Linux
- SunOS/Solaris
- IRIX

## Directory Structure

The cx ecosystem expects the following directory layout:

```
cx/
  cx/           <- this repo (cx library)
  cx_tests/     <- test suite
  cx_apps/
    cm/         <- CMacs terminal editor
```

## Setup

Clone the repositories into the correct structure:

```bash
mkdir -p ~/dev/cx/cx_apps
cd ~/dev/cx

git clone git@github.com:toddvernon/cx.git
git clone git@github.com:toddvernon/cx_tests.git

cd cx_apps
git clone git@github.com:toddvernon/cm.git
```

## Building

```bash
cd ~/dev/cx/cx
make
```

This builds all library modules into `lib/<platform>_<arch>/`.

## Modules

- **base** - Core types: strings, buffers, files, exceptions
- **editbuffer** - Text editing buffer with UTF-8 support
- **screen** - Terminal/screen handling
- **keyboard** - Keyboard input handling
- **net** - Networking (sockets)
- **json** - JSON parsing
- **log** - Logging
- **thread** - Threading primitives
- **expression** - Expression evaluation
- **tz** - Timezone handling
- **b64** - Base64 encoding

## Related Repositories

- [cm](https://github.com/toddvernon/cm) - CMacs terminal text editor
- [cx_tests](https://github.com/toddvernon/cx_tests) - Test suite
