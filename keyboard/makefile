## makefile: libcx_base.a ######################################


CPP=        g++
SHELL=		/bin/sh
MOVE=		mv
COPY=		ln -s
RM=			rm -rf
TOUCH=		touch
LIBRARIAN=	ar rvu
INC=        -I../..


## Platform ###################################################


# get the OS
UNAME_S := $(shell uname -s | tr '[A-Z]' '[a-z]' )

# if this is linux
ifeq ($(UNAME_S),linux)
	ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
	CPPFLAGS = -D _LINUX_  -g -Wno-deprecated
endif

#if this is OSX
ifeq ($(UNAME_S), darwin)
	ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
	CPPFLAGS = -D _OSX_ -g -Wno-deprecated
endif

ifeq ($(UNAME_S), linux)
    CCFLAGS = -D _LINUX_
endif

ifeq ($(UNAME_S), darwin)
    CCFLAGS = -D _OSX_
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
        CPPFLAGS = -D _SOLARIS10_  -g
    endif
endif

#if this is IRIX
ifeq ($(UNAME_S),irix)
    UNAME_R := $(shell uname -r)
    ifeq ($(UNAME_R), 6.5)
        CPPFLAGS = -D _IRIX6_ -g
    endif
endif

#if this is NETBSD
ifeq ($(UNAME_S),netbsd)
	CPPFLAGS = -D _NETBSD_ -g
endif

ifeq ($(UNAME_S),nextstep)
    ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
    CPPFLAGS = -D _NEXT_ -D_POSIX_SOURCE=1 -g
endif

## Object #####################################################


LIB_CX_PLATFORM_LIB_DIR=../../lib/$(UNAME_S)_$(ARCH)
LIB_CX_PLATFORM_OBJECT_DIR=$(UNAME_S)_$(ARCH)

LIB_CX_KEYBOARD_NAME=libcx_keyboard.a
LIB_CX_KEYBOARD_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/keyboard.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/keyaction.o

## Targets ####################################################


ALL: MAKE_OBJ_DIR $(LIB_CX_KEYBOARD_NAME)

MAKE_OBJ_DIR:
	test -d $(UNAME_S)_$(ARCH) || mkdir $(UNAME_S)_$(ARCH)

$(LIB_CX_KEYBOARD_NAME): $(LIB_CX_KEYBOARD_OBJECTS) 
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_KEYBOARD_NAME)
	$(LIBRARIAN) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_KEYBOARD_NAME) $(LIB_CX_KEYBOARD_OBJECTS)
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; \
		then ranlib $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_KEYBOARD_NAME) ; fi
	cp $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_KEYBOARD_NAME) $(LIB_CX_PLATFORM_LIB_DIR)
	@echo "Done building static library "$(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_KEYBOARD_NAME)

cleanupall:
	$(RM) ._*
	$(RM) darwin_x86_64/*
	$(RM) darwin_arm64/*
	$(RM) linux/*
	$(RM) sunos/*

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

$(LIB_CX_PLATFORM_OBJECT_DIR)/keyaction.o	: keyaction.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/keyboard.o	: keyboard.cpp

$(LIB_CX_KEYBOARD_OBJECTS):
	$(CPP)  $(CPPFLAGS) $(INC) -c $? -o $@
