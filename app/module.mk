# Makefile for building the app module
# Need to define the local_dir variable each time when used in another module


# Directories
# ===========
local_dir := $(APP)
win_dirs  := mgxll
sub_dirs  := $(call module-subdirectories,$(local_dir))
sub_dirs  := $(shell echo $(sub_dirs) | awk -F"/" 'BEGIN { RS=" " } $$2!~"$(win_dirs)" { print $$0; }')



# Source, generated files and third libraries
# ===========================================
local_src    := $(call module-sources,$(sub_dirs))

#third_libs   := $(ARC)
#l_third_libs := $(addprefix -l,$(third_libs))
depend_libs   :=
#depend_libs  := $(addprefix $(LIBRARY_DIR)/lib,$(third_libs))
#depend_libs  := $(addsuffix .a,$(depend_libs))
#depend_libs  += $(addsuffix .so,$(LIBRARY_DIR)/lib$(SRL))


# Targets
# =======

#> .o and .d rules
$(eval $(call compile-rules))

local_obj := $(call source-to-object,$(local_src))
$(local_obj) : $(archives)

$(programs): $(local_obj)
	@echo $@
	@$(CC) -o $@ $(CPPFLAGS) -L$(lib_dirs) $^
