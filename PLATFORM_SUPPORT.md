# Platform Support Matrix

This document describes which libraries are built and which tests are run on each supported platform.

## Build system (platform detection)

All platform detection lives in `cx/platform.mk`, included by every makefile
(cx library modules, cx_tests, cx_apps). It derives:

- `UNAME_S`, `UNAME_R`, `ARCH` from `uname`.
- `CPPFLAGS` with the per-platform compile macro (`-D _SUNOS_`, `-D _SOLARIS6_`,
  `-D _OSX_`, ...). This is what the `#ifdef` guards in the sources compile
  against; it is unchanged from the old per-makefile blocks.
- `PLATFORM_LIBS` (`-lsocket -lnsl` on Solaris, `-lpthread` on NetBSD, empty
  elsewhere).
- `PLATFORM = $(PLATFORM_OS)_$(ARCH)` — the per-platform output dir token used
  for `lib/<PLATFORM>` and the object dirs.

`PLATFORM_OS` encodes the OS release for the sun family so incompatible binaries
never share a directory (a real problem on shared NFS, since SunOS 4.x and every
Solaris release all report `uname -s = sunos` and often the same `uname -m`):

| Platform | PLATFORM_OS | Example output dir |
|----------|-------------|--------------------|
| Linux | linux | `lib/linux_x86_64` |
| macOS | darwin | `lib/darwin_arm64` |
| SunOS 4.x | sunos4 | `lib/sunos4_sun4m` |
| Solaris 6/7 | solaris6 | `lib/solaris6_sun4m` |
| Solaris 10 | solaris10 | `lib/solaris10_sun4u` |
| IRIX 6.5 | irix6 | `lib/irix6_IP22` |
| NetBSD | netbsd | `lib/netbsd_sparc` |
| NeXTSTEP | nextstep | (no `uname`; override with `make PLATFORM=...`) |

`linux` and `darwin` keep `PLATFORM_OS == uname_s`, so their dir names are
unchanged. Requires GNU make on every platform (as the build always has).

## Supported Platforms

| Platform | Macro | uname -s | uname -r |
|----------|-------|----------|----------|
| Linux | `_LINUX_` | linux | * |
| macOS | `_OSX_` | darwin | * |
| SunOS 4.x | `_SUNOS_` | sunos | 4.1.3, 4.1.4 |
| Solaris 6/7 | `_SOLARIS6_` | sunos | 5.6, 5.7 |
| Solaris 10 | `_SOLARIS10_` | sunos | 5.10 |
| IRIX 6.5 | `_IRIX6_` | irix | 6.5 |
| NetBSD | `_NETBSD_` | netbsd | * |
| NeXTSTEP | `_NEXT_` | nextstep | * |

## Libraries (cx)

| Library | Linux | macOS | SunOS 4.x | Solaris | IRIX | NetBSD | NeXTSTEP |
|---------|-------|-------|-----------|---------|------|--------|----------|
| base | Y | Y | Y | Y | Y | Y | Y |
| net | Y | Y | Y | Y | Y | Y | Y |
| log | Y | Y | Y | Y | Y | Y | Y |
| b64 | Y | Y | Y | Y | Y | Y | Y |
| json | Y | Y | Y | Y | Y | Y | Y |
| screen | Y | Y | Y | Y | Y | Y | Y |
| editbuffer | Y | Y | Y | Y | Y | Y | Y |
| keyboard | Y | Y | Y | Y | Y | Y | Y |
| expression | Y | Y | Y | Y | Y | Y | Y |
| sheetModel | Y | Y | Y | Y | Y | Y | Y |
| commandcompleter | Y | Y | Y | Y | Y | Y | Y |
| process | Y | Y | Y | Y | Y | Y | Y |
| buildoutput | Y | Y | Y | Y | Y | Y | Y |
| thread | Y | Y | - | Y | Y | Y | Y |
| regex | Y | Y | - | - | - | - | - |
| tz | Y | Y | - | Y | - | Y | Y |

