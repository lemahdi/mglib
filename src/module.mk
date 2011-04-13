# Makefile for building the source module
# Need to define the local_dir variable each time when used in another module


# Directories
# ===========
local_dir := $(SRC)
local_lib := libmg
sub_dirs  := $(call module-subdirectories,$(local_dir))


# Source, generated files and third libraries
# ===========================================
local_src    := $(call module-sources,$(sub_dirs))

third_libs   := 
depend_libs  :=
#depend_libs  := $(addprefix $(LIBRARY_DIR)/lib,$(third_libs))
#depend_libs  := $(addsuffix .so,$(depend_libs))


scanner_file  := $(local_dir)/my_scanner.l
parser_file   := $(local_dir)/my_parser.ypp
out_scan_file := $(local_dir)/lex.yy.cpp
out_parse_f1  := $(basename $(parser_file)).tab.cpp
out_parse_f2  := $(basename $(parser_file)).tab.hpp
out_file      := $(out_scan_file) $(out_parse_f1) $(out_parse_f2)
out_file      += $(addprefix $(local_dir)/,position.hh location.hh stack.hh)


# Targets
# =======
generated := $(out_scan_file) $(out_parse_f1) $(out_parse_f2)
$(objects): $(generated)

$(out_scan_file): $(scanner_file)
	@echo $<
	@flex $<
	@sed 's/"lex.yy.c"/"lex.yy.cpp"/' lex.yy.c > $(out_scan_file)
	@rm lex.yy.c

$(out_parse_f1) $(out_parse_f2): $(parser_file)
	@echo $<
	@bison -d $<
	@mv *.tab.* src
	@mv *.hh src

local_src += $(notdir $(out_scan_file)) $(notdir $(out_parse_f1))

#> Building the library
$(eval $(call make-library,$(local_lib),$(local_src),$(depend_libs)))

#> .o and .d rules
$(eval $(call compile-rules))


.PHONY: extra_clean
extra_clean:
	@echo Cleaning generated files in src
	@$(RM) $(out_file)
