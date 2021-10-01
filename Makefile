CC=gcc
CFLAGS=-lstdc++ 
SOURCEDIR = src
BUILDDIR = build
OBJECTS = utils CdCmd CmdBase NyuShell

.PHONY: all
all: nyush

nyush: nyush.o $(addsuffix .o, $(OBJECTS))
	$(CC) -o $@ $(addprefix $(BUILDDIR)/,$^) $(CFLAGS)

nyush.o: $(SOURCEDIR)/nyush.cpp
	$(CC) -c -o $(BUILDDIR)/$@ $^ $(CFLAGS)

utils.o: $(SOURCEDIR)/utils.cpp $(SOURCEDIR)/utils.hpp
	$(CC) -c -o $(BUILDDIR)/$@ $< $(CFLAGS)

CdCmd.o: $(SOURCEDIR)/CdCmd.cpp $(SOURCEDIR)/CdCmd.hpp
	$(CC) -c -o $(BUILDDIR)/$@ $< $(CFLAGS)

CmdBase.o: $(SOURCEDIR)/CmdBase.cpp $(SOURCEDIR)/CmdBase.hpp
	$(CC) -c -o $(BUILDDIR)/$@ $< $(CFLAGS)

NyuShell.o: $(SOURCEDIR)/NyuShell.cpp $(SOURCEDIR)/NyuShell.hpp
	$(CC) -c -o $(BUILDDIR)/$@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(BUILDDIR)/*.o nyush