### Library Notes

- **thread**: Not available on SunOS 4.x (no pthreads support)
- **regex**: Only available on Linux and macOS (uses POSIX regex which varies across platforms)
- **tz**: Not available on SunOS 4.x or IRIX (timezone database dependencies)

## Tests (cx_tests)

| Test | Linux | macOS | SunOS 4.x | Solaris | IRIX | NetBSD | NeXTSTEP |
|------|-------|-------|-----------|---------|------|--------|----------|
| cxb64 | Y | Y | Y | Y | Y | Y | Y |
| cxbuildoutput | Y | Y | Y | Y | Y | Y | Y |
| cxcallback | Y | Y | Y | Y | Y | Y | Y |
| cxcommandcompleter | Y | Y | Y | Y | Y | Y | Y |
| cxdatetime | Y | Y | - | Y | - | Y | Y |
| cxdirectory | Y | Y | Y | Y | Y | Y | Y |
| cxeditbuffer | Y | Y | Y | Y | Y | Y | Y |
| cxeditline | Y | Y | Y | Y | Y | Y | Y |
| cxexpression | Y | Y | Y | Y | Y | Y | Y |
| cxfile | Y | Y | Y | Y | Y | Y | Y |
| cxfunctor | Y | Y | Y | Y | Y | Y | Y |
| cxhandle | Y | Y | Y | Y | Y | Y | Y |
| cxhashmap | Y | Y | Y | Y | Y | Y | Y |
| cxjson | Y | Y | Y | Y | Y | Y | Y |
| cxlog | Y | Y | Y | Y | Y | Y | Y |
| cxlogfile | Y | Y | Y | Y | Y | Y | Y |
| cxnet | Y | Y | Y | Y | Y | Y | Y |
| cxprop | Y | Y | Y | Y | Y | Y | Y |
| cxregex | Y | Y | - | - | - | - | - |
| cxscreen | Y | Y | Y | Y | Y | Y | Y |
| cxslist | Y | Y | Y | Y | Y | Y | Y |
| cxstar | Y | Y | Y | Y | Y | Y | Y |
| cxstring | Y | Y | - | Y | Y | Y | Y |
| cxthread | Y | Y | - | Y | Y | Y | Y |
| cxtime | Y | Y | Y | Y | Y | Y | Y |
| cxtz | Y | Y | - | Y | - | Y | Y |
| cxutfcharacter | Y | Y | Y | Y | Y | Y | Y |
| cxutfeditbuffer | Y | Y | Y | Y | Y | Y | Y |
| cxutfstring | Y | Y | Y | Y | Y | Y | Y |
| cxutfstringlist | Y | Y | Y | Y | Y | Y | Y |

### Test Notes

- **cxdatetime**: Requires tz library (not on SunOS 4.x or IRIX)
- **cxregex**: Requires regex library (Linux/macOS only)
- **cxstring**: Requires thread library (not on SunOS 4.x)
- **cxthread**: Requires thread library (not on SunOS 4.x)
- **cxtz**: Requires tz library (not on SunOS 4.x or IRIX)

## SunOS 4.x Specific Notes

SunOS 4.x (BSD-based) requires several compatibility workarounds:

1. **No C99 fpclassify**: Uses portable math tricks for NaN/Infinity detection
2. **No ssize_t**: Typedef provided in buildoutput.cpp
3. **No `<sstream>`**: JSON classes use toJsonString() instead of ostringstream
4. **No C++ guards in system headers**: System includes wrapped with `extern "C"`
5. **15-char ar filename limit**: Some .o filenames get truncated (cosmetic warning)

## Solaris Notes

Solaris 5.x (SVR4-based) requires:
- Link with `-lsocket -lnsl` for network functions

## Legend

- **Y** = Supported and built/tested
- **-** = Not supported on this platform
