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


LIB_CX_PLATFORM_LIB_DIR=../../lib/$(PLATFORM)
LIB_CX_PLATFORM_OBJECT_DIR=$(PLATFORM)

LIB_CX_BASE_NAME=libcx_base.a
LIB_CX_BASE_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/string.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/buffer.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/rbuffer.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/prop.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/rule.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/exception.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/fileaccess.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/file.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/filei.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/time.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/star.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/cntbody.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/err.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/filename.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/directory.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/tokenizer.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/double.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/utfcharacter.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/utfstring.o


## Targets ####################################################


ALL: MAKE_OBJ_DIR $(LIB_CX_BASE_NAME)

MAKE_OBJ_DIR:
	test -d $(PLATFORM) || mkdir $(PLATFORM)
    
$(LIB_CX_BASE_NAME): $(LIB_CX_BASE_OBJECTS) 
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_BASE_NAME)
	$(LIBRARIAN) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_BASE_NAME) $(LIB_CX_BASE_OBJECTS) 
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; \
		then ranlib $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_BASE_NAME)/$(LIB_CX_BASE_NAME) ; fi
	cp $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_BASE_NAME) $(LIB_CX_PLATFORM_LIB_DIR)
	@echo "Done building static library "$(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_BASE_NAME)

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

$(LIB_CX_PLATFORM_OBJECT_DIR)/string.o 		: string.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/buffer.o		: buffer.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/rbuffer.o		: rbuffer.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/prop.o		: prop.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/rule.o		: rule.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/exception.o 	: exception.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/fileaccess.o	: fileaccess.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/file.o		: file.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/filei.o		: filei.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/time.o		: time.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/timeval.o		: timeval.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/star.o		: star.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/cntbody.o		: cntbody.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/err.o			: err.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/filename.o	: filename.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/directory.o	: directory.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/tokenizer.o 	: tokenizer.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/double.o		: double.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/utfcharacter.o	: utfcharacter.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/utfstring.o	: utfstring.cpp

$(LIB_CX_BASE_OBJECTS):
	$(CPP)  $(CPPFLAGS) $(INC) -c $? -o $@
