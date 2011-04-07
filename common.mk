####################
#                  #
# GENERIC MAKEFILE #
#                  #
####################

PATH += :/usr/sfw/bin
LD_LIBRARY_PATH += :/usr/sfw/lib

DEBUG := no

CC := CC

export TRY_DIR := $(shell pwd)
LIB := lib

#THIRD_LIBS :=
SUB_DIRS := . 
SUB_DIRS += $(sort $(dir $(shell find . -name '*.cpp')))
SUB_DIRS += $(sort $(dir $(shell find . -name '*.c')))

INCLUDES := -I.

#Flags
ifeq ($(DEBUG),yes)
	CFLAGS = -w -g -DDEBUG -KPIC -features=extensions $(INCLUDES)
else
	CFLAGS = -w $(INCLUDES)
endif
LDFLAGS = $(LIBS) -ldl -lm -lrt -L/usr/sfw/lib -lfl
#-L/usr/ccs/lib -ll: lex
#-L/usr/sfw/lib -lfl: flex

#Lex files
LEX_SRC := $(foreach dir, $(SUB_DIRS), $(wildcard $(dir)*.l))

#Source files
C_SRC := $(foreach dir, $(SUB_DIRS), $(wildcard $(dir)*.c))
CPP_SRC := $(foreach dir, $(SUB_DIRS), $(wildcard $(dir)*.cpp))
SRC := $(C_SRC) $(CPP_SRC)

#Orphan header files
ORPHAN_H := $(filter-out $(addsuffix .h, $(basename $(SRC))), $(wildcard *.h))

#Object files
C_OBJ := $(addsuffix .o, $(basename $(C_SRC)))
CPP_OBJ := $(addsuffix .o, $(basename $(CPP_SRC)))
LEX_OBJ := $(addsuffix .o, $(basename $(LEX_SRC)))
OBJ := $(C_OBJ) $(CPP_OBJ) $(LEX_OBJ)

.PHONY: $(OUTPUT_BINARY)
	
all: $(OUTPUT_BINARY)
ifeq ($(DEBUG),yes)
	@echo "DEBUG"
else
	@echo "RELEASE"
endif

$(OUTPUT_BINARY): $(OBJ)
	@echo $@
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(ORPHAN_H)
	@echo $@
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.c $(ORPAHN_H)
	@echo $@
	$(CC) -o $@ -c $< $(CFLAGS)
	
%.c: %.l
	@echo $@
	flex $<
	mv lex.yy.c $@
	
%.tab.c: %.y
	@echo $@
	bison -d $<

clean:
	rm -f $(OBJ)

pclean: clean
	rm -rf $(OUTPUT_BINARY)

