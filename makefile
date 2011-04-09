OUTPUT_BINARY = calculator
CC = CC

$(OUTPUT_BINARY): lex.yy.c parser.tab.c
	@echo $@
	$(CC) -o $@ $^ -L.. -lfl

lex.yy.c: scanner.l parser.tab.c
	@echo $@
	flex $<
	
parser.tab.c: parser.y
	@echo $@
	bison -d $<

clean:
	rm *.o

lclean: clean
	rm parser.tab.h
	rm parser.tab.c
	rm lex.yy.c
