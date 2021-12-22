// Author: Nikhil Kumawat
// Date: 06/12/2021
// Program: LU Decomposition using Gaussian Elimination

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <climits>
#include <vector>
#include <cmath>
#include "utils.h"

using namespace std;

typedef struct arrguments{
	int threadNo;
	int col;
}arg;

vector <vector <double>> matrix;
vector <vector <double>> lowerMatrix;
vector <vector <double>> upperMatrix;
vector <vector <double>> permutationMatrix;
int matrixSize = 0;		//size of matrix.
int threadCount = 4;	//Number of threads.

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
		
	//cout << args->threadNo << " " << start << " " << end << endl;		//Debug
	for (int i = start; i <= end;++i){		//parallalize
		for (int j = col; j < matrixSize; ++j){
			matrix[i][j] -= lowerMatrix[i][col-1]*upperMatrix[col-1][j];
		}
	}
	pthread_exit(NULL);
}

void luDecomposition(){
	pthread_t t[threadCount];
	
	for (int col = 0; col < matrixSize; ++col){
		double maxPivot = 0;
		int swapRowNo = col;
		
		for (int i = col; i < matrixSize; ++i){		//This can be parallalize.
			if (abs(matrix[i][col]) > maxPivot){
				maxPivot = abs(matrix[i][col]);
				swapRowNo = i;
			}
		}
		
		if(maxPivot == 0) {
            cout << "Given matrix is singular. LU decomposition can't be calculated.\n";
            exit(1);
        }
        
        swap(permutationMatrix[col], permutationMatrix[swapRowNo]);
		swap(matrix[col], matrix[swapRowNo]);
		
		for(int i = 0; i < col; i++) {			//This can be parallalize
            swap(lowerMatrix[col][i], lowerMatrix[swapRowNo][i]);
        }
        
        upperMatrix[col][col] = matrix[col][col];

        for(int i = col+1; i < matrixSize; i++) {	//This can be parallalize
            lowerMatrix[i][col] = matrix[i][col] / upperMatrix[col][col];
            upperMatrix[col][i] = matrix[col][i];
        }
		
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
}

int main(int argc, char ** argv){
	
	//Matrix input
	if (argc == 3) {
        // expected arguments are `-n` and `size`
        // generates a matrix of order`size` with random values
        matrixSize = atoi(argv[2]);
        matrix = generate_random_matrix(matrixSize);
    } 
    else if (argc == 2) {
        // expected argument is `fileName`
        // first line contains the size of matrix and further lines contains the entries of matrix
        ifstream file;
        file.open(argv[1], ios::in);
        if (!file.is_open()) {
          cout << "Can't open the file. Exiting..." << endl;
          exit(1);
        }

        matrix = read_matrix(file);
        matrixSize = matrix.size();
    } 
    else {
        // exit if program is executed in none of the above format
        cout << "Either provide size of the matrix or the entire matrix from an input file." << endl;
        exit(1);
    }
    
    upperMatrix.resize(matrixSize, vector <double> (matrixSize, 0));
	lowerMatrix.resize(matrixSize, vector <double> (matrixSize, 0));
	permutationMatrix.resize(matrixSize, vector <double> (matrixSize, 0));
	
	//Initializtion of lower matrix and permutation matrix.
	//Diagonal elements are set to 1.
	for (int i = 0; i < matrixSize; ++i){	//O(n)
		lowerMatrix[i][i] = 1;
		permutationMatrix[i][i] = 1;
	}
	
	luDecomposition();
	
	save_matrix(lowerMatrix, "lowerMatrix");
	save_matrix(upperMatrix, "upperMatrix");
	save_matrix(permutationMatrix, "permutationMatrix");
}
