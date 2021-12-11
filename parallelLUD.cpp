// Author: Nikhil Kumawat, Chandrakishor
// Date: 06/12/2021
// Program: LU Decomposition using Gaussian Elimination

#include <iostream>
#include <climits>
#include <pthread.h>
#include <cmath>

using namespace std;

typedef struct arrguments{
	int threadNo;
	int col;
}arg;

float **matrix;
float **lowerMatrix;
float **upperMatrix;
int matrixSize = 0;		//size of matrix.
int threadCount = 4;	//Number of threads.

void printMatrix(float **matrix){
	for (int i = 0; i < matrixSize; ++i){
		for (int j = 0; j < matrixSize; ++j){
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}
	cout << "-------------------------------\n";
}

void *getLowerUpperMatrix(void *tmp){
	arg *args = (arg*)tmp;
	int col = args->col + 1;
	int totalRows = matrixSize - col;	//get total rows
	int rowsPerThread = totalRows / threadCount;
	if (rowsPerThread == 0)
		rowsPerThread = totalRows;
	int start = col + rowsPerThread*(args->threadNo), end = start + rowsPerThread - 1;
	
	//Condition for last thread allocation
	if (args->threadNo == threadCount-1 or end >= matrixSize)
		end = matrixSize-1;
		
	//cout << args->threadNo << " " << start << " " << end << endl;		Debug
	upperMatrix[col-1][start-1] = matrix[col-1][start-1];
	for (int i = start; i <= end;++i){
		lowerMatrix[i][col-1] = matrix[i][col-1] / matrix[col-1][col-1];
		upperMatrix[col-1][i] = matrix[col-1][i];
		for (int j = start; j < matrixSize; ++j){
			matrix[i][j] = matrix[i][j] - (matrix[i][col-1] / matrix[col-1][col-1])*(matrix[i][col]);
		}
	}
	pthread_exit(NULL);
}

void luDecomposition(){
	pthread_t t[threadCount];
	
	for (int col = 0; col < matrixSize; ++col){
		int maxPivot = INT_MIN;
		int swapRowNo = 0;
		
		for (int i = col; i < matrixSize; ++i){		//This can be parallalize.
			if (matrix[i][col] > maxPivot){
				maxPivot = matrix[i][col];
				swapRowNo = i;
			}
		}
		
		swap(matrix[col], matrix[swapRowNo]);
		//printMatrix(matrix);		Debug
		
		//Get the requirment of threads.
		int remainingRows = matrixSize - (col + 1);
		while (remainingRows < threadCount)
			threadCount = threadCount / 2;
			
		
		for (int threadNo = 0; threadNo < threadCount; ++threadNo){
			arg *args = new arg();
			args->threadNo = threadNo;
			args->col = col;
			pthread_create(t+threadNo, NULL, &getLowerUpperMatrix, (void*)args);
		}
		
		for (int i = 0; i < threadCount; ++i)
			pthread_join(t[i], NULL);
			
		
	}
	/*for (int i = 0; i < matrixSize; ++i){
		for (int j = 0; j < matrixSize; ++j){
			cout << upperMatrix[i][j] << "\t";
		}
		cout << endl;
	}*/
}

int main(){
	
	cout << "Enter size of the matrix: " << endl;
	cin >> matrixSize;
	
	//Random matrix is created.
	matrix = new float*[matrixSize];
	lowerMatrix = new float*[matrixSize];
	upperMatrix = new float*[matrixSize];
	
	//Dynamic matrix size allocation.
	for (int i = 0; i < matrixSize; ++i){
		matrix[i] = new float[matrixSize];
		lowerMatrix[i] = new float[matrixSize];
		upperMatrix[i] = new float[matrixSize];
	}
	srand (time(NULL));
	//Random matrix is created.
	for (int i = 0; i < matrixSize; ++i){
		for (int j = 0; j < matrixSize; ++j){
			matrix[i][j] = rand() % 10;
		}
	}
	
	//printMatrix(matrix);		//Debug
	
	//Initializtion of lower matrix.
	//Diagonal elements are set to 1.
	for (int i = 0; i < matrixSize; ++i){	//O(n)
		lowerMatrix[i][i] = 1;
	}
	
	//printMatrix(lowerMatrix); 	//Debug
	
	cout << "Step 1: Random matrix is successfully created of size: " << sizeof(float)*matrixSize*matrixSize << "Bytes." << endl;
	printMatrix(matrix);
	luDecomposition();
	printMatrix(matrix);
	printMatrix(lowerMatrix);
	printMatrix(upperMatrix);
}
