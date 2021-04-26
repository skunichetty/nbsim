# Makefile for compiling code

CXX = g++
FLAGS = -std=c++17 -Werror -Wall -Wconversion -pedantic
EXECUTABLE = nbody

release:
	$(CXX) $(FLAGS) -O3 $(wildcard *.cpp) -o $(EXECUTABLE)

debug:
	$(CXX) $(FLAGS) -g3 $(wildcard *.cpp) -o $(EXECUTABLE)

clean:
	rm -rf *.o nbody*