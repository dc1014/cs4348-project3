# Daniel Casper
# djc062000@utdallas.edu
# CS3377.002

CXX = g++
CPPFLAGS = -std=c++1z -Wall
PROJECTNAME = CS4348.Project3
EXECFILE = main

OBJS = main.o FileSystem.o CtsFileSystem.o IndexedFileSystem.o

all: $(EXECFILE)

clean:
	rm -f $(OBJS) $(EXECFILE)

$(EXECFILE): $(OBJS)
	$(CXX) -o $@ $(OBJS)

