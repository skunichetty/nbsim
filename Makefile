# Makefile for compiling code

CXX = g++ # compiler
FLAGS = -std=c++17 -Werror -Wall -Wconversion -pedantic # compiler flags
EXECUTABLE = nbody
OBJECTDIR = build
SOURCEDIR = src
INCLUDEDIR = include
_SRC = $(notdir $(wildcard src/*.cpp))
SOURCES = $(addprefix src/, $(_SRC))
OBJECTS = $(patsubst %, $(OBJECTDIR)/%, $(_SRC:%.cpp=%.o))

# release executable
release: FLAGS += -O3
release: $(OBJECTS)
	$(CXX) $(FLAGS) $(OBJECTS) -o $(EXECUTABLE) 

# debug executable
debug: FLAGS += -g3
debug: $(OBJECTS)
	$(CXX) $(FLAGS) $(OBJECTS) -o $(EXECUTABLE)_debug

# clean up extra files
clean:
	rm -rf nbody* $(OBJECTS)

# compile sources into objects
$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CXX) $(FLAGS) -c -I $(INCLUDEDIR) $< -o $@