CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

all: main

main: main.cpp cayley_tree.h cayley_tree.cpp
	$(CXX) $(CXXFLAGS) -o main main.cpp cayley_tree.cpp

clean:
	rm -f main