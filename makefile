OUTPUT_BINARY = calculator
CC = g++

$(OUTPUT_BINARY): lex.yy.c cppcalc.tab.cc
	@echo $@
	$(CC) -o $@ $^ -L.. -lfl

lex.yy.c: cppcalc.l cppcalc.tab.cc
	@echo $@
	flex $<
	
cppcalc.l: cppcalc-ctx.hh
	
cppcalc.tab.cc: cppcalc.yy
	@echo $@
	bison -d $<

clean:

lclean: clean
	rm cppcalc.tab.hh
	rm cppcalc.tab.cc
	rm lex.yy.c
