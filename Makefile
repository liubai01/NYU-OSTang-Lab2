CC=gcc
CFLAGS= -lstdc++ 

.PHONY: all
all: nyush

nyush: nyush.o utils.o
	$(CC) -o $@ $^ $(CFLAGS)

nyush.o: nyush.cpp nyush.hpp

utils.o: utils.cpp utils.hpp

.PHONY: clean
clean:
	rm -f *.o nyush