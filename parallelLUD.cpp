// Author: Nikhil Kumawat
// Date: 06/12/2021
// Program: LU Decomposition using Gaussian Elimination

#include <iostream>

using namespace std;


void luDecomposition(float **matrix, float **&lowerMatrix, float **&upperMatrix, int matrixSize){
	
}

int main(){
	int matrixSize = 0;	//Matrix Size
	cout << "Enter size of the matrix: " << endl;
	cin >> matrixSize;
	
	//Random matrix is created.
	float **matrix = new float*[matrixSize];
	float **lowerMatrix = new float*[matrixSize];
	float **upperMatrix = new float*[matrixSize];
	
	//Dynamic matrix size allocation.
	for (int i = 0; i < matrixSize; ++i){
		matrix[i] = new float[matrixSize];
		lowerMatrix[i] = new float[matrixSize];
		upperMatrix[i] = new float[matrixSize];
	}
	
	//Random matrix is created.
	for (int i = 0; i < matrixSize; ++i){
		for (int j = 0; j < matrixSize; ++j){
			matrix[i][j] = rand() % 100;
		}
	}
	
	//initializtion of lower matrix.
	//Diagonal elements are set to 1.
	for (int i = 0; i < matrixSize; ++i){
		lowerMatrix[i][i] = 1;
	}
	
	cout << "Step 1: Random matrix is successfully created of size: " << sizeof(float)*matrixSize*matrixSize << "Bytes." << endl;
	
	luDecomposition(matrix, lowerMatrix, upperMatrix, matrixSize);
}
