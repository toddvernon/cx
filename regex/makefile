## makefile: libcx_regex.a ######################################
##
## Linux and macOS only - POSIX regex wrapper
##

CPP=        g++
SHELL=		/bin/sh
MOVE=		mv
COPY=		ln -s
RM=			rm -rf
TOUCH=		touch
LIBRARIAN=	ar rvu
INC=        -I../..


## Platform ###################################################

# Platform detection + flags live in cx/platform.mk (single source of truth).
include ../platform.mk

# regex is a POSIX-regex wrapper; build it only on Linux and macOS (see
# PLATFORM_SUPPORT.md). This module-local capability gate stays here, not in
# platform.mk -- platform.mk answers "what OS/arch am I", the module answers
# "do I build here". The ALL target below is wrapped in ifeq($(BUILD_REGEX),1).
BUILD_REGEX=0
ifeq ($(UNAME_S),linux)
	BUILD_REGEX=1
endif
ifeq ($(UNAME_S),darwin)
	BUILD_REGEX=1
endif


## Object #####################################################

LIB_CX_PLATFORM_LIB_DIR=../../lib/$(PLATFORM)
LIB_CX_PLATFORM_OBJECT_DIR=$(PLATFORM)

LIB_CX_REGEX_NAME=libcx_regex.a
LIB_CX_REGEX_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/regex.o


###########################   Targets    #############################

ifeq ($(BUILD_REGEX),1)

ALL: MAKE_OBJ_DIR $(LIB_CX_REGEX_NAME)

MAKE_OBJ_DIR:
	test -d $(PLATFORM) || mkdir $(PLATFORM)

$(LIB_CX_REGEX_NAME): $(LIB_CX_REGEX_OBJECTS)
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_REGEX_NAME)
	$(LIBRARIAN) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_REGEX_NAME) $(LIB_CX_REGEX_OBJECTS)
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; \
		then ranlib $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_REGEX_NAME) ; fi
	cp $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_REGEX_NAME) ../../lib/$(PLATFORM)
	@echo "Done building static library "$(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_REGEX_NAME)

else

ALL:
	@echo "CxRegex: skipping build (not Linux/macOS, UNAME_S=$(UNAME_S))"

endif


cleanupall:
	$(RM) ._*
	$(RM) darwin_x86_64/*
	$(RM) darwin_arm64/*
	$(RM) linux/*
	$(RM) linux_x86_64/*

cleanupmac:
	$(RM) ._*

clean:
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/*.o \
	$(LIB_CX_PLATFORM_OBJECT_DIR)/*.dbx \
	$(LIB_CX_PLATFORM_OBJECT_DIR)/*.i \
	$(LIB_CX_PLATFORM_OBJECT_DIR)/*.ixx \
	$(LIB_CX_PLATFORM_OBJECT_DIR)/core \
	$(LIB_CX_PLATFORM_OBJECT_DIR)/a.out \
	$(LIB_CX_PLATFORM_OBJECT_DIR)/*.a


##########################  Conversions   ########################


.PRECIOUS: $(CX_LIBS)

.SUFFIXES: .cpp .C .cc .cxx .o

$(LIB_CX_PLATFORM_OBJECT_DIR)/regex.o : regex.cpp

$(LIB_CX_REGEX_OBJECTS):
	$(CPP) $(CPPFLAGS) $(INC) -c $? -o $@
