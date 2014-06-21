CC=gcc
CFLAGS=-c -Wall
EXECUTABLE=nth
LEX=flex
YACC=bison -y
YFLAGS=-d --report=all

run: $(EXECUTABLE)
	./$(EXECUTABLE) ../nth.nth

run_simple: $(EXECUTABLE)
	./$(EXECUTABLE) ../simple.nth

$(EXECUTABLE): parse.o scan.o ast.o main.o
	$(CC) $^ -o $@

scan.o: scan.l parse.c
parse.o: parse.y

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) *.o y.tab.h parse.c

