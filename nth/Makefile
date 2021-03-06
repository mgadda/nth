CC=clang
CXX=c++
CFLAGS=-Wall
CXXFLAGS=-Wall -std=c++11
EXECUTABLE=nth
LEX=flex
YACC=bison
YFLAGS=-fcaret -d --report=all -x -g

$(EXECUTABLE): libnth.a main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

libnth.a: libnth.a(scan.o parse.o driver.o ast.o type.o type_literal.o scope_checker.o type_checker.o symbol_table.o ast_visitor.o ast_string_printer.o ast_dot_printer.o)

parse.html: parse.o
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl parse.xml > parse.html
	open parse.html

parse.png: parse.o
	dot parse.dot -Tpng -oparse.png
	open parse.png

test: $(EXECUTABLE)
	./$(EXECUTABLE) ../nth.nth # --debug-scan --debug-parse ../nth.nth

scan.o: scan.l parse.o
	$(LEX) --header-file=$*.h -o $*.cc $<
	$(CXX) $(CXXFLAGS) $*.cc -c

parse.o parse.tab.cc: parse.y
	$(YACC) $(YFLAGS) $^ -o $*.cc
	$(CXX) $(CXXFLAGS) parse.cc -o $@ -c

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) *.o \
		libnth.a \
		location.hh position.hh stack.hh \
		parse.cc parse.hh parse.tab.* parse.output \
		scan.cc scan.h \
		parse.xml parse.png parse.dot parse.html

