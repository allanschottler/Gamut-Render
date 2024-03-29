# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = gamut

CC       = g++
# compiling flags here
CFLAGS   = -Wall -g -I. -std=c++11 -D_GLIBCXX_PARALLEL

LINKER   = g++ -o
# linking flags here
LFLAGS   = -Wall -I. -lm

GTKFLAGS = -export-dynamic `pkg-config --cflags --libs gtk+-3.0 gtkglext-1.0 gtkglext-x11-1.0`

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f

INCLUDE = -I/usr/include/gtk-2.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/freetype2 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/atk-1.0 -I/usr/include/harfbuzz -I/usr/include/gtkglext-1.0 -I/usr/lib/gtkglext-1.0/include


LIBSDIR = -lgtkglext-x11-1.0 -lgdkglext-x11-1.0 -lGLU -lGL -lXmu -lXt -lSM -lICE -lgtk-x11-2.0 -lpangox-1.0 -lX11 -lgmodule-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lgobject-2.0 -lglib-2.0 -lfreetype -losg -losgUtil -losgGA -losgDB -losgText -losgViewer -losgSim -lOpenThreads -fopenmp



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

