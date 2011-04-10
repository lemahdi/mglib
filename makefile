OUTPUT_BINARY = calculator
SRC_DIR = src
CC = g++ -g

scanner := my_scanner.l
parser := my_parser.ypp
scanner_out_c := lex.yy.cpp
parser_out_h := $(basename $(parser)).tab.hpp
parser_out_c := $(basename $(parser)).tab.cpp
parser_gen_files := position.hh location.hh stack.hh
sources := run.cpp $(shell ls $(SRC_DIR)/*.cpp)
objects := $(addsuffix .o, $(basename $(scanner_out_c) $(parser_out_c) $(sources)))

$(OUTPUT_BINARY): $(sources) $(scanner_out_c) $(parser_out_c)
	@echo $@
	$(CC) -o $@ $^ -I$(SRC_DIR) -L.. -lfl

$(scanner_out_c): $(scanner) $(parser_out_c)
	@echo $@
	flex $<
	sed 's/"lex.yy.c"/"lex.yy.cpp"/' lex.yy.c > $(scanner_out_c)
	rm lex.yy.c
	
$(parser_out_c): $(parser) $(sources)
	@echo $@
	bison -d $<

clean:
	rm $(parser_out_h)
	rm $(parser_out_c)
	rm $(scanner_out_c)
	rm $(parser_gen_files)

fool:
	@echo $(sources)
