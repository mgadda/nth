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

scan.o: scan.l parse.cc 
	$(LEX) -t $< > $*.cc
	$(CXX) $(CXXFLAGS) $*.cc -c

parse.o parse.tab.cc: parse.y
	$(YACC) $(YFLAGS) $^ -o $*.cc
	$(CXX) $(CXXFLAGS) parse.cc -o $@ -c

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) *.o \
		location.hh position.hh stack.hh \
		parse.cc parse.hh parse.tab.* parse.output scan.cc

