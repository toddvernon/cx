## makefile: libcx_sheetmodel.a ######################################


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

LIB_CX_SHEETMODEL_NAME=libcx_sheetmodel.a
LIB_CX_SHEETMODEL_OBJECTS=\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetCellCoordinate.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetCellRange.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetCell.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetVariableDatabase.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetFunctionDatabase.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetDependencyGraph.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetInputParser.o\
	$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetModel.o


## Targets ####################################################


ALL: MAKE_OBJ_DIR $(LIB_CX_SHEETMODEL_NAME)

MAKE_OBJ_DIR:
	test -d $(PLATFORM) || mkdir $(PLATFORM)

$(LIB_CX_SHEETMODEL_NAME): $(LIB_CX_SHEETMODEL_OBJECTS)
	$(RM) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_SHEETMODEL_NAME)
	$(LIBRARIAN) $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_SHEETMODEL_NAME) $(LIB_CX_SHEETMODEL_OBJECTS)
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ] ; \
		then ranlib $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_SHEETMODEL_NAME) ; fi
	cp $(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_SHEETMODEL_NAME) $(LIB_CX_PLATFORM_LIB_DIR)
	@echo "Done building static library "$(LIB_CX_PLATFORM_OBJECT_DIR)/$(LIB_CX_SHEETMODEL_NAME)

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

$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetCellCoordinate.o	: sheetCellCoordinate.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetCellRange.o		: sheetCellRange.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetCell.o		: sheetCell.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetVariableDatabase.o	: sheetVariableDatabase.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetFunctionDatabase.o	: sheetFunctionDatabase.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetDependencyGraph.o	: sheetDependencyGraph.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetInputParser.o	: sheetInputParser.cpp
$(LIB_CX_PLATFORM_OBJECT_DIR)/sheetModel.o 		: sheetModel.cpp

$(LIB_CX_SHEETMODEL_OBJECTS):
	$(CPP)  $(CPPFLAGS) $(INC) -c $? -o $@
