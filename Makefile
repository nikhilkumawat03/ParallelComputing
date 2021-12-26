all: serialLUD parallelLUD ompLUD verification

serialLUD: serialLUD.o utils.o
	g++ serialLUD.o utils.o -o serialLUD
	
parallelLUD: parallelLUD.o utils.o
	g++ -pthread parallelLUD.o utils.o -o parallelLUD
	
ompLUD: ompLUD.o utils.o
	g++ -fopenmp ompLUD.o utils.o -o ompLUD
	
verification: verification.o utils.o
	g++ verification.o utils.o -o verification

utils.o: utils.cpp utils.h
	g++ -c utils.cpp
	
serialLUD.o: serialLUD.cpp
	g++ -c serialLUD.cpp

parallelLUD.o: parallelLUD.cpp
	g++ -c parallelLUD.cpp
	
ompLUD.o: ompLUD.cpp
	g++ -c ompLUD.cpp

verification.o: verification.cpp
	g++ -c verification.cpp

clean:
	rm *.o serialLUD parallelLUD ompLUD verification lowerMatrix upperMatrix permutationMatrix originalMatrix verificationMatrix
