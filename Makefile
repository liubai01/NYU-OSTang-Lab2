CC=gcc
CFLAGS= -lstdc++ 

.PHONY: all
all: nyush

nyush: nyush.o utils.o CdCmd.o CmdBase.o
	$(CC) -o $@ $^ $(CFLAGS)

nyush.o: nyush.cpp

utils.o: utils.cpp utils.hpp

CdCmd.o: CdCmd.cpp CdCmd.hpp

CmdBase.o: CmdBase.cpp CmdBase.hpp

.PHONY: clean
clean:
	rm -f *.o nyush