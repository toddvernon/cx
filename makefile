        
########################################################################################
# Figure out the OS
#
########################################################################################
# Platform detection + flags live in cx/platform.mk (single source of truth).
include platform.mk



########################################################################################
# Core libraries
#
########################################################################################


all:
	test -d ../lib || mkdir ../lib
	test -d ../lib/$(PLATFORM) || mkdir ../lib/$(PLATFORM)
	#mkdir -p ../lib/$(PLATFORM)

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

	rm -fr ../lib/$(PLATFORM)/*.a


########################################################################################
# Archives are built from the umbrella makefile (../makefile), not here:
#   make -C .. cxlibs_unix.tar     (this library)
#   make -C .. cxapps_unix.tar     (cm, psd, heliosAgent)
#   make -C .. cxtest_unix.tar     (cx_tests)
# One place to maintain the object/binary exclusion, and it ships cx/platform.mk.
########################################################################################

