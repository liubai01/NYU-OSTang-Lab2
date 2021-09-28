CC=gcc

.PHONY: all
all: nyush

nyush: nyush.o

nyuc.o: nyush.c nyush.h

.PHONY: clean
clean:
	rm -f *.o nyush
