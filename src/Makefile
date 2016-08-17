CC=g++
CPPFLAGS=-g -c -std=c++11 -Wall -Werror -Wpedantic -Wextra -Wfatal-errors
LDFLAGS=
OBJECTS=byte_buffer.o \
		load_image.o \
		main.o
EXECUTABLE=twodthreed

GTEST_ROOT=~/dev/googletest/googletest
TESTCPPFLAGS=-isystem $(GTEST_ROOT) -isystem $(GTEST_ROOT)/include
#TESTLDFLAGS=-lgtest -lgtest_main -lpthread
#TESTLDFLAGS=$(GTEST_ROOT)/make/gtest.a $(GTEST_ROOT)/make/gtest_main.a -lpthread
TESTLDFLAGS=$(GTEST_ROOT)/make/gtest_main.a -lpthread

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

load_image.o : bmp_constants.h bitmap.h

test: test.cpp testsrc.cpp testsrc.h
	g++ $(TESTCPPFLAGS) test.cpp testsrc.cpp -o test $(TESTLDFLAGS)

clean:
	rm $(OBJECTS) $(EXECUTABLE) test
