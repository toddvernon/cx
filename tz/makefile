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


# Platform detection + flags live in cx/platform.mk (single source of truth).
include ../platform.mk


## Object #####################################################

# cctz (this library) requires C++11. The cx CPPFLAGS carry no -std, so tz
# would otherwise rely on the compiler's DEFAULT standard being new enough
# (gcc >= 6 defaults to gnu++14; older-but-capable gcc 4.8-5.x default to
# gnu++98 and would fail). Pin it explicitly so tz builds on any C++11 compiler.
# The pre-C++11 boxes (SunOS 4.x, Solaris 2.6 g++ 2.95, IRIX) never reach here:
# the parent cx/makefile gates tz off those platforms.
TZ_STD = -std=c++11

LIB_CX_PLATFORM_LIB_DIR=../../lib/$(PLATFORM)
LIB_CX_PLATFORM_OBJECT_DIR=$(PLATFORM)

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
	test -d $(PLATFORM) || mkdir $(PLATFORM)
    
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
	$(CPP)  $(CPPFLAGS) $(TZ_STD) $(INC) -c $? -o $@


