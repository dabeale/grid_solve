CXX=g++ -std=c++20 
INCLUDE=-Ilib -Itests
RFLAGS=-Ofast -Wall -fanalyzer -Wno-analyzer-possible-null-dereference -Wno-analyzer-null-dereference
DFLAGS=-g -Og -Wall

all: test_release test_debug

test_release: tests/tests.cpp
	$(CXX) $(INCLUDE) $(RFLAGS) -o $@ $< $(LDLIBS)

test_debug: tests/tests.cpp
	$(CXX) $(INCLUDE) $(DFLAGS) -o $@ $< $(LDLIBS)

clean:
	-rm -f test_release
	-rm -f test_debug
