CXX        := g++
CXXFLAGS   := -Wall -g -O0

SRCS := test.cpp

PROGS := test

LDLIBS += -lcurl

default: ${PROGS}

clean:
	rm -f core.* ${PROGS} *~
