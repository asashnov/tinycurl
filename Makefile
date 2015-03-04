CC := g++

PROG := test

OBJECTS := tinycurl.o test.o

CXXFLAGS := -Wall -g -O0

LDLIBS += -lcurl


default: ${PROG}

all: ${PROG}


$(PROG): $(OBJECTS)

$(OBJECTS): tinycurl.h

.PHONY : clean
clean:
	rm -f core.* ${PROGS} ${OBJECTS} *~
