# Makefile for building the source module
# Need to define the local_dir variable each time when used in another module


# Directories
# ===========
local_dir := $(SRC)
win_dirs  := xlw
local_lib := libmg
sub_dirs  := $(call module-subdirectories,$(local_dir))
sub_dirs  := $(shell echo $(sub_dirs) | awk -F"/" 'BEGIN { RS=" " } $$2!~"$(win_dirs)" { print $$0; }')


# Source, generated files and third libraries
# ===========================================
local_src    := $(call module-sources,$(sub_dirs))

third_libs   := 
depend_libs  :=
#depend_libs  := $(addprefix $(LIBRARY_DIR)/lib,$(third_libs))
#depend_libs  := $(addsuffix .so,$(depend_libs))


scanner_file  := $(local_dir)/genpricer/infra/my_scanner.l
parser_file   := $(local_dir)/genpricer/infra/my_parser.ypp
out_scan_file := $(local_dir)/genpricer/infra/lex.yy.cpp
out_parse_f1  := $(basename $(parser_file)).tab.cpp
out_parse_f2  := $(basename $(parser_file)).tab.hpp
out_file      := $(out_scan_file) $(out_parse_f1) $(out_parse_f2)
out_file      += $(addprefix $(local_dir)/genpricer/infra/,position.hh location.hh stack.hh)


# Targets
# =======
generated := $(out_scan_file) $(out_parse_f1) $(out_parse_f2)

$(out_scan_file): $(scanner_file)
	@echo $<
	@$(FLEX) $<
	@$(SED) 's/"lex.yy.c"/"lex.yy.cpp"/' lex.yy.c > $@.tmp
	@$(SED) 's/\(#include <unistd.h>\)/#ifndef WIN32\n\1\n#endif/' $@.tmp > $@
	@$(RM) lex.yy.c $@.tmp

$(out_parse_f1) $(out_parse_f2): $(parser_file)
	@echo $<
	@$(BISON) $<
	@$(MV) *.tab.* src/genpricer/infra
	@$(MV) *.hh src/genpricer/infra

#local_src    := $(call module-sources,$(sub_dirs))
local_src += $(out_scan_file) $(out_parse_f1)

#> Building the library
$(eval $(call make-library,$(local_lib),$(local_src),$(depend_libs)))

#> .o and .d rules
$(eval $(call compile-rules))


.PHONY: extra_clean
extra_clean:
	@echo Cleaning generated files in src
	@$(RM) $(out_file)
