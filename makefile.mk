#
# Copyright				: 2011 by MG
# File					: makefile
# Version				: 0.1.22
# Last changed			: 10 APR 2011
# Purpose				: makefile
# Author				: MM Akkouh
# Notes					: based on GNU Make O'REILLY book, and improved
#


# This is the MG project makefile
# Possible targets are: build, clean
# Other variables:
#-- CONFIG: RELEASE, DEBUG
#-- FAST  : 0, 1
#-- 
# Each module in this project should have a makefile named module.mk


# Configuration
# =============
CONFIG		:= RELEASE
FAST		:= 0
TARGET_ARCH	:= $(shell uname -m)
ADDR		:= 32


# Checks
# ======
ifneq "$(CONFIG)" "RELEASE"
ifneq "$(CONFIG)" "DEBUG"
$(error Assertion failed: CONFIG should be DEBUG or RELEASE (def.))
endif
endif

ifneq "$(FAST)" "0"
ifneq "$(FAST)" "1"
$(error Assertion failed: FAST should be 1 or 0 (def.))
endif
endif


# Directories
# ===========
include_dirs := src
lib_dirs     := lib/$(CONFIG)
bin_dirs     := bin/$(CONFIG)
src_dirs     := src

INCLUDE_DIR	:= $(shell pwd)/$(include_dirs)
SOURCE_DIR  := $(shell pwd)/$(src_dirs)
LIBRARY_DIR  = $(shell pwd)/$(lib_dirs)
BINARY_DIR  := $(shell pwd)/$(bin_dirs)


# Shell commands
# ==============
MV    := mv -f
RM    := rm -f
SED   := sed
FLEX  := flex
BISON := bison -d
TEST  := test
MKDIR := mkdir -p
CC    := g++
AR    := ar -rv


# Modules names
# =============
SRC := src
APP := app

RUN := run


# Project variables
# =================
#> Collect information from each module in these 4 variables.
#> Initialize them as simple variables.
#> Use the command below to automatically generate the variable 'modules'
#> modules := $(subst /module.mk,,$(shell find . -name module.mk))
modules   := $(SRC) $(APP)
programs  := $(BINARY_DIR)/$(RUN)
libraries :=
archives  :=
sources   :=


# Functions
# =========

#-- $(call make-depend, source-file, object-file, depend-file)
define make-depend
	@$(CC) $(CFLAGS) $(CPPFLAGS) -MM -MG $$< |		\
	 $(SED) 's,\($$(notdir $2)\) *:,$2 $3: ,' > $3.tmp
	@$(SED) -e 's/#.*//'						\
	        -e 's/^[^:]*: *//'					\
	        -e 's/ *\\$$$$//'					\
	        -e '/^$$$$/ d'						\
	        -e 's/$$$$/ :/' $3.tmp >> $3.tmp
	@$(MV) $3.tmp $3
endef

#-- $(call make-library, library-name, source-file-list, third-libraries, depend-libraries)
#> Generating target, prequisites and rules for making a library
define make-library
  libraries += $(LIBRARY_DIR)/$1.so
  archives  += $(LIBRARY_DIR)/$1.a
  sources   += $2

  $(LIBRARY_DIR)/$1.so: $(call source-to-object,$2) $3
	@echo $$@
	@$(MKDIR) $(LIBRARY_DIR)
	@$(CC) -o $$@ $$^ $(LDFLAGS)
endef

#-- $(compile-rules)
#> Generating all object and dependency rules
define compile-rules
  $(foreach f, $(local_src),
    $(call one-compile-rule,$(call source-to-object,$f),$f))
endef

#-- $(call one-compile-rule, binary-file, source-file)
#> Generating rules for object and dependency file
define one-compile-rule
ifeq '$(FAST)' '0'
  $1: $2
	@echo $$@
	$(call make-depend,$$^,$$@,$(subst .o,.d,$1))
	@$(CC) -o $$@ -c $$< $(CPPFLAGS)
else
  $1: $2
	@echo $$@
	@$(CC) -o $$@ -c $$< $(CPPFLAGS)
endif
endef

#-- Object files from source files
source-to-object = $(call source-dir-to-binary-dir,			\
                     $(subst .cpp,.o,$(filter %.cpp,$1))	\
                     $(subst .c,.o,$(filter %.c,$1)))

#-- $(call source-dir-to-binary-dir, directory-list)
#> The binary tree
source-dir-to-binary-dir = $(addprefix $(BINARY_DIR)/, $1)

