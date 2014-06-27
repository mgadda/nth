CC=clang
CXX=clang++
CFLAGS=-Wall
CXXFLAGS=-Wall
EXECUTABLE=nth
LEX=flex
YACC=bison
YFLAGS=-d --report=all

$(EXECUTABLE): parse.o driver.o ast.o main.o scan.o
	$(CXX) $(CXXFLAGS) $^ -o $@

scan.o: scan.l parse.tab.cc 
	$(LEX) -t $< > $*.cc
	$(CXX) $(CXXFLAGS) $*.cc -c

parse.o parse.tab.cc: parse.y
	$(YACC) $(YFLAGS) $^ -o $*.cpp
	$(CXX) $(CXXFLAGS) parse.cpp -o $@ -c

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) *.o \
		location.hh position.hh stack.hh \
		parse.cpp parse.hpp parse.tab.* parse.output scan.c

