all: parallelLUD ompLUD generateInput

parallelLUD:
	g++ -pthread parallelLUD.cpp utils.cpp -o parallelLUD
	
ompLUD:
	g++ -fopenmp ompLUD.cpp utils.cpp -o ompLUD
	
generateInput:
	g++ generateInput.cpp -o generateInput
	
clean:
	rm *.o parallelLUD ompLUD generateInput
