OUTPUT_BINARY = calculator
CC = g++

scanner := my_scanner.l
parser := my_parser.ypp
scanner_out_c := lex.yy.cpp
parser_out_h := $(basename $(parser)).tab.hpp
parser_out_c := $(basename $(parser)).tab.cpp
sources := run.cpp nodes.cpp

$(OUTPUT_BINARY): $(sources) $(scanner_out_c) $(parser_out_c)
	@echo $@
	$(CC) -o $@ $^ -L.. -lfl

$(scanner_out_c): $(scanner) $(parser_out_c)
	@echo $@
	flex $<
	sed 's/"lex.yy.c"/"lex.yy.cpp"/' lex.yy.c > $(scanner_out_c)
	rm lex.yy.c

$(scanner): nodes.h

$(sources): nodes.h
	
$(parser_out_c): $(parser)
	@echo $@
	bison -d $<

clean:

lclean: clean
	rm $(parser_out_h)
	rm $(parser_out_c)
	rm $(scanner_out_c)
