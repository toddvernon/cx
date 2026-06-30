## makefile: libcx_editbuffer.a ######################################


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

LIB_CX_EDITBUFFER_NAME=libcx_editbuffer.a
LIB_CX_EDITBUFFER_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/editbufferpos.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/editbuffer.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/edithint.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/stringlist.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/stringutils.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/editbufferlist.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/editline.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/utfstringlist.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/utfeditbuffer.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/utfeditbufferlist.o


## Targets ####################################################


ALL: MAKE_OBJ_DIR $(LIB_CX_EDITBUFFER_NAME)

MAKE_OBJ_DIR:
	test -d $(PLATFORM) || mkdir $(PLATFORM)

$(LIB_CX_EDITBUFFER_NAME): $(LIB_CX_EDITBUFFER_OBJECTS)
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EDITBUFFER_NAME)
	$(LIBRARIAN) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EDITBUFFER_NAME) $(LIB_CX_EDITBUFFER_OBJECTS)
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; \
		then ranlib $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EDITBUFFER_NAME) ; fi
	cp $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EDITBUFFER_NAME) $(LIB_CX_PLATFORM_LIB_DIR)
	@echo "Done building static library "$(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_EDITBUFFER_NAME)

cleanupall:
	$(RM) ._*
	$(RM) darwin/*
	$(RM) linux/*

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

$(LIB_CX_PLATFORM_OBJECT_DIR)/editbufferpos.o		: editbufferpos.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/editbuffer.o		: editbuffer.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/editbufferlist.o	: editbufferlist.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/edithint.o		: edithint.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/stringlist.o		: stringlist.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/stringutils.o		: stringutils.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/editline.o		: editline.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/utfstringlist.o		: utfstringlist.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/utfeditbuffer.o		: utfeditbuffer.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/utfeditbufferlist.o	: utfeditbufferlist.cpp


$(LIB_CX_EDITBUFFER_OBJECTS):
	$(CPP)  $(CPPFLAGS) $(INC) -c $? -o $@
