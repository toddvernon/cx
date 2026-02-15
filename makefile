        
########################################################################################
# Figure out the OS
#
########################################################################################
UNAME_S := $(shell uname -s | tr '[A-Z]' '[a-z]' )

ifeq ($(UNAME_S), linux)
	UNAME_R := $(shell uname -r | tr '[A-Z]' '[a-z]' )
	ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
	CPPFLAGS = -D _LINUX_ -g
endif

#if this is OSX
ifeq ($(UNAME_S), darwin)
	ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
	CPPFLAGS = -D _OSX_ -g -Wno-deprecated
endif

#if this is SUNOS or SOLARIS
ifeq ($(UNAME_S),sunos)
    UNAME_R := $(shell uname -r)

    ifeq ($(UNAME_R), 4.1.3)
        CPPFLAGS = -D _SUNOS_ -g
    endif

    ifeq ($(UNAME_R), 4.1.4)
        CPPFLAGS = -D _SUNOS_ -g
    endif

    ifeq ($(UNAME_R), 5.6)
        CPPFLAGS = -D _SOLARIS6_ -g
    endif

    ifeq ($(UNAME_R), 5.7)
        CPPFLAGS = -D _SOLARIS6_ -g
    endif

    ifeq ($(UNAME_R), 5.10)
        CPPFLAGS = -D _SOLARIS10_ -g
    endif

endif

#if this is IRIX
ifeq ($(UNAME_S),irix)
	UNAME_R := $(shell uname -r)
 	ifeq ($(UNAME_R), 6.5) 
		CPPFLAGS = -D _IRIX6_ -g 
    endif
endif

ifeq ($(UNAME_S),nextstep)
    ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
    CPPFLAGS = -D _NEXT_ -D_POSIX_SOURCE=1 -g
endif



########################################################################################
# Core libraries
#
########################################################################################


all:
	test -d ../lib || mkdir ../lib
	test -d ../lib/$(UNAME_S)_$(ARCH) || mkdir ../lib/$(UNAME_S)_$(ARCH)
	#mkdir -p ../lib/$(UNAME_S)_$(ARCH)

# Base Library
	
	@if [ ! -d "./base" ]; then \
		echo ""; \
		echo "BUILD ABORT: './base' not found"; \
		exit 1; \
	fi

	@if [ -d "./base" ]; then \
		cd base; make; \
	fi

# Net Library

	@if [ ! -d "./net" ]; then \
		echo "BUILD ABORT: './base' not found"; \
		exit 1; \
	fi

	@if [ -d "./net" ]; then \
		cd net; make; \
	fi

# Logfile Library

	@if [ ! -d "./log" ]; then \
		echo "BUILD ABORT: './log' not found"; \
		exit 1; \
	fi

	@if [ -d "./log" ]; then \
		cd log; make; \
	fi

# Base 64 Library

	@if [ ! -d "./b64" ]; then \
		echo "BUILD ABORT: './log' not found"; \
		exit 1; \
	fi

	@if [ -d "./b64" ]; then \
		cd b64; make; \
	fi

# JSON library

	@if [ ! -d "./json" ]; then \
		echo "BUILD ABORT: './json' not found"; \
		exit 1; \
	fi

	@if [ -d "./json" ]; then \
		cd json; make; \
	fi

# Screen Library

	@if [ ! -d "./screen" ]; then \
		echo "BUILD ABORT: './screen' not found"; \
		exit 1; \
	fi

	@if [ -d "./screen" ]; then \
		cd screen; make; \
	fi

# EditBuffer library

	@if [ ! -d "./editbuffer" ]; then \
		echo "BUILD ABORT: './editbuffer' not found"; \
		exit 1; \
	fi

	@if [ -d "./editbuffer" ]; then \
		cd editbuffer; make; \
	fi

# Keyboard Library 

	@if [ ! -d "./keyboard" ]; then \
		echo "BUILD ABORT: './keyboard' not found"; \
		exit 1; \
	fi

	@if [ -d "./keyboard" ]; then \
		cd keyboard; make; \
	fi

# ExpressionLibrary

	@if [ ! -d "./expression" ]; then \
		echo "BUILD ABORT: './expression' not found"; \
		exit 1; \
	fi

	@if [ -d "./expression" ]; then \
		cd expression; make; \
	fi

# SheetModel Library

	@if [ ! -d "./sheetModel" ]; then \
		echo "BUILD ABORT: './sheetModel' not found"; \
		exit 1; \
	fi

	@if [ -d "./sheetModel" ]; then \
		cd sheetModel; make; \
	fi

