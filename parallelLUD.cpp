// Author: Nikhil Kumawat, Chandrakishor
// Date: 06/12/2021
// Program: LU Decomposition using Gaussian Elimination

#include <iostream>
#include <climits>
#include <pthread.h>
#include <cmath>

using namespace std;

typedef struct arrguments{
	float **matrix;
	float **lowerMatrix;
	float **upperMatrix;
	int threadNo;
	int col;
}arg;

int matrixSize = 0;		//size of matrix.
const int threadCount = 4;	//Number of threads.

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
	float totalRows = matrixSize - col;	//get total rows
	int rowsPerThread = ceil (totalRows / (float)threadCount);
	if (rowsPerThread == 0)
		rowsPerThread = totalRows;
	int start = col + rowsPerThread*(args->threadNo), end = start + rowsPerThread - 1;
	if (end >= matrixSize)
		end = matrixSize - 1;
	cout << args->threadNo << " " << start << " " << end << endl;
	for (int i = start; i <= end;++i){
		args->lowerMatrix[i][col-1] = args->matrix[i][col-1] / args->matrix[col-1][col-1];
		args->upperMatrix[col-1][i] = args->matrix[col-1][i];
		for (int j = start; j < matrixSize; ++j){
			args->matrix[i][j] = args->matrix[i][j] - (args->matrix[i][col-1] / args->matrix[col-1][col-1])*(args->matrix[i][col]);
		}
	}
	/*for (int i = 0; i < matrixSize; ++i){
		cout << args->upperMatrix[col-1][i] << " ";
	}
	cout << endl;*/
	pthread_exit(NULL);
}

void luDecomposition(arg *&args){
	pthread_t t[threadCount];
	
	for (int col = 0; col < matrixSize; ++col){
		int maxPivot = INT_MIN;
		int swapRowNo = 0;
		
		for (int i = col; i < matrixSize; ++i){		//This can be parallalize.
			if (args->matrix[i][col] > maxPivot){
				maxPivot = args->matrix[i][col];
				swapRowNo = i;
			}
		}
		
		swap(args->matrix[col], args->matrix[swapRowNo]);
		//printMatrix(args->matrix);		Debug
		args->col = col;
		for (int threadNo = 0; threadNo < threadCount; ++threadNo){
			args->threadNo = threadNo;
			pthread_create(t+threadNo, NULL, &getLowerUpperMatrix, (void*)args);
		}
		
		for (int i = 0; i < threadCount; ++i)
			pthread_join(t[i], NULL);
			
		
	}
	/*for (int i = 0; i < matrixSize; ++i){
		for (int j = 0; j < matrixSize; ++j){
			cout << args->upperMatrix[i][j] << "\t";
		}
		cout << endl;
	}*/
}

int main(){

	arg *args = new arg();	
	
	cout << "Enter size of the matrix: " << endl;
	cin >> matrixSize;
	
	//Random matrix is created.
	args->matrix = new float*[matrixSize];
	args->lowerMatrix = new float*[matrixSize];
	args->upperMatrix = new float*[matrixSize];
	
	//Dynamic matrix size allocation.
	for (int i = 0; i < matrixSize; ++i){
		args->matrix[i] = new float[matrixSize];
		args->lowerMatrix[i] = new float[matrixSize];
		args->upperMatrix[i] = new float[matrixSize];
	}
	srand (time(NULL));
	//Random matrix is created.
	for (int i = 0; i < matrixSize; ++i){
		for (int j = 0; j < matrixSize; ++j){
			args->matrix[i][j] = rand() % 10;
		}
	}
	
	//printMatrix(args->matrix);		//Debug
	
	//Initializtion of lower matrix.
	//Diagonal elements are set to 1.
	for (int i = 0; i < matrixSize; ++i){	//O(n)
		args->lowerMatrix[i][i] = 1;
	}
	
	//printMatrix(lowerMatrix); 	//Debug
	
	cout << "Step 1: Random matrix is successfully created of size: " << sizeof(float)*matrixSize*matrixSize << "Bytes." << endl;
	
	luDecomposition(args);
}
