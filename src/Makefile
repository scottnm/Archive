CC=g++
CPPFLAGS=-g -c -std=c++11 -Wall -Werror -Wpedantic -Wextra -Wfatal-errors
LDFLAGS=
OBJECTS=byte_buffer.o \
		load_image.o \
		main.o
EXECUTABLE=twodthreed

GTEST_ROOT=~/dev/googletest/googletest
TESTCPPFLAGS=-std=c++11 -Wall -Werror -Wpedantic -Wextra -Wfatal-errors -isystem $(GTEST_ROOT) -isystem $(GTEST_ROOT)/include
TESTLDFLAGS=$(GTEST_ROOT)/make/gtest_main.a -lpthread

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

load_image.o : bmp_constants.h bitmap.h

test: byte_buffer_test.cpp byte_buffer.cpp
	$(CC) $(TESTCPPFLAGS) byte_buffer_test.cpp byte_buffer.cpp -o test $(TESTLDFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) test
