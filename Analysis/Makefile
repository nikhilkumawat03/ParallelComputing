all: parallelLUD ompLUD

parallelLUD:
	g++ -pthread parallelLUD.cpp utils.cpp -o parallelLUD
	
ompLUD:
	g++ -fopenmp ompLUD.cpp utils.cpp -o ompLUD
	
clean:
	rm *.o parallelLUD ompLUD