# Command Completer Library

	@if [ ! -d "./commandcompleter" ]; then \
		echo ""; \
		echo "BUILD ABORT: './commandcompleter' not found"; \
		exit 1; \
	fi

	@if [ -d "./commandcompleter" ]; then \
		cd commandcompleter; make; \
	fi

# Process Library

	@if [ -d "./process" ]; then \
		cd process; make; \
	fi

# Build Output Library

	@if [ -d "./buildoutput" ]; then \
		cd buildoutput; make; \
	fi

# Thread library (doesn't build on SunOS )

	@if [ "$(UNAME_S)" != "sunos" ]; then \
		if [ ! -d "./thread" ]; then \
			echo "BUILD ABORT: './thread' not found"; \
			exit 1;\
		fi;\
	fi

	@if [ "$(UNAME_S)" != "sunos" ]; then \
		if [ -d "./thread" ]; then \
			cd thread; make; \
		fi;\
	fi

# Regex Library (Linux and macOS only)

	@if [ "$(UNAME_S)" = "darwin" ] || [ "$(UNAME_S)" = "linux" ]; then \
		if [ -d "./regex" ]; then \
			cd regex; make; \
		fi;\
	fi

# Timezone Library (doesn't build on SunOS

	@if [ "$(UNAME_S)" != "sunos" ]; then \
		if [ ! -d "./tz" ]; then \
			echo "BUILD ABORT: './tz' not found"; \
			exit 1;\
		fi;\
	fi

	@if [ "$(UNAME_S)" != "sunos" ]; then \
		if [ "$(UNAME_S)" != "irix" ]; then \
			if [ -d "./tz" ]; then \
				cd tz; make; \
			fi;\
		fi;\
	fi

	@echo "-------------------------------"
	@echo "All libs built for: $(UNAME_S) "
	@echo "-------------------------------"


########################################################################################
# Clean up current platform specific object files
#
########################################################################################

clean:

	@if [ -d "./base" ]; then \
		cd base; make clean; \
	fi

	@if [ -d "./net" ]; then \
		cd net; make clean; \
	fi

	@if [ -d "./log" ]; then \
		cd log; make clean; \
	fi

	@if [ -d "./b64" ]; then \
		cd b64; make clean; \
	fi

	@if [ -d "./json" ]; then \
		cd json; make clean; \
	fi

	@if [ -d "./screen" ]; then \
		cd screen; make clean; \
	fi

	@if [ -d "./editbuffer" ]; then \
		cd editbuffer; make clean; \
	fi

	@if [ -d "./keyboard" ]; then \
		cd keyboard; make clean; \
	fi

	@if [ -d "./expression" ]; then \
		cd expression; make clean; \
	fi

	@if [ -d "./sheetModel" ]; then \
		cd sheetModel; make clean; \
	fi

	@if [ -d "./commandline" ]; then \
		cd commandline; make clean; \
	fi

	@if [ -d "./commandcompleter" ]; then \
		cd commandcompleter; make clean; \
	fi

	@if [ -d "./process" ]; then \
		cd process; make clean; \
	fi

	@if [ -d "./buildoutput" ]; then \
		cd buildoutput; make clean; \
	fi

	@if [ -d "./thread" ]; then \
		cd thread; make clean; \
	fi

	@if [ -d "./regex" ]; then \
		cd regex; make clean; \
	fi

	@if [ -d "./tz" ]; then \
		cd tz; make clean; \
	fi

	rm -fr ../lib/$(UNAME_S)/*.a


########################################################################################
# Create tar archive for distribution
#
########################################################################################

archive:
	@echo "Creating cxlibs_unix.tar..."
	@echo "  (extracts to cx/ when untarred from parent directory)"
	@test -d ../ARCHIVE || mkdir ../ARCHIVE
	@tar cvf ../ARCHIVE/cxlibs_unix.tar \
		--transform 's,^\./,cx/,' \
		--exclude='*.o' \
		--exclude='*.a' \
		--exclude='.git' \
		--exclude='.claude' \
		--exclude='.DS_Store' \
		--exclude='darwin_*' \
		--exclude='linux_*' \
		--exclude='sunos_*' \
		--exclude='irix_*' \
		--exclude='netbsd_*' \
		--exclude='nextstep_*' \
		--exclude='*.xcodeproj' \
		--exclude='*.xcworkspace' \
		--exclude='xcuserdata' \
		--exclude='DerivedData' \
		--exclude='*.pbxuser' \
		--exclude='*.mode1v3' \
		--exclude='*.mode2v3' \
		--exclude='*.perspectivev3' \
		--exclude='*.xcuserstate' \
		.
	@echo "Archive created: ../ARCHIVE/cxlibs_unix.tar"

