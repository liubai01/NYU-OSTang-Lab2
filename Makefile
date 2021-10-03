CC=gcc
CFLAGS=-lstdc++ 
SOURCEDIR = src
BUILDDIR = build
OBJECTS = utils CdCmd CmdBase NyuShell ExitCmd ShellStatus SubProcess Job JobsCmd FgCmd

.PHONY: all
all: nyush

nyush: nyush.o $(addsuffix .o, $(OBJECTS))
	$(CC) -o $@ $(addprefix $(BUILDDIR)/,$^) $(CFLAGS)

nyush.o: $(SOURCEDIR)/nyush.cpp
	$(CC) -c -o $(BUILDDIR)/$@ $^ $(CFLAGS)

%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.hpp
	$(CC) -c -o $(BUILDDIR)/$@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(BUILDDIR)/*.o nyush