#-- Creating output directories
output-directories := $(addprefix $(bin_dirs)/,		\
		        $(sort								\
			  $(dir									\
			    $(shell for d in $(modules);		\
			      do								\
			        find $$d -name '*.cpp';			\
				find $$d -name '*.c';				\
			      done;))))
				
create-output-directories := $(shell for f in $(output-directories);	\
			       do													\
				 $(MKDIR) $$f;											\
			       done;)

#-- Subdirectories containing code files
module-subdirectories = .							\
                        $(sort						\
			  $(dir $(shell find $1 -name '*.cpp'))	\
			  $(dir $(shell find $1 -name '*.c')))

#-- Looking for .c and .cpp files
module-sources = $(foreach dir, $1, $(wildcard $(dir)*.cpp)) \
                 $(foreach dir, $1, $(wildcard $(dir)*.c))


# Object files
# ============
#> To use for building one library
#> PS: objects is already appended in each module.mk
objects = $(call source-to-object,$(sources))


# Dependency files
# ================
dependencies  = $(subst .o,.d, $(objects))
dependencies += $(BINARY_DIR)/$(APP)/$(RUN).d


# Flags
# =====

#-- Compile
CPPFLAGS := -w
ifeq "$(CONFIG)" "DEBUG"
  CPPFLAGS += -g -DDEBUG
endif
#ifeq "$(ADDR)" "64"
#  CPPFLAGS += -m64
#endif
CPPFLAGS += $(addprefix -I, $(INCLUDE_DIR))

#-- Link
LDFLAGS := -G
#ifeq "$(ADDR)" "64"
#  LDFLAGS += -m64
#endif
LDFLAGS += $(addprefix -L, $(lib_dirs))
#LDFLAGS += -ldl -lm -lrt -lnsl -lsocket -lCstd -lCrun -lfl
LDFLAGS += -lfl
LDFLAGS += $(addprefix -I, $(INCLUDE_DIR))


# Targets
# =======

#> To be sure that all is the default target
all:


#> Include makefiles for each module
ifneq "$(MAKECMDGOALS)" "help"
ifneq "$(MAKECMDGOALS)" "version"
  include $(addsuffix /module.mk,$(modules))
endif
endif


#> Pre-build

.PHONY: all
all: $(programs)


.PHONY: $(programs)
$(programs): $(archives)


#> There is an error here, bin/$(CONFIG)/app/run.o is part of libmg.a, it shouldn't
.PHONY: $(archives)
$(archives): $(objects)
	@echo $@
	@$(MKDIR) $(LIBRARY_DIR)
	@$(AR) $@ $^ > /dev/null

$(objects): $(generated)


#> Only parser
parser: $(generated)


#> Cleaning
.PHONY: clean
clean: pclean lclean dclean oclean extra_clean

.PHONY: pclean
pclean:
	@echo Cleaning programs
	@$(RM) $(programs)

.PHONY: lclean
lclean:
	@echo Cleaning libraries
	@$(RM) $(libraries)
	@$(RM) $(archives)

.PHONY: dclean
dclean:
	@echo Cleaning dependency files
	@$(RM) $(dependencies)

.PHONY: oclean
oclean:
	@echo Cleaning objects files
	@$(RM) $(objects)


.PHONY: version
version:
	@echo $(MAKE_VERSION)


#> Help
.PHONY: help
help:
	@echo "Use the following targets:"
	@echo "    all: default target for an all build"
	@echo "    pclean: removing the executable file"
	@echo "    lclean: removing the libraries from lib/CONFIG"
	@echo "    dclean: removing the dependency files"
	@echo "    oclean: removing the object files"
	@echo "    extra_clean: removing generated files"
	@echo "    clean: all preceding clean targets are executed"
	@echo "    version: display the Make version (should be >= 3.81)"
	@echo ""
	@echo "Use the following options:"
	@echo "    CONFIG: possible values are RELEASE (default) and DEBUG"
	@echo "    FAST: possible values are 0 (default) and 1. If 1, the dependency rules "
	@echo "        are simplified to .o<->.cpp relationship and #include are not taken"
	@echo "        into account."

# Dependency files
# ================

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "help"
ifneq "$(MAKECMDGOALS)" "version"
ifeq  "$(FAST)"         "0"
  -include $(dependencies)
endif
endif
endif
endif


