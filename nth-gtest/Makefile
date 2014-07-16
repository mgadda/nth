
CC=clang
CXX=c++
CFLAGS=-Wall -I../nth -I../include
CXXFLAGS=-Wall -std=c++11 -I../nth -I../include
EXECUTABLE=nth-tests
LDFLAGS=-L ../lib -L ../nth
LDLIBS=-lgtest -lnth

test: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): driver_test.o main.o objc_helper.o ../nth/libnth.a
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) *.o -o $(EXECUTABLE)

../nth/libnth.a:
	$(MAKE) -C ../nth libnth.a

.DEFAULT_GOAL := test

clean:
	rm -rf *.o $(EXECUTABLE)
	$(MAKE) -C ../nth clean