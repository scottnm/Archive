CC=g++
CPPFLAGS=-g -c -std=c++11 -Wall -Werror -Wpedantic -Wextra -Wfatal-errors
LDFLAGS=
OBJECTS=byte_buffer.o \
		load_image.o \
		main.o
EXECUTABLE=twodthreed

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

load_image.o : bmp_constants.h bitmap.h

clean:
	rm $(OBJECTS) $(EXECUTABLE)
