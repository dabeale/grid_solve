CXX=g++ -std=c++20
INCLUDE=-Ilib -Itests
RFLAGS=-Ofast -Wall -Werror -Wextra -Wpedantic
DFLAGS=-g -Wall -Werror -Wextra -Wpedantic -D_GLIBCXX_DEBUG
DOXY=doxygen

all: bin bin/test_release bin/test_debug docs
debug: bin bin/test_debug
release: bin bin/test_release
bin/test_release: tests/tests.cpp
	$(CXX) $(INCLUDE) $(RFLAGS) -o $@ $< $(LDLIBS)
bin/test_debug: tests/tests.cpp
	$(CXX) $(INCLUDE) $(DFLAGS) -o $@ $< $(LDLIBS)
bin:
	-mkdir bin
docs:
ifeq (, $(shell which dot))
	@echo "Unable to compile documentation"
	@echo "dot is not available, please install graphviz"
else ifeq (, $(shell which doxygen))
	@echo "Unable to compile documentation"
	@echo "doxygen is not available, please install"
else
	$(DOXY) config.dox
endif
clean:
	-rm -rf bin
	-rm -rf docs
