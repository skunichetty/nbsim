# Makefile for compiling code

CXX = g++ # compiler
FLAGS = -std=c++17 -Werror -Wall -Wconversion -pedantic # compiler flags
EXECUTABLE = nbsim

OBJECTDIR = build
SOURCEDIR = src
INCLUDEDIR = include
TESTDIR = test
BINDIR = bin

_SRC = $(notdir $(wildcard $(SOURCEDIR)/*.cpp))
OBJECTS = $(patsubst %, $(OBJECTDIR)/%, $(_SRC:%.cpp=%.o))
_TEST = $(notdir $(wildcard $(TESTDIR)/*.cpp))
TESTS = $(patsubst %, $(BINDIR)/%, $(_TEST:%.cpp=%))

.PHONY: clean, test, cleantest

# release executable
release: FLAGS += -O3
release: $(OBJECTS)
	$(CXX) $(FLAGS) $(OBJECTS) -o $(BINDIR)/$(EXECUTABLE) 

# debug executable
debug: FLAGS += -g3
debug: $(OBJECTS)
	$(CXX) $(FLAGS) $(OBJECTS) -o $(BINDIR)/$(EXECUTABLE)_debug

# clean up extra files
clean:
	rm -rf bin/* build/* *.out

# clean up tests specifically
cleantest:
	rm -rf $(TESTS)

# make all tests
test: FLAGS += -g3
test: $(TESTS)

#----------------compile tests------------------
#vec3 tests
$(BINDIR)/vec3_tests: $(TESTDIR)/vec3_tests.cpp $(OBJECTDIR)/vec3.o
	$(CXX) $(FLAGS) -I $(INCLUDEDIR)/test -I $(INCLUDEDIR)/nbsim  $(OBJECTDIR)/vec3.o $< -o $@
#octree nodes tests
$(BINDIR)/octree_node_tests: $(TESTDIR)/octree_node_tests.cpp $(OBJECTDIR)/octree_node.o $(OBJECTDIR)/vec3.o $(OBJECTDIR)/object.o
	$(CXX) $(FLAGS) -I $(INCLUDEDIR)/test -I $(INCLUDEDIR)/nbsim  $(OBJECTDIR)/octree_node.o $(OBJECTDIR)/vec3.o $(OBJECTDIR)/object.o $< -o $@

$(BINDIR)/octree_tests: $(TESTDIR)/octree_tests.cpp $(OBJECTDIR)/object.o $(OBJECTDIR)/body.o $(OBJECTDIR)/octree.o $(OBJECTDIR)/octree_node.o $(OBJECTDIR)/vec3.o
	$(CXX) $(FLAGS) -I $(INCLUDEDIR)/test -I $(INCLUDEDIR)/nbsim  $(OBJECTDIR)/object.o $(OBJECTDIR)/body.o $(OBJECTDIR)/octree.o $(OBJECTDIR)/octree_node.o $(OBJECTDIR)/vec3.o $< -o $@
#-----------------------------------------------

# compile sources into objects
$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CXX) $(FLAGS) -c -I $(INCLUDEDIR)/nbsim $< -o $@