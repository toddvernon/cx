## cx/platform.mk -- single source of truth for platform detection ###########
##
## Included by every cx makefile (library modules, cx_tests, cx_apps) so the
## OS/arch detection lives in ONE place instead of being copy-pasted into ~58
## makefiles (where it had drifted: some set ARCH for netbsd, most didn't; none
## set it for sunos/irix; two test makefiles even had a "netbsk" typo).
##
## TWO SEPARATE THINGS, kept separate on purpose:
##
##   1. The COMPILE MACRO in CPPFLAGS (-D _SUNOS_, -D _SOLARIS6_, -D _OSX_, ...).
##      This is what the #ifdef guards in the .cpp/.h sources compile against.
##      It NEVER changes here -- it is byte-for-byte what the old per-makefile
##      blocks emitted. Touch this and you change which code path compiles.
##
##   2. The DIRECTORY TAG, $(PLATFORM) = $(PLATFORM_OS)_$(ARCH), used only to
##      name the per-platform object and lib output dirs (e.g. lib/darwin_arm64,
##      lib/sunos4_sun4m). It never reaches the preprocessor.
##
## Why PLATFORM_OS exists separate from `uname -s`: SunOS 4.1.x and every
## Solaris release all report `uname -s = sunos`, and on the same hardware
## report the same `uname -m` (both sun4m on a SS-5). Keying the output dir on
## raw uname_s makes their incompatible binaries collide in one dir (a real
## problem on shared NFS). PLATFORM_OS encodes the release (sunos4 / solaris6 /
## solaris10) so the dirs stay distinct. linux and darwin keep PLATFORM_OS ==
## uname_s, so lib/linux_x86_64 and lib/darwin_arm64 are unchanged.
##
## Override hook: PLATFORM and CPPFLAGS are plain make variables, so a host with
## no `uname` (historically NeXT) can still build with, e.g.:
##     make PLATFORM=nextstep_m68k CPPFLAGS='-D _NEXT_ -D_POSIX_SOURCE=1 -g'
## (a command-line assignment overrides the values derived below).
##
## Portability: uses only the make features the old blocks already used --
## $(shell ...), := , and separate ifeq/endif. No filter/wildcard/patsubst,
## no `else ifeq`. Requires GNU make, same as the rest of the cx build.
#############################################################################

UNAME_S := $(shell uname -s | tr '[A-Z]' '[a-z]' )
UNAME_R := $(shell uname -r)
ARCH    := $(shell uname -m | tr '[A-Z]' '[a-z]' )

# Default OS tag is just the OS name; the sun/irix release blocks below refine
# it. linux / darwin / netbsd / nextstep all keep PLATFORM_OS == uname_s.
PLATFORM_OS := $(UNAME_S)

# Linux
ifeq ($(UNAME_S),linux)
	CPPFLAGS = -D _LINUX_ -g -Wno-deprecated
endif

# macOS
ifeq ($(UNAME_S),darwin)
	CPPFLAGS = -D _OSX_ -g -Wno-deprecated
endif

# SunOS 4.x (BSD) and Solaris 5.x (SVR4) -- both are uname_s = sunos, split by
# uname_r. Solaris splits socket/name-service out of libc (-lsocket -lnsl);
# SunOS 4.x has them in libc (empty PLATFORM_LIBS -- -lsocket would fail there).
ifeq ($(UNAME_S),sunos)

	ifeq ($(UNAME_R), 4.1.3)
		PLATFORM_OS := sunos4
		CPPFLAGS = -D _SUNOS_ -g
	endif

	ifeq ($(UNAME_R), 4.1.4)
		PLATFORM_OS := sunos4
		CPPFLAGS = -D _SUNOS_ -g
	endif

	ifeq ($(UNAME_R), 5.6)
		PLATFORM_OS := solaris6
		CPPFLAGS = -D _SOLARIS6_ -g
		PLATFORM_LIBS = -lsocket -lnsl
	endif

	ifeq ($(UNAME_R), 5.7)
		PLATFORM_OS := solaris6
		CPPFLAGS = -D _SOLARIS6_ -g
		PLATFORM_LIBS = -lsocket -lnsl
	endif

	ifeq ($(UNAME_R), 5.10)
		PLATFORM_OS := solaris10
		CPPFLAGS = -D _SOLARIS10_ -g
		PLATFORM_LIBS = -lsocket -lnsl
	endif

endif

# IRIX
ifeq ($(UNAME_S),irix)
	ifeq ($(UNAME_R), 6.5)
		PLATFORM_OS := irix6
		CPPFLAGS = -D _IRIX6_ -g
	endif
endif

# NetBSD
ifeq ($(UNAME_S),netbsd)
	CPPFLAGS = -D _NETBSD_ -g
	PLATFORM_LIBS = -lpthread
endif

# NeXTSTEP (no uname; see the override hook in the header comment)
ifeq ($(UNAME_S),nextstep)
	CPPFLAGS = -D _NEXT_ -D_POSIX_SOURCE=1 -g
endif

# The per-platform output dir token. Everything that used to spell out
# $(UNAME_S)_$(ARCH) now uses $(PLATFORM).
PLATFORM := $(PLATFORM_OS)_$(ARCH)
