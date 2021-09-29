CC=gcc
CFLAGS= -lstdc++ 

.PHONY: all
all: nyush

nyush: nyush.o utils.o LsCmd.o CmdBase.o
	$(CC) -o $@ $^ $(CFLAGS)

nyush.o: nyush.cpp

utils.o: utils.cpp utils.hpp

LsCmd.o: LsCmd.cpp LsCmd.hpp

CmdBase.o: CmdBase.cpp CmdBase.hpp

.PHONY: clean
clean:
	rm -f *.o nyush