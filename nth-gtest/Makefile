
CC=clang
CXX=c++
CFLAGS=-Wall -I../nth -I../include
CXXFLAGS=-Wall -std=c++11 -I../nth -I../include
EXECUTABLE=nth-tests
LDFLAGS=-L ../lib -L ../nth
LDLIBS=-lgtest -lnth

test: $(EXECUTABLE)
	./$(EXECUTABLE)

cc_files := $(wildcard *.cc)

$(EXECUTABLE): $(cc_files) ../nth/libnth.a
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(cc_files) -o $(EXECUTABLE)

../nth/libnth.a:
	$(MAKE) -C ../nth libnth.a

.DEFAULT_GOAL := test

.PHONY: clean
clean:
	rm -rf *.o $(EXECUTABLE)