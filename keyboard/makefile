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

LIB_CX_KEYBOARD_NAME=libcx_keyboard.a
LIB_CX_KEYBOARD_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/keyboard.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/keyaction.o

## Targets ####################################################


ALL: MAKE_OBJ_DIR $(LIB_CX_KEYBOARD_NAME)

MAKE_OBJ_DIR:
	test -d $(PLATFORM) || mkdir $(PLATFORM)

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
