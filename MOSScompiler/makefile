CXX = g++
CC = g++
FLAGS = -std=c++0x

all:   tundra 

tundra: tundra.cpp tunparse.o tunscan.o tuncode.o
	$(CXX) $(FLAGS) tunparse.o tunscan.o tuncode.o tundra.cpp -o tundra

tunparse.o: tunparse.cpp
	$(CXX) $(FLAGS) -c tunparse.cpp

tunscan.o: tunscan.cpp
	$(CXX) $(FLAGS) -c tunscan.cpp

tuncode.o: tuncode.cpp
	$(CXX) $(FLAGS) -c tuncode.cpp

clobber:
	rm -f *.o *.exe *.lis *.obj core micro sam macc trace.txt

