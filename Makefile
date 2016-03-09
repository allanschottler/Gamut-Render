# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#                thanks to http://stackoverflow.com/users/128940/beta
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = gamut

CC       = g++
# compiling flags here
CFLAGS   = -Wall -g -I. -std=c++11 -D_GLIBCXX_PARALLEL

LINKER   = g++ -o
# linking flags here
LFLAGS   = -Wall -I. -lm

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f

INCLUDE = -I/home/p/libs/freeglut/2.6.0/include    \
          -I/home/p/libs/glew/1.5.7/include        \
          -I/home/p/libs/OSG3.2/include


LIBSDIR =  -L/home/p/libs/freeglut/2.6.0/lib/Linux26g4_64 \
           -L/home/p/libs/glew/1.5.7/lib/Linux26g4_64 \
           -lGL	-lGLU -lglut -lGLEW \
           -L/home/p/libs/OSG3.2/lib/Linux26g4_64              \
           -losg -losgUtil -losgGA -losgDB -losgText -losgViewer -losgSim -lOpenThreads -fopenmp



$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(INCLUDE) $(OBJECTS) $(LIBSDIR)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

