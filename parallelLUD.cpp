// Author: Nikhil Kumawat
// Date: 06/12/2021
// Program: LU Decomposition using Gaussian Elimination

#include <iostream>

using namespace std;

int main(){
	int matrixSize = 0;	//Matrix Size
	cout << "Enter size of the matrix: " << endl;
	cin >> matrixSize;
	cout << "Step 1: Random matrix of size " << matrixSize << "x" << matrixSize << " will be generated." << endl;
	
	//Matrix is taken as input first creating the matrix.
	float **matrix = new float*[matrixSize];
	for (int i = 0; i < matrixSize; ++i){
		matrix[i] = new float[matrixSize];
	}
	for (int i = 0; i < matrixSize; ++i){
		for (int j = 0; j < matrixSize; ++j){
			matrix[i][j] = rand() % 100;
		}
	}
	cout << "Step 1.1: Random matrix is generated of size: " << sizeof(float)*matrixSize*matrixSize << "Bytes." << endl;
	
}
