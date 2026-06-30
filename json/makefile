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

LIB_CX_JSON_NAME=libcx_json.a
LIB_CX_JSON_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_base.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_string.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_number.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_boolean.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_null.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_member.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_object.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_array.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_factory.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_base.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_string.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_number.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_boolean.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_null.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_member.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_object.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_array.o

NXJSON_OBJECTS=$(LIB_CX_PLATFORM_OBJECT_DIR)/nxjson.o

###########################   Targets    #############################

ALL: MAKE_OBJ_DIR $(NXJSON_OBJECTS) $(LIB_CX_JSON_NAME)
	
MAKE_OBJ_DIR:
	test -d $(PLATFORM) || mkdir $(PLATFORM)

$(LIB_CX_JSON_NAME): $(LIB_CX_JSON_OBJECTS) $(NXJSON_OBJECTS)
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_JSON_NAME)
	$(LIBRARIAN) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_JSON_NAME) $(LIB_CX_JSON_OBJECTS) $(NXJSON_OBJECTS)
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; \
		then ranlib $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_JSON_NAME) ; fi
	cp $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_JSON_NAME) $(LIB_CX_PLATFORM_LIB_DIR)
	@echo "Done building static library "$(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_JSON_NAME)


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

.SUFFIXES: .cpp .C .cc .cxx .o .c

$(LIB_CX_PLATFORM_OBJECT_DIR)/json_base.o	 	: json_base.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_string.o 	: json_string.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_number.o 	: json_number.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_boolean.o 	: json_boolean.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_null.o		: json_null.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_member.o		: json_member.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_object.o		: json_object.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_array.o 		: json_array.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_factory.o	: json_factory.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_base.o	: json_utf_base.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_string.o	: json_utf_string.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_number.o	: json_utf_number.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_boolean.o	: json_utf_boolean.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_null.o	: json_utf_null.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_member.o	: json_utf_member.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_object.o	: json_utf_object.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/json_utf_array.o	: json_utf_array.cpp

$(LIB_CX_PLATFORM_OBJECT_DIR)/nxjson.o: nxjson.c
	gcc -c -O0 -g -Wall ${CPPFLAGS} nxjson.c -o $(LIB_CX_PLATFORM_OBJECT_DIR)/nxjson.o

$(LIB_CX_JSON_OBJECTS):
	$(CPP)  $(CPPFLAGS) $(INC) -c $? -o $@


