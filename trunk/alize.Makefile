LIB_DIR=./lib/
LIB_DIR_DEBUG=./libdebug/
#***************************************
# DO NOT EDIT BELOW THIS LINES
#***************************************
###############################
#  Compiler options 
###############################
CPP=/usr/bin/g++

CXXFLAGS=-O3 -Wall -ffast-math
CXXFLAGS_DEBUG=-Wall -pg -ggdb
C=/usr/bin/gcc
CFLAGS=-O3 -Wall -ffast-math
CFLAGS_DEBUG=-Wall -pg -ggdb

ifeq ($(DEBUG),yes)
   CXXFLAGS=$(CXXFLAGS_DEBUG)
   CFLAGS=$(CFLAGS_DEBUG)
	LIB_DIR=$(LIB_DIR_DEBUG)
endif


###############################
#  Sources 
###############################
SRC_DIR=./src
HEADERS=$(wildcard $(SRC_DIR)/*.h)
SRC= $(wildcard $(SRC_DIR)/*.cpp)
OBJ= $(SRC:$(SRC_DIR)/%.cpp=$(SRC_DIR)/%.o) $(SRC_DIR)/misc.o $(SRC_DIR)/sig.o

###############################
# LIBS & INC DIR
###############################
INCLDIR = -I. -I$(SRC_DIR)
LIB_ALIZE = $(SRC_DIR)/libalize.a

###############################
# All target
###############################
all: spro $(LIB_ALIZE)

###############################
# Objs, Exec, Lib targets
###############################
$(LIB_ALIZE):$(OBJ) $(HEADERS)
	@echo "---------- Building Lib ----------------------";
	ar cru $@ $^
	ranlib $@
	cp $(LIB_ALIZE) $(LIB_DIR)
	@echo "---------- Done ----------------------";

###############################
#  Recursive targets
###############################
install: $(LIB_ALIZE) 
	@echo "----- Installing $^-------------";
	cp $(LIB_ALIZE) $(LIB_DIR)
clean: 
	@echo "----- Cleaning -------------";
	@rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.a

###############################
# Objs, Exec, Lib targets
###############################
%.o:%.cpp
	@echo "----- Compiling $^ -------------";
	$(CPP) $(INCLDIR) $(CXXFLAGS) -o $@ -c $< 

################################
# Building Spro
################################
spro: $(SRC_DIR)/sig.o $(SRC_DIR)/misc.o
$(SRC_DIR)/sig.o: 
	@echo "----- Compiling $^ -------------";
	$(C) $(INCLDIR) $(CFLAGS) -o $@ -c $(SRC_DIR)/sig.c
$(SRC_DIR)/misc.o: 
	@echo "----- Compiling $^ -------------";
	$(C) $(INCLDIR) $(CFLAGS) -o $@ -c $(SRC_DIR)/misc.c
.PRECIOUS: $(LIB_ALIZE)
