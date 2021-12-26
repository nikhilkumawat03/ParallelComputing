all: parallelLUD ompLUD

parallelLUD: parallelLUD.o utils.o
	g++ -pthread parallelLUD.o utils.o -o parallelLUD
	
ompLUD: ompLUD.o utils.o
	g++ -fopenmp ompLUD.o utils.o -o ompLUD
	
utils.o: utils.cpp utils.h
	g++ -c utils.cpp
	
parallelLUD.o: parallelLUD.cpp
	g++ -c parallelLUD.cpp

ompLUD.o: ompLUD.cpp
	g++ -c ompLUD.cpp
	
clean:
	rm *.o parallelLUD ompLUD
