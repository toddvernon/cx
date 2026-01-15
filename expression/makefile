## makefile: libcx_expression.a ######################################


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

#if this is linux
ifeq ($(UNAME_S),linux)
	ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
	CPPFLAGS = -D _LINUX_  -g -Wno-deprecated -Wwrite-strings
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

    CPPFLAGS = -D _SOLARIS6_  -g

    ifeq ($(UNAME_R), 4.1.3) 
        CPPFLAGS = -D _SUNOS_ -g
    endif

    ifeq ($(UNAME_R), 4.1.4) 
        CPPFLAGS = -D _SUNOS_ -g
    endif

endif

#if this is NETBSD
ifeq ($(UNAME_S),netbsd)
	CPPFLAGS = -D _NETBSD_ -g -Wno-deprecated -Wwrite-strings
endif

ifeq ($(UNAME_S),nextstep)
    ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
    CPPFLAGS = -D _NEXT_ -D_POSIX_SOURCE=1 -g
endif


## Object #####################################################

LIB_CX_PLATFORM_LIB_DIR=../../lib/$(UNAME_S)_$(ARCH)
LIB_CX_PLATFORM_OBJECT_DIR=$(UNAME_S)_$(ARCH)

LIB_CX_EXPRESSION_NAME=libcx_expression.a
LIB_CX_EXPRESSION_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/token.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/funcdb.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/vardb.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/scanDouble.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/expression.o


## Targets ####################################################


ALL: MAKE_OBJ_DIR $(LIB_CX_EXPRESSION_NAME)

MAKE_OBJ_DIR:
	test -d $(UNAME_S)_$(ARCH) || mkdir $(UNAME_S)_$(ARCH)



$(LIB_CX_EXPRESSION_NAME): $(LIB_CX_EXPRESSION_OBJECTS) 
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EXPRESSION_NAME)
	$(LIBRARIAN) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EXPRESSION_NAME) $(LIB_CX_EXPRESSION_OBJECTS)
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; \
		then ranlib $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EXPRESSION_NAME) ; fi
	cp $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EXPRESSION_NAME) $(LIB_CX_PLATFORM_LIB_DIR)
	@echo "Done building static library "$(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EXPRESSION_NAME)

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

## Conversions ################################################


.PRECIOUS: $(CX_LIBS)

.SUFFIXES: .cpp .C .cc .cxx .o

$(LIB_CX_PLATFORM_OBJECT_DIR)/expression.o	: expression.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/token.o		: token.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/scanDouble.o	: scanDouble.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/vardb.o		: vardb.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/funcdb.o		: funcdb.cpp


$(LIB_CX_EXPRESSION_OBJECTS):
	$(CPP)  $(CPPFLAGS) $(INC) -c $? -o $@
