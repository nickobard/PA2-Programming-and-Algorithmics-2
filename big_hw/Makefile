CXX=g++
LD=g++
AR=ar
CXXFLAGS=-std=c++20 -Wall -pedantic
SHELL:=/bin/bash
MACHINE=$(shell uname -m)-$(shell echo $$OSTYPE)

release: CXXFLAGS += -O2
release: test

sanitizer: CXXFlAGS += -fsanitize=address
sanitizer: LD += -fsanitize=address
sanitizer: test

all: CXXFLAGS += -ggdb
all: test

deps:
	g++ -MM *.cpp > Makefile.d

pt_all: pt_compile

pt_tinker:
	./src/assets/progt.sh ./src/assets/solution.cpp ./src

pt_compile: progtest.o sample_tester.o
	$(LD) $(CXXFLAGS) -o $@ $^ -L./$(MACHINE) -lexpression_parser

test: main.o
	$(LD) $(CXXFLAGS) -o $@ $^ -L./$(MACHINE) -lexpression_parser

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lib: expression_parser.o
	mkdir -p $(MACHINE)
	$(AR) cfr $(MACHINE)/expression_parser.a $^

clean:
	rm -f *.o test *~ core sample.tgz Makefile.d
	rm -f progtest.cpp
	rm -f pt_compile

-include Makefile.d

