CXX=g++ -std=c++20 
INCLUDE=-Ilib -Itests
RFLAGS=-Ofast -Wall
DFLAGS=-g -Og -Wall

all: bin bin/test_release bin/test_debug
bin/test_release: tests/tests.cpp
	$(CXX) $(INCLUDE) $(RFLAGS) -o $@ $< $(LDLIBS)
bin/test_debug: tests/tests.cpp
	$(CXX) $(INCLUDE) $(DFLAGS) -o $@ $< $(LDLIBS)
bin:
	-mkdir bin
clean:
	-rm -rf bin
