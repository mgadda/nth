CC=gcc
CFLAGS=-c -Wall
EXECUTABLE=nth
LEX=flex
YACC=bison -y
YFLAGS=-d

run: $(EXECUTABLE)
	./$(EXECUTABLE) ../scan_tests.nth

run_simple: $(EXECUTABLE)
	./$(EXECUTABLE) ../simple.nth

$(EXECUTABLE): parse.o scan.o ast.o main.o
	$(CC) $^ -o $@

scan.o: scan.l parse.c
parse.o: parse.y

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) *.o y.tab.h parse.c

