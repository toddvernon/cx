# CX Library Instructions

## Overview
CX is a portable C++ library with modules: base, b64, commandline, editbuffer, expression, functor, json, keyboard, log, net, screen, thread, tz.

## Naming Conventions
When the user refers to:
- **"cx"** - means all cx repositories together: `cx/cx` (this library), `cx/cx_tests` (tests), and `cx/cx_apps/cm` (editor app)
- **"cm"** - means the cmacs editor application at `cx/cx_apps/cm`
- **"cx library"** or **"cx/cx"** - means specifically this shared library

## Related Repositories
- **cmacs editor**: `../cx_apps/cm/` - terminal text editor that uses this library

## Change Policy
- **Always show proposed changes before applying them** - describe what will be modified and wait for approval before editing files
- After approval, verify changes compile successfully by running `make`

## Do Not Modify
- Build output directories

## Build Instructions
```bash
make
```

## Creating Archives
Always use the make target to create distribution archives - never run tar manually:
```bash
make archive    # creates ../ARCHIVE/cxlibs_unix.tar
```
The make target excludes build outputs and other cruft that should not be distributed.

## Makefile guidelines
- Platform detection should use `uname -s` (OS name), not `uname -r` (kernel version)
- Never hardcode specific kernel versions for Linux - flags like `CPPFLAGS` and `CCFLAGS` should apply to all Linux versions
- Solaris is an exception: different Solaris versions (4.1.x, 5.6, 5.7, 5.10) require different defines (`_SUNOS_`, `_SOLARIS6_`, `_SOLARIS10_`)
- When adding or modifying makefiles, follow the patterns in existing lib makefiles (e.g., `base/makefile`)
- Makefiles must be portable across old make implementations (SunOS, IRIX, BSD). Only use features and automatic variables already present in the existing makefiles. Do not introduce GNU make extensions such as `$(filter ...)`, `$(wildcard ...)`, `$(patsubst ...)`, etc.

## Non-negotiable constraints
- DO NOT use the C++ Standard Library: no `std::` anywhere.
- DO NOT introduce templates, including template-based third-party libs.
- DO NOT add new external dependencies.
- Only use libraries that already exist in this repository.
- If a feature would normally use STL/RAII/modern C++, implement it using existing in-repo utilities.

## Portability targets
- Code must compile on: macOS (darwin), Linux, SunOS/Solaris, IRIX.
- Assume older compilers. Avoid modern language features (C++11+), unless the codebase already uses them everywhere.

## Language/feature restrictions (assume old toolchains)
- Avoid: auto, nullptr, constexpr, lambda, range-for, threads, regex, exceptions (unless already widely used).
- Avoid: <iostream>, <string>, <vector>, <map>, <memory>, <algorithm>, <functional>, <type_traits>, etc.
- DO NOT define classes inside function bodies (local/inner classes). GCC 2.95 crashes generating DWARF2 debug info for local class methods. Always define classes at file scope.
- Prefer C-style interfaces or existing repo abstractions.

## Includes & headers
- Prefer existing project headers and utilities.
- Use C headers where appropriate: <stdio.h>, <stdlib.h>, <string.h>, <unistd.h> (guarded), etc.
- Minimize OS-specific includes; isolate with `#ifdef` blocks and use only existing defines for platform

## OS-specific code
- Any OS-specific code MUST be isolated behind preprocessor guards.
- Use existing platform abstraction layers if present.
- Never add a new platform directory structure without explicit instruction.

## Code changes workflow
- Make the smallest change that solves the problem.
- Do not reformat unrelated code.
- Do not rename symbols/files unless asked.
- Add comments where portability is non-obvious.

## Output expectation
- When proposing a change, briefly state:
  1) which files you changed,
  2) why it is portable across targets,
  3) what repo-local libraries you used.
