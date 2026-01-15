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

# if this is darwin
ifeq ($(UNAME_S),darwin)
	ARCH := $(shell uname -m | tr '[A-Z]' '[a-z]' )
	CPPFLAGS = -D _OSX_  -g -Wno-deprecated -g -std=c++11
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

LIB_CX_TZ_NAME=libcx_tz.a
LIB_CX_TZ_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/civil_time_detail.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_fixed.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_format.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_if.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_impl.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_info.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_libc.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_lookup.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_posix.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/zone_info_source.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/date_time.o


###########################   Targets    #############################

ALL: MAKE_OBJ_DIR $(LIB_CX_TZ_NAME)

MAKE_OBJ_DIR:
	test -d $(UNAME_S)_$(ARCH) || mkdir $(UNAME_S)_$(ARCH)
    
$(LIB_CX_TZ_NAME): $(LIB_CX_TZ_OBJECTS) 
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_TZ_NAME)
	$(LIBRARIAN) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_TZ_NAME) $(LIB_CX_TZ_OBJECTS)
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; \
		then ranlib $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_TZ_NAME) ; fi
	cp $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_TZ_NAME) $(LIB_CX_PLATFORM_LIB_DIR)
	@echo "Done building static library "$(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_TZ_NAME)

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

$(LIB_CX_PLATFORM_OBJECT_DIR)/date_time.o			: date_time.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/civil_time_detail.o	: civil_time_detail.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_fixed.o	: time_zone_fixed.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_format.o  : time_zone_format.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_if.o		: time_zone_if.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_impl.o    : time_zone_impl.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_info.o	: time_zone_info.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_libc.o	: time_zone_libc.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_lookup.o  : time_zone_lookup.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/time_zone_posix.o   : time_zone_posix.cc
$(LIB_CX_PLATFORM_OBJECT_DIR)/zone_info_source.o	: zone_info_source.cc

$(LIB_CX_TZ_OBJECTS):
	$(CPP)  $(CPPFLAGS) $(INC) -c $? -o $@


